#include "Jsonifier.h"
#include <string.h>
#include "import/libjson/libjson.h"
#include "import/example/Helper.h"

Jsonifier::Jsonifier( void ) {
}

Jsonifier::~Jsonifier( void ) {
}

/*static*/ JSONNode Jsonifier::jsonifyAtTime( std::string name,
                                                LPATTIME pTime ) {
    JSONNode n( JSON_NODE );
    n.set_name( name );
    n.push_back( JSONNode( "year", pTime->year ) );
    n.push_back( JSONNode( "month", pTime->month ) );
    n.push_back( JSONNode( "dayOfWeek", pTime->dayOfWeek ) );
    n.push_back( JSONNode( "day", pTime->day ) );
    n.push_back( JSONNode( "hour", pTime->hour ) );
    n.push_back( JSONNode( "minute", pTime->minute ) );
    n.push_back( JSONNode( "second", pTime->second ) );
    n.push_back( JSONNode( "milliseconds", pTime->milliseconds ) );
    return n;
}

/*static*/ JSONNode Jsonifier::jsonifyAtSymbol( LPATSYMBOL pSymbol ) {
    JSONNode s( JSON_NODE );
    s.set_name( "ATSymbol" );

    char symbolStr[512] = {0};
    std::string str = Helper::ConvertString(pSymbol->symbol, Helper::StringLength(pSymbol->symbol));
    strncpy(symbolStr, str.c_str(), sizeof(symbolStr));
    s.push_back( JSONNode( "symbolStr", symbolStr ) );
    s.push_back( JSONNode( "symbolType", pSymbol->symbolType ) );
    s.push_back( JSONNode( "exchangeType", pSymbol->exchangeType ) );
    s.push_back( JSONNode( "countryType", pSymbol->countryType ) );

    return s;
}
