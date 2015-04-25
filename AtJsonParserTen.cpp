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
             node_name == "response" ||
             node_name == "data" ||
             node_name == "records" ||
             node_name == "ATBarHistory" ||
             node_name == "ATSymbol" ||
             node_name == "ATLoginResponse" ||
             node_name == "time" ||
             node_name == "open" ||
             node_name == "high" ||
             node_name == "low" ||
             node_name == "close" ||
             node_name == "ATTickHistory" ||
             node_name == "offsetDatabaseDate" ||
             node_name == "tradeLastPrice" ||
             node_name == "quoteBidPrice" ||
             node_name == "quoteAskPrice" ||
             node_name == "ATMarketMoversDb" ||
             node_name == "recordSymbol" ||
             node_name == "itemSymbol" ||
             node_name == "itemLastPrice" ||
             node_name == "ATQuoteDb" ||
             node_name == "dataItems" ||
             node_name == "priceData" ||
             node_name == "dateTime" ||
             node_name == "ATQuoteStream" ||
             node_name == "lastPrice" ||
             node_name == "bidPrice" ||
             node_name == "askPrice" ||
             node_name == "lastUpdateTime" ||
             node_name == "marketMovers" ||
             node_name == "closePrice" ||
             node_name == "lastDateTime"
             
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
    if ( node_name == "messageId" ||
         node_name == "name" ||
         node_name == "type" ||
         node_name == "sector" ||
         node_name == "industry" ||
         node_name == "symbolStr" ||
         node_name == "tradeLastExchange" ||
         node_name == "quoteBidExchange" ||
         node_name == "quoteAskExchange" ||
         node_name == "byte" ||
         node_name == "string" ||
         node_name == "unicodeString" ||
         node_name == "bidExchange" ||
         node_name == "askExchange"
       )
        return true;
    return false;
}


bool AtJsonParser::filterAsNumber( const std::string& node_name ) {
    if ( node_name == "price"
       )
        return true;
    return false;
}

bool AtJsonParser::filterAsInteger( const std::string& node_name ) {
    if ( node_name == "hSession" ||
         node_name == "hOrigRequest" ||
         node_name == "hRequest" ||
         node_name == "statusType" ||
         node_name == "loginResponse" || 
         node_name == "origRequest" ||
         node_name == "enum" ||
         node_name == "count" ||
         node_name == "precision" ||
         node_name == "volume" ||
         node_name == "symbolType" ||
         node_name == "exchangeType" ||
         node_name == "countryType" ||
         node_name == "nextOffset" ||
         node_name == "tradeLastSize" ||
         node_name == "tradeCondition" ||
         node_name == "quoteBidSize" ||
         node_name == "quoteAskSize" ||
         node_name == "quoteCondition" ||
         node_name == "itemVolume" ||
         node_name == "uint32" ||
         node_name == "uint64" ||
         node_name == "int32" ||
         node_name == "int64" ||
         node_name == "dataItemQuoteFieldType" ||
         node_name == "dataItemFieldStatus" ||
         node_name == "year" ||
         node_name == "month" ||
         node_name == "dayOfWeek" ||
         node_name == "day" ||
         node_name == "hour" ||
         node_name == "minute" ||
         node_name == "second" ||
         node_name == "milliseconds" ||
         node_name == "lastSize" ||
         node_name == "condition" ||
         node_name == "bidSize" ||
         node_name == "askSize" ||
         node_name == "status"
       )
        return true;
    return false;
}

bool AtJsonParser::filterAsBoolean( const std::string& node_name ) {
    return false;
}
