#include "Jsonifier.h"
#include "import/libjson/libjson.h"

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