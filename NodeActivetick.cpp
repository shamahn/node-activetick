#include <cstring>
#include <node.h>
#include "NodeActivetick.h"
#include "import/libjson/libjson.h"

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
    serverPort = args[2]->Int32Value();
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
