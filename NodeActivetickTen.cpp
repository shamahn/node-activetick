#include <cstring>
#include <node.h>
#include "NodeActivetickTen.h"
#include "import/libjson/libjson.h"

using namespace v8;

NodeActivetick::NodeActivetick()
    : m_session( &this->m_inboundMsgs ),
      m_requestor( this->m_session, &this->m_inboundMsgs ),
      m_streamer( this->m_session, &this->m_inboundMsgs ) {
}
NodeActivetick::~NodeActivetick() {
    ATShutdownAPI();
}

void NodeActivetick::Init( Handle<Object> exports ) {
    ATInitAPI();
    Local<FunctionTemplate> tpl = FunctionTemplate::New( New );
    tpl->SetClassName( String::NewSymbol( "NodeActivetick" ) );
    tpl->InstanceTemplate()->SetInternalFieldCount( 1 );

    tpl->PrototypeTemplate()->Set( String::NewSymbol( "sessionInit" ),
        FunctionTemplate::New( SessionInit )->GetFunction() );
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "getSessionHandle" ),
        FunctionTemplate::New( GetSessionHandle )->GetFunction() );
    
    tpl->PrototypeTemplate()->Set( String::NewSymbol( "popMsg" ),
        FunctionTemplate::New( PopMsg )->GetFunction() );

    Persistent<Function> constructor =
        Persistent<Function>::New( tpl->GetFunction() );
    exports->Set( String::NewSymbol( "NodeActivetick" ), constructor );
}

Handle<Value> NodeActivetick::New( const Arguments &args ) {
    HandleScope scope;
    NodeActivetick *obj = new NodeActivetick();
    obj->Wrap( args.This() );
    return args.This();
}

Handle<Value> NodeActivetick::SessionInit( const Arguments &args ) {
    HandleScope scope;
    NodeActivetick *obj = ObjectWrap::Unwrap<NodeActivetick>( args.This() );

    std::string serverIpAddress, apiUserid, userid, password;
    uint32_t serverPort = 0;

    apiUserid = *v8::String::Utf8Value( args[0]->ToString() );
    serverIpAddress = *String::Utf8Value( args[1]->ToString() );
    serverPort = args[2]->Int32Value();
    userid = *String::Utf8Value( args[3]->ToString() );
    password = *String::Utf8Value( args[4]->ToString() );
    
    ATGUID guidApiUserid = Helper::StringToATGuid(apiUserid);
    bool rc = obj->m_session.Init(guidApiUserid, serverIpAddress, serverPort, &Helper::ConvertString(userid).front(), &Helper::ConvertString(password).front());

    return scope.Close( Boolean::New( rc ) );
}
Handle<Value> NodeActivetick::GetSessionHandle( const Arguments &args ) {
    HandleScope scope;
    NodeActivetick *obj = ObjectWrap::Unwrap<NodeActivetick>( args.This() );
    return scope.Close( Integer::New( obj->m_session.GetSessionHandle() ) );
}
Handle<Value> NodeActivetick::PopMsg( const Arguments &args ) {
    HandleScope scope;
    NodeActivetick *obj = ObjectWrap::Unwrap<NodeActivetick>( args.This() );

    JSONNode newMsg;
    newMsg = obj->getInboundMsg();

    Handle<Object> retData = Object::New();
    retData = obj->m_parser.parse( newMsg );

    return scope.Close( retData );
}

//--- helper
JSONNode NodeActivetick::getInboundMsg() {
    JSONNode popped;
    if ( !this->m_inboundMsgs.empty() ) {
        popped = this->m_inboundMsgs.front();
        this->m_inboundMsgs.pop();
        return popped;
    }
    return popped;
}
