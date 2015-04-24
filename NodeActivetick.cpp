#include <cstring>
#include <locale>
#include <sstream>
#include <node.h>
#include "NodeActivetick.h"
#include "import/libjson/libjson.h"
#include "import/example/Helper.h"

using namespace v8;

Persistent<Function> NodeActivetick::constructor;

NodeActivetick::NodeActivetick()
    : m_session( &this->m_inboundMsgs ),
      m_requestor( this->m_session, &this->m_inboundMsgs ),
      m_streamer( this->m_session, &this->m_inboundMsgs ) {
}
NodeActivetick::~NodeActivetick() {
    ATShutdownAPI();
}

void NodeActivetick::Init( Handle<Object> exports ) {
    Isolate* isolate = Isolate::GetCurrent();

    ATInitAPI();
    Local<FunctionTemplate> tpl = FunctionTemplate::New( isolate, New );

    tpl->SetClassName( String::NewFromUtf8( isolate, "NodeActivetick" ) );
    tpl->InstanceTemplate()->SetInternalFieldCount( 1 );

    NODE_SET_PROTOTYPE_METHOD( tpl, "sessionInit" , SessionInit );
    NODE_SET_PROTOTYPE_METHOD( tpl, "getSessionHandle" , GetSessionHandle );
    NODE_SET_PROTOTYPE_METHOD( tpl, "closeAllATRequests" , CloseAllATRequests );
    NODE_SET_PROTOTYPE_METHOD( tpl, "getMsg" , GetMsg );

    constructor.Reset( isolate, tpl->GetFunction() );
    exports->Set( String::NewFromUtf8( isolate, "NodeActivetick" ), tpl->GetFunction() );
}

void NodeActivetick::New( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope( isolate );

    if (args.IsConstructCall()) {
        double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
        NodeActivetick* obj = new NodeActivetick();
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    } else {
        const int argc = 1;
        Local<Value> argv[argc] = { args[0] };
        Local<Function> cons = Local<Function>::New(isolate, constructor);
        args.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }
}

void NodeActivetick::SessionInit( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope( isolate );
    NodeActivetick *obj = ObjectWrap::Unwrap<NodeActivetick>( args.Holder() );

    std::string serverIpAddress, apiUserid, userid, password;
    uint32_t serverPort = 0;

    apiUserid = *v8::String::Utf8Value( args[0]->ToString() );
    serverIpAddress = *String::Utf8Value( args[1]->ToString() );
    serverPort = args[2]->Uint32Value();
    userid = *String::Utf8Value( args[3]->ToString() );
    password = *String::Utf8Value( args[4]->ToString() );
    
    ATGUID guidApiUserid = Helper::StringToATGuid(apiUserid);
    bool rc = obj->m_session.Init(guidApiUserid, serverIpAddress, serverPort, &Helper::ConvertString(userid).front(), &Helper::ConvertString(password).front());

    args.GetReturnValue().Set( Boolean::New( isolate, rc ) );
}
void NodeActivetick::GetSessionHandle( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope( isolate );
    NodeActivetick *obj = ObjectWrap::Unwrap<NodeActivetick>( args.Holder() );
    args.GetReturnValue().Set( Integer::New( isolate, obj->m_session.GetSessionHandle() ) );
}

// -- requestor --
void NodeActivetick::CloseAllATRequests( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope( isolate );
    NodeActivetick *obj = ObjectWrap::Unwrap<NodeActivetick>( args.Holder() );
    obj->m_requestor.CloseAllATRequests();
}

void NodeActivetick::CloseATRequest( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope( isolate );
    NodeActivetick *obj = ObjectWrap::Unwrap<NodeActivetick>( args.Holder() );

    // TODO: incorrect type assignment
    uint64_t request = args[0]->IntegerValue();

    obj->m_requestor.CloseATRequest( request );
}

void NodeActivetick::SendATBarHistoryDbRequest( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope( isolate );
    NodeActivetick *obj = ObjectWrap::Unwrap<NodeActivetick>( args.Holder() );

    std::string symbol( *String::Utf8Value( args[0]->ToString() ) );

    ATBarHistoryType type;
    std::string historyType( *String::Utf8Value( args[1]->ToString() ) );
    if ( historyType == "BarHistoryIntraday" )
        type = BarHistoryIntraday;
    else if ( historyType == "BarHistoryDaily" )
        type = BarHistoryDaily;
    else if ( historyType == "BarHistoryWeekly" )
        type = BarHistoryWeekly;
    else {
        type = BarHistoryIntraday;
    }

    uint32_t minutes = args[2]->Uint32Value();

    std::string beginTime( *String::Utf8Value( args[3]->ToString() ) );
    std::string endTime( *String::Utf8Value( args[4]->ToString() ) );

    uint32_t timeout = DEFAULT_REQUEST_TIMEOUT;
    if ( args[5]->IsUint32() )
        timeout = args[5]->Uint32Value();

    ATTIME atBeginTime = Helper::StringToATTime(beginTime);
    ATTIME atEndTime = Helper::StringToATTime(endTime);

    ATSYMBOL atSymbol = Helper::StringToSymbol(symbol);
    uint64_t request = obj->m_requestor.SendATBarHistoryDbRequest( atSymbol, type, (uint8_t)minutes, atBeginTime, atEndTime, timeout );

    args.GetReturnValue().Set( Integer::New( isolate, request ) );
}

void NodeActivetick::SendATLoginRequest( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope( isolate );
    NodeActivetick *obj = ObjectWrap::Unwrap<NodeActivetick>( args.Holder() );

    std::string userid = *String::Utf8Value( args[0]->ToString() );
    std::string passwd = *String::Utf8Value( args[1]->ToString() );
    uint32_t timeout = DEFAULT_REQUEST_TIMEOUT;
    if ( args[2]->IsUint32() )
        timeout = args[2]->Uint32Value();

    uint64_t request = obj->m_requestor.SendATLoginRequest( &Helper::ConvertString(userid).front(),
                                                            &Helper::ConvertString(passwd).front(), timeout );
    args.GetReturnValue().Set( Integer::New( isolate, request ) );
}

void NodeActivetick::SendATMarketHolidaysRequest( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope( isolate );
    NodeActivetick *obj = ObjectWrap::Unwrap<NodeActivetick>( args.Holder() );

    uint8_t yearsGoingBack = (uint8_t)args[0]->Uint32Value();
    uint8_t yearsGoingFwd = (uint8_t)args[1]->Uint32Value();

    
    uint32_t timeout = DEFAULT_REQUEST_TIMEOUT;
    if ( args[4]->IsUint32() )
        timeout = args[4]->Uint32Value();


    std::string exchangeStr( *String::Utf8Value( args[2]->ToString() ) );
    std::string countryStr( *String::Utf8Value( args[3]->ToString() ) );

    ATExchangeType exchangeType = obj->m_enumMap.toAtExchange(exchangeStr);
    ATCountryType countryType = obj->m_enumMap.toAtCountry(countryStr);

    uint64_t request = obj->m_requestor.SendATMarketHolidaysRequest(
                           yearsGoingBack, yearsGoingFwd, exchangeType,
                           countryType, timeout );
    
    args.GetReturnValue().Set( Integer::New( isolate, request ) );
}

void NodeActivetick::SendATMarketMoversDbRequest( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope( isolate );
    NodeActivetick *obj = ObjectWrap::Unwrap<NodeActivetick>( args.Holder() );

    // QUESTION is atSymbol.symbolType = SymbolTopMarketMovers; necessary?
    std::string symbols( *String::Utf8Value( args[0]->ToString() ) );
    std::vector<ATSYMBOL> atSymbolsVector = Helper::StringToSymbols(symbols);
    ATSYMBOL* atSymbols = &atSymbolsVector[0];

    uint16_t symbolCount = (uint16_t)args[1]->Uint32Value();
    uint32_t timeout = DEFAULT_REQUEST_TIMEOUT;
    if ( args[2]->IsUint32() )
        timeout = args[2]->Uint32Value();

    uint64_t request = obj->m_requestor.SendATMarketMoversDbRequest(
                            atSymbols, symbolCount, timeout );

    args.GetReturnValue().Set( Integer::New( isolate, request ) );
}

void NodeActivetick::SendATMarketMoversStreamRequest( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope( isolate );
    NodeActivetick *obj = ObjectWrap::Unwrap<NodeActivetick>( args.Holder() );

    std::string symbol( *String::Utf8Value( args[0]->ToString() ) );
    ATSYMBOL atSymbol = Helper::StringToSymbol(symbol);

    std::string requestStr( *String::Utf8Value( args[1]->ToString() ) );
    ATStreamRequestType requestType = obj->m_enumMap.toAtStreamRequest(requestStr);

    uint32_t timeout = DEFAULT_REQUEST_TIMEOUT;
    if ( args[2]->IsUint32() )
        timeout = args[2]->Uint32Value();

    uint64_t request = obj->m_requestor.SendATMarketMoversStreamRequest(
                            atSymbol, requestType, timeout );

    args.GetReturnValue().Set( Integer::New( isolate, request ) );
}

void NodeActivetick::SendATQuoteDbRequest ( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope( isolate );
    NodeActivetick *obj = ObjectWrap::Unwrap<NodeActivetick>( args.Holder() );

    std::string symbols( *String::Utf8Value( args[0]->ToString() ) );
    std::vector<ATSYMBOL> atSymbolsVector = Helper::StringToSymbols(symbols);
    ATSYMBOL* atSymbols = &atSymbolsVector[0];

    uint16_t symbolCount = (uint16_t)args[1]->Uint32Value();

    std::string quoteFieldStr( *String::Utf8Value( args[2]->ToString() ) );
    //
    std::vector<ATQuoteFieldType> atQuoteFieldVector;
    std::size_t pos = 0;
    std::size_t prevpos = 0;

    while((pos = quoteFieldStr.find(",", pos)) != std::string::npos)
    {
        std::string field = quoteFieldStr.substr(prevpos, pos - prevpos);
        atQuoteFieldVector.push_back(obj->m_enumMap.toAtQuoteField(field));
        
        ++pos;
        prevpos = pos;              
    }   

    atQuoteFieldVector.push_back(obj->m_enumMap.toAtQuoteField(quoteFieldStr.substr(prevpos)));
    ATQuoteFieldType* fields = &atQuoteFieldVector[0];

    uint16_t fieldCount = (uint16_t)args[3]->Uint32Value();

    uint32_t timeout = DEFAULT_REQUEST_TIMEOUT;
    if ( args[2]->IsUint32() )
        timeout = args[2]->Uint32Value();

    uint64_t request = obj->m_requestor.SendATQuoteDbRequest(
                            atSymbols, symbolCount, fields, fieldCount, timeout );

    args.GetReturnValue().Set( Integer::New( isolate, request ) );
}

void NodeActivetick::SendATQuoteStreamRequest ( const FunctionCallbackInfo<Value> &args ) {
}

void NodeActivetick::SendATTickHistoryDbRequest ( const FunctionCallbackInfo<Value> &args ) {
}

void NodeActivetick::SendATSectorListRequest ( const FunctionCallbackInfo<Value> &args ) {
}

void NodeActivetick::SendATConstituentListRequest ( const FunctionCallbackInfo<Value> &args ) {
}

// -- Streamer --




void NodeActivetick::GetMsg( const FunctionCallbackInfo<Value> &args ) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope( isolate );
    NodeActivetick *obj = ObjectWrap::Unwrap<NodeActivetick>( args.Holder() );

    JSONNode newMsg;
    newMsg = obj->getInboundMsg();

    Handle<Object> retData = Object::New( isolate );
    retData = obj->m_parser.parse( newMsg );

    args.GetReturnValue().Set( retData );
}

///////////////////////////////////////////////////////////////////////////////
// Helper methods
///////////////////////////////////////////////////////////////////////////////

JSONNode NodeActivetick::getInboundMsg() {
    JSONNode popped;
    if ( !this->m_inboundMsgs.empty() ) {
        popped = this->m_inboundMsgs.front();
        this->m_inboundMsgs.pop();
        return popped;
    }
    return popped;
}

char *NodeActivetick::getChar( v8::Local<v8::Value> value, const char *fallback ) {
    if ( value->IsString() ) {
        v8::String::Utf8Value string( value );
        char *str  = ( char * ) malloc( string.length() + 1 );
        std::strcpy( str, *string );
        return str;
    }
    char *str = ( char * ) malloc( std::strlen( fallback ) + 1 );
    std::strcpy( str, fallback );
    return str;
}

