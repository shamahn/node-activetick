#include "StdAfx.h"
#include "Requestor.h"
#include <ActiveTickServerAPI/ATBarHistoryDbResponseParser.h>
#include <ActiveTickServerAPI/ATMarketMoversDbResponseParser.h>
#include <ActiveTickServerAPI/ATQuoteDbResponseParser.h>
#include <ActiveTickServerAPI/ATQuoteStreamResponseParser.h>
#include <ActiveTickServerAPI/ATTickHistoryDbResponseParser.h>

#include <fstream>
#include <stdio.h>
#include <string>
#include <string.h>

#include <node.h>
#include "import/example/Helper.h"
#include "import/libjson/libjson.h"

using namespace std;

Requestor::Requestor( const APISession& session,
                      std::queue< JSONNode > *pInboundMsgs)
: ActiveTickServerRequestor(session.GetSessionHandle())
{
}

Requestor::~Requestor(void)
{
}

/*virtual*/ void Requestor::OnATBarHistoryDbResponse(
                                    uint64_t origRequest,
                                    ATBarHistoryResponseType responseType,
                                    LPATBARHISTORY_RESPONSE pResponse) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "atBarHistoryDb" ) );
    n.push_back( JSONNode( "origRequest", origRequest ) );

    string strResponseType;
    switch(responseType)
    {
    case BarHistoryResponseSuccess: strResponseType = "BarHistoryResponseSuccess"; break;
    case BarHistoryResponseInvalidRequest: strResponseType = "BarHistoryResponseInvalidRequest"; break;
    case BarHistoryResponseMaxLimitReached: strResponseType = "BarHistoryResponseMaxLimitReached"; break;
    case BarHistoryResponseDenied: strResponseType = "BarHistoryResponseDenied"; break;
    default: break;
    }

    JSONNode resp( JSON_NODE );
    resp.set_name( "response" );
    resp.push_back( JSONNode( "name", "ATBarHistoryResponseType" ) );
    resp.push_back( JSONNode( "enum", responseType ) );
    resp.push_back( JSONNode( "type", strResponseType ) );

    JSONNode data( JSON_NODE );
    data.set_name( "data" );
    data.push_back( jsonifyAtBarHistory( pResponse ) );

    resp.push_back( data );

    n.push_back( resp );
    m_pInboundMsgs->push( n );
}

JSONNode Requestor::jsonifyAtBarHistory( LPATBARHISTORY_RESPONSE pResponse ) {
    ATBarHistoryDbResponseParser parser(pResponse);

    JSONNode n( JSON_NODE );
    n.set_name( "atBarHistory" );
    n.push_back( m_jsonifier.jsonifyAtSymbol( parser.GetSymbol() ) );
    n.push_back( JSONNode( "recordsCount", parser.GetRecordCount() ) );

    if( parser.MoveToFirstRecord() ) {
        JSONNode c( JSON_ARRAY );
        c.set_name( "records");
        while( true ) {
            JSONNode bar( JSON_NODE );

            ATTIME recordDateTime = parser.GetDateTime();

            bar.push_back( m_jsonifier.jsonifyAtTime( "barTime",
                                                    &recordDateTime ) );
            
            JSONNode open( JSON_NODE );
            open.set_name( "open" );
            open.push_back( JSONNode( "precision",
                                      parser.GetOpen().precision ) );
            open.push_back( JSONNode( "price", parser.GetOpen().price ) );
            bar.push_back( open );

            JSONNode high( JSON_NODE );
            high.set_name( "high" );
            high.push_back( JSONNode( "precision",
                                      parser.GetHigh().precision ) );
            high.push_back( JSONNode( "price", parser.GetHigh().price ) );
            bar.push_back( high );

            JSONNode low( JSON_NODE );
            low.set_name( "low" );
            low.push_back( JSONNode( "precision", parser.GetLow().precision ) );
            low.push_back( JSONNode( "price", parser.GetLow().price ) );
            bar.push_back( low );

            JSONNode close( JSON_NODE );
            close.set_name( "close" );
            close.push_back( JSONNode( "precision",
                                       parser.GetClose().precision ) );
            close.push_back( JSONNode( "price", parser.GetClose().price ) );
            bar.push_back( close );

            bar.push_back( JSONNode( "volume", parser.GetVolume() ) );
            c.push_back( bar );

            if(parser.MoveToNextRecord() == false)
                break;
        }
        n.push_back( c );
    }

    return n;
}

/*virtual*/ void Requestor::OnATSectorListResponse(
                                    uint64_t origRequest,
                                    LPATSECTORLIST_RECORD pRecords,
                                    uint32_t recordsCount ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "atSectorList" ) );
    n.push_back( JSONNode( "origRequest", origRequest ) );
    n.push_back( JSONNode( "count", recordsCount ) );

    JSONNode list( JSON_ARRAY );
    list.set_name( "data" );
    for(uint32_t i = 0; i < recordsCount; ++i) {
        JSONNode item( JSON_NODE );
        item.push_back( JSONNode( "sector", Helper::ConvertString(
                pRecords[i].sector, ATSectorNameMaxLength).c_str() ) );
        item.push_back( JSONNode( "industry", Helper::ConvertString(
                pRecords[i].industry, ATIndustryNameMaxLength).c_str() ) );
        list.push_back(item);
    }
    m_pInboundMsgs->push( n );
}

/*virtual*/ void Requestor::OnATConstituentListResponse(
                                    uint64_t origRequest,
                                    LPATSYMBOL pSymbols,
                                    uint32_t symbolsCount ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "atConstituentList" ) );
    n.push_back( JSONNode( "origRequest", origRequest ) );
    n.push_back( JSONNode( "count", symbolsCount ) );
    
    JSONNode list( JSON_ARRAY );
    list.set_name( "data" );
    for(uint32_t i = 0; i < symbolsCount; ++i) {
        JSONNode symbol( JSON_NODE );
        symbol.push_back( JSONNode( "symbol", Helper::ConvertString(
                pSymbols[i].symbol, _countof(pSymbols[i].symbol)).c_str() ) );
        symbol.push_back( JSONNode( "symbolType", pSymbols[i].symbolType ) );
        symbol.push_back( JSONNode( "exchangeType", pSymbols[i].exchangeType ));
        symbol.push_back( JSONNode( "countryType", pSymbols[i].countryType ) );

        list.push_back( symbol );
    }
    m_pInboundMsgs->push( n );
}


/*virtual*/ void Requestor::OnATTickHistoryDbResponse(
                                    uint64_t origRequest,
                                    ATTickHistoryResponseType responseType,
                                    LPATTICKHISTORY_RESPONSE pResponse ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "atTickHistoryDb" ) );
    n.push_back( JSONNode( "origRequest", origRequest ) );

    string strResponseType;
    switch(responseType)
    {
    case TickHistoryResponseSuccess: strResponseType = "TickHistoryResponseSuccess"; break;
    case TickHistoryResponseInvalidRequest: strResponseType = "TickHistoryResponseInvalidRequest"; break;
    case TickHistoryResponseMaxLimitReached: strResponseType = "TickHistoryResponseMaxLimitReached"; break;
    case TickHistoryResponseDenied: strResponseType = "TickHistoryResponseDenied"; break;
    default: break;
    }
    JSONNode resp( JSON_NODE );
    resp.set_name( "response" );
    resp.push_back( JSONNode( "name", "ATTickHistoryResponseType" ) );
    resp.push_back( JSONNode( "enum", responseType ) );
    resp.push_back( JSONNode( "type", strResponseType ) );

    JSONNode data( JSON_NODE );
    data.set_name( "data" );
    data.push_back( jsonifyAtTickHistory( pResponse ) );

    resp.push_back( data );

    n.push_back( resp );
    m_pInboundMsgs->push( n );

    if( pResponse->nextOffset != 0xffffffffffffffffULL &&
        pResponse->nextOffset != 0xfffffffffffffff0ULL &&
        pResponse->nextOffset != 0 )
        SendATTickHistoryDbRequest( pResponse->symbol, true, true, 10,
                                    pResponse->nextOffset,
                                    pResponse->offsetDatabaseDate,
                                    DEFAULT_REQUEST_TIMEOUT);

}

JSONNode Requestor::jsonifyAtTickHistory( LPATTICKHISTORY_RESPONSE pResponse ) {
    ATTickHistoryDBResponseParser parser(pResponse);

    JSONNode n( JSON_NODE );
    n.set_name( "atTickHistory" );
    n.push_back( m_jsonifier.jsonifyAtSymbol( parser.GetSymbol() ) );
    n.push_back( jsonifyAtSymbolStatus( parser.GetSymbolStatus() ) );
    n.push_back( JSONNode( "recordCount", parser.GetRecordCount() ) );
    n.push_back( JSONNode( "nextOffset", parser.GetNextOffset() ) );
    ATTIME offsetDatabaseDate = parser.GetOffsetDbDate();
    n.push_back( m_jsonifier.jsonifyAtTime( "offsetDatabaseDate",
                                            &offsetDatabaseDate ) );

    if( parser.MoveToFirstRecord() ) {
        JSONNode c( JSON_ARRAY );
        c.set_name( "records" );
        while( true ) {
            JSONNode tick( JSON_NODE );

            ATTIME recordDateTime = parser.GetRecordDateTime();
            tick.push_back( m_jsonifier.jsonifyAtTime( "tickTime",
                                                    &recordDateTime ) );
            switch( parser.GetRecordType() )
            {
            case TickHistoryRecordTrade:
                {
                JSONNode last( JSON_NODE );
                last.set_name( "tradeLastPrice" );
                last.push_back( JSONNode( "precision",
                                    parser.GetTradeLastPrice().precision ) );
                last.push_back( JSONNode( "price",
                                    parser.GetTradeLastPrice().price ) );
                tick.push_back( last );
                tick.push_back( JSONNode( "tradeLastSize", parser.GetTradeLastSize() ) );
                tick.push_back( JSONNode( "tradeLastExchange", parser.GetTradeLastExchange() ) );
                tick.push_back( JSONNode( "tradeCondition", parser.GetTradeCondition(0) ) );
                }
                break;
            case TickHistoryRecordQuote:
                {
                JSONNode bid( JSON_NODE );
                bid.set_name( "quoteBidPrice" );
                bid.push_back( JSONNode( "precision",
                                   parser.GetQuoteBidPrice().precision ) );
                bid.push_back( JSONNode( "price",
                                   parser.GetQuoteBidPrice().price ) );
                tick.push_back( bid );
                JSONNode ask( JSON_NODE );
                ask.set_name( "quoteAskPrice" );
                ask.push_back( JSONNode( "precision",
                                   parser.GetQuoteAskPrice().precision ) );
                ask.push_back( JSONNode( "price",
                                   parser.GetQuoteAskPrice().price ) );
                tick.push_back( ask );
                tick.push_back( JSONNode( "quoteBidSize", parser.GetQuoteBidSize() ) );
                tick.push_back( JSONNode( "quoteAskSize", parser.GetQuoteAskSize() ) );
                tick.push_back( JSONNode( "quoteBidExchange", parser.GetQuoteBidExchange() ) );
                tick.push_back( JSONNode( "quoteAskExchange", parser.GetQuoteAskExchange() ) );
                tick.push_back( JSONNode( "quoteCondition", parser.GetQuoteCondition() ) );
                }
                break;
            default:
                break;
            }
            c.push_back( tick );
            if(parser.MoveToNextRecord() == false)
                break;
            
        }
        n.push_back( c );
    }
    return n;
}

JSONNode Requestor::jsonifyAtSymbolStatus( ATSymbolStatus status ) {
    string strStatusType;
    switch(status)
    {
    case SymbolStatusSuccess: strStatusType = "SymbolStatusSuccess"; break;
    case SymbolStatusInvalid: strStatusType = "SymbolStatusInvalid"; break;
    case SymbolStatusUnavailable: strStatusType = "SymbolStatusUnavailable"; break;
    case SymbolStatusNoPermission: strStatusType = "SymbolStatusNoPermission"; break;
    default: break;
    }
    JSONNode n( JSON_NODE );
    n.set_name( "status" );
    n.push_back( JSONNode( "name", "ATSymbolStatus" ) );
    n.push_back( JSONNode( "enum", status ) );
    n.push_back( JSONNode( "type", strStatusType ) );
    return n;
}


/*virtual*/ void Requestor::OnATMarketMoversDbResponse(
                                    uint64_t origRequest,
                                    ATMarketMoversDbResponseType responseType,
                                    LPATMARKET_MOVERSDB_RESPONSE pResponse ) {
    JSONNode n( JSON_NODE );
    n.push_back( JSONNode( "messageId", "atMarketMoversDb" ) );
    n.push_back( JSONNode( "origRequest", origRequest ) );

    string strResponseType;
    switch(responseType)
    {
    case MarketMoversDbResponseSuccess: strResponseType = "MarketMoversDbResponseSuccess"; break;
    case MarketMoversDbResponseInvalidRequest: strResponseType = "MarketMoversDbResponseInvalidRequest"; break;
    case MarketMoversDbResponseDenied: strResponseType = "MarketMoversDbResponseDenied"; break;
    default: break;
    }

    JSONNode resp( JSON_NODE );
    resp.set_name( "response" );
    resp.push_back( JSONNode( "name", "ATMarketMoversDbResponseType" ) );
    resp.push_back( JSONNode( "enum", responseType ) );
    resp.push_back( JSONNode( "type", strResponseType ) );

    JSONNode data( JSON_NODE );
    data.set_name( "data" );
    data.push_back( jsonifyAtMarketMoversDb( pResponse ) );

    resp.push_back( data );

    n.push_back( resp );
    m_pInboundMsgs->push( n );
}

JSONNode Requestor::jsonifyAtMarketMoversDb( LPATMARKET_MOVERSDB_RESPONSE pResponse ) {

    ATMarketMoversDbResponseParser parser(pResponse);

    JSONNode n( JSON_NODE );
    n.set_name( "atMarketMoversDb" );

    if( parser.MoveToFirstRecord() == true ) {
        JSONNode c( JSON_ARRAY );
        c.set_name( "records" );
        while( true ) {
            JSONNode rec( JSON_NODE );
            rec.push_back( JSONNode( "recordSymbol", parser.GetRecordSymbol()->symbol ) );

            if(parser.MoveToFirstItem() == true) {
                JSONNode items( JSON_ARRAY );
                items.set_name( "items" );
                while( true ) {
                    JSONNode item( JSON_NODE );
                    item.push_back( JSONNode( "symbol", parser.GetItemSymbol()->symbol ) );

                    JSONNode lastPrice( JSON_NODE );
                    lastPrice.set_name( "itemLastPrice" );
                    lastPrice.push_back( JSONNode( "precision", parser.GetItemSymbol()->symbol ) );
                    lastPrice.push_back( JSONNode( "price", parser.GetItemLastPrice().price ) );
                    item.push_back( lastPrice );

                    item.push_back( JSONNode( "itemVolume", parser.GetItemVolume() ) );
                    items.push_back( item );
                    if(parser.MoveToNextItem() == false)
                        break;
                }
                rec.push_back( items );

                c.push_back( rec );
                if(parser.MoveToNextRecord() == false)
                    break;
            }
        }
    }
    return n;
}

/*virtual*/ void Requestor::OnATQuoteDbResponse(
                                    uint64_t origRequest,
                                    ATQuoteDbResponseType responseType,
                                    LPATQUOTEDB_RESPONSE pResponse,
                                    uint32_t responseCount ) {
}

/*virtual*/ void Requestor::OnATMarketMoversStreamResponse(
                                uint64_t origRequest,
                                ATStreamResponseType responseType,
                                LPATMARKET_MOVERS_STREAM_RESPONSE pResponse ) {
}

/*virtual*/ void Requestor::OnATQuoteStreamResponse(
                                    uint64_t origRequest,
                                    ATStreamResponseType responseType,
                                    LPATQUOTESTREAM_RESPONSE pResponse,
                                    uint32_t responseCount ) {
}

/*virtual*/ void Requestor::OnATRequestTimeout( uint64_t origRequest ) {
}
