#include "StdAfx.h"
#include "Streamer.h"
#include <string>
#include <stdio.h>

#include "import/libjson/libjson.h"
#include "import/example/Helper.h"

using namespace std;

Streamer::Streamer( const APISession& session, 
                    std::queue< JSONNode > *pInboundMsgs)
: ActiveTickStreamListener(session.GetSessionHandle())
, Requestor(session, pInboundMsgs)
, m_session(session)
{
    m_pInboundMsgs = pInboundMsgs;
}

Streamer::~Streamer(void)
{
}

/*virtual*/ void Streamer::OnATStreamTradeUpdate(
                                    LPATQUOTESTREAM_TRADE_UPDATE pUpdate ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "ATStreamTradeUpdate" ) );

    JSONNode data( JSON_NODE );
    data.set_name( "data" );
    data.push_back( jsonifyAtQuoteStreamTradeUpdate( pUpdate ) );
    n.push_back( data );

    m_pInboundMsgs->push( n );
}

JSONNode Streamer::jsonifyAtQuoteStreamTradeUpdate( 
                                    LPATQUOTESTREAM_TRADE_UPDATE pUpdate ) {
    JSONNode n( JSON_NODE );
    n.push_back( m_jsonifier.jsonifyAtSymbol( &pUpdate->symbol ) );
    n.push_back( JSONNode( "flags", pUpdate->flags ) );

// TODO: Not sure about this
    n.push_back( JSONNode( "condition", pUpdate->condition ) );

    n.push_back( JSONNode( "lastExchange", pUpdate->lastExchange ) );

    JSONNode last( JSON_NODE );
    last.set_name( "lastPrice" );
    last.push_back( JSONNode( "precision", pUpdate->lastPrice.precision ) );
    last.push_back( JSONNode( "price", pUpdate->lastPrice.price ) );
    n.push_back( last );

    n.push_back( JSONNode( "lastSize", pUpdate->lastSize ) );
    n.push_back( m_jsonifier.jsonifyAtTime( "time", &pUpdate->lastDateTime ) );

    return n;
}

/*virtual*/ void Streamer::OnATStreamQuoteUpdate(
                                    LPATQUOTESTREAM_QUOTE_UPDATE pUpdate ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "ATStreamQuoteUpdate" ) );

    JSONNode data( JSON_NODE );
    data.set_name( "data" );
    data.push_back( jsonifyAtQuoteStreamQuoteUpdate( pUpdate ) );
    n.push_back( data );

    m_pInboundMsgs->push( n );
}

JSONNode Streamer::jsonifyAtQuoteStreamQuoteUpdate( LPATQUOTESTREAM_QUOTE_UPDATE pUpdate ) {
    JSONNode n( JSON_NODE );
    n.push_back( m_jsonifier.jsonifyAtSymbol( &pUpdate->symbol ) );
    n.push_back( JSONNode( "condition", pUpdate->condition ) );
    n.push_back( JSONNode( "bidExchange", pUpdate->bidExchange ) );
    n.push_back( JSONNode( "askExchange", pUpdate->askExchange ) );

    JSONNode bid( JSON_NODE );
    bid.set_name( "bidPrice" );
    bid.push_back( JSONNode( "precision", pUpdate->bidPrice.precision ) );
    bid.push_back( JSONNode( "price", pUpdate->bidPrice.price ) );
    n.push_back( bid );

    JSONNode ask( JSON_NODE );
    ask.set_name( "askPrice" );
    ask.push_back( JSONNode( "precision", pUpdate->askPrice.precision ) );
    ask.push_back( JSONNode( "price", pUpdate->askPrice.price ) );
    n.push_back( ask );

    n.push_back( JSONNode( "bidSize", pUpdate->bidSize ) );
    n.push_back( JSONNode( "askSize", pUpdate->askSize ) );

    n.push_back( m_jsonifier.jsonifyAtTime( "time", &pUpdate->quoteDateTime ) );

    return n;
}

/*virtual*/ void Streamer::OnATStreamTopMarketMoversUpdate(
                                    LPATMARKET_MOVERS_STREAM_UPDATE pUpdate ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "ATStreamTopMarketMoversUpdate" ) );

    JSONNode data( JSON_NODE );
    data.set_name( "data" );
    data.push_back( jsonifyAtMarketMoversStreamUpdate( pUpdate ) );
    n.push_back( data );

    m_pInboundMsgs->push( n );
}

JSONNode Streamer::jsonifyAtMarketMoversStreamUpdate( LPATMARKET_MOVERS_STREAM_UPDATE pUpdate ) {
    JSONNode n( JSON_NODE );
    n.push_back( m_jsonifier.jsonifyAtTime( "lastUpdateTime", &pUpdate->lastUpdateTime ) );

    JSONNode mmovers( JSON_NODE );
    mmovers.set_name( "marketMovers" );
    mmovers.push_back( m_jsonifier.jsonifyAtSymbol( &pUpdate->marketMovers.symbol ) );
    mmovers.push_back( JSONNode( "status", pUpdate->marketMovers.status ) );

    JSONNode records( JSON_ARRAY );
    records.set_name( "dataItems" );

    for( int32_t i = 0; i < ATMarketMoversMaxRecords; i++ ) {
        JSONNode item( JSON_NODE );
        item.push_back( m_jsonifier.jsonifyAtSymbol( &pUpdate->marketMovers.items[i].symbol ) );
        item.push_back( JSONNode( "name", pUpdate->marketMovers.items[i].name ) );

        JSONNode last( JSON_NODE );
        last.set_name( "lastPrice" );
        last.push_back( JSONNode( "precision",  pUpdate->marketMovers.items[i].lastPrice.precision ) );
        last.push_back( JSONNode( "price",  pUpdate->marketMovers.items[i].lastPrice.price ) );
        item.push_back( last );

        JSONNode close( JSON_NODE );
        close.set_name( "closePrice" );
        close.push_back( JSONNode( "precision",  pUpdate->marketMovers.items[i].closePrice.precision ) );
        close.push_back( JSONNode( "price",  pUpdate->marketMovers.items[i].closePrice.price ) );
        item.push_back( close );

        item.push_back( JSONNode( "volume",  pUpdate->marketMovers.items[i].volume ) );
        item.push_back( m_jsonifier.jsonifyAtTime( "lastDateTime", &pUpdate->marketMovers.items[i].lastDateTime ) );

        records.push_back( item );
    }
    n.push_back( records );

    return n;
}
