#include "AtJsonParser.h"
#include "import/libjson/libjson.h"

AtJsonParser::AtJsonParser()
{
}
AtJsonParser::~AtJsonParser()
{
}

Handle<Object> AtJsonParser::parse( const JSONNode &n ) {
    Handle<Object> retData = Object::New();
    JSONNode::const_iterator i = n.begin();
    while ( i != n.end() ) {
        std::string node_name = i->name();
        if ( node_name == "serverTime" ||
             node_name == "atLoginResponse"
              ) {
            retData->Set( String::NewSymbol( node_name.c_str() ), parse( *i ) );
        }
        else if ( filterAsString( node_name ) ) {
            retData->Set( String::NewSymbol( node_name.c_str() ),
                          String::New( i->as_string().c_str() ) );
        }
        else if ( filterAsNumber( node_name ) ) {
            retData->Set( String::NewSymbol( node_name.c_str() ),
                          Number::New( i->as_float() ) );
        }
        else if ( filterAsInteger( node_name ) ) {
            retData->Set( String::NewSymbol( node_name.c_str() ),
                          Integer::New( i->as_int() ) );
        }
        else if ( filterAsBoolean( node_name ) ) {
            retData->Set( String::NewSymbol( node_name.c_str() ),
                          Boolean::New( i->as_bool() ) );
        }
        ++i;
    }
    return retData;
}

bool AtJsonParser::filterAsString( const std::string& node_name ) {
    if ( node_name == "messageId" )
        return true;
    return false;
}


bool AtJsonParser::filterAsNumber( const std::string& node_name ) {
    return false;
}

bool AtJsonParser::filterAsInteger( const std::string& node_name ) {
    if ( node_name == "hSession" ||
         node_name == "statusType" )
        return true;
    return false;
}

bool AtJsonParser::filterAsBoolean( const std::string& node_name ) {
    return false;
}
