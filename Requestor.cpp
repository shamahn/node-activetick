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
    n.push_back( JSONNode( "messageId", "atBarHistory" ) );
    n.push_back( JSONNode( "origRequest", origRequest ) );
    n.push_back( JSONNode( "responseType", responseType ) );
    n.push_back( jsonifyAtBarHistory( pResponse ) );
    m_pInboundMsgs->push( n );
}

JSONNode Requestor::jsonifyAtBarHistory( LPATBARHISTORY_RESPONSE pResponse ) {
    ATBarHistoryDbResponseParser parser(pResponse);

    JSONNode n( JSON_NODE );
    n.set_name( "atBarHistory" );

    JSONNode s( JSON_NODE );
    s.set_name( "atSymbol" );
    s.push_back( JSONNode( "symbol", parser.GetSymbol()->symbol ) );
    s.push_back( JSONNode( "symbolType", parser.GetSymbol()->symbolType ) );
    s.push_back( JSONNode( "exchangeType", parser.GetSymbol()->exchangeType ) );
    s.push_back( JSONNode( "countryType", parser.GetSymbol()->countryType ) );
    n.push_back( s );
    n.push_back( JSONNode( "recordsCount", parser.GetRecordCount() ) );

    if(parser.MoveToFirstRecord())
    {
        JSONNode c( JSON_ARRAY );
        c.set_name( "records");
        while(true)
        {
            ATTIME recordDateTime = parser.GetDateTime();

            c.push_back( m_jsonifier.jsonifyAtTime( "barTime",
                                                    &recordDateTime ) );
            
            JSONNode open( JSON_NODE );
            open.set_name( "open" );
            open.push_back( JSONNode( "precision",
                                      parser.GetOpen().precision ) );
            open.push_back( JSONNode( "price", parser.GetOpen().price ) );
            c.push_back( open );

            JSONNode high( JSON_NODE );
            high.set_name( "high" );
            high.push_back( JSONNode( "precision",
                                      parser.GetHigh().precision ) );
            high.push_back( JSONNode( "price", parser.GetHigh().price ) );
            c.push_back( high );

            JSONNode low( JSON_NODE );
            low.set_name( "low" );
            low.push_back( JSONNode( "precision", parser.GetLow().precision ) );
            low.push_back( JSONNode( "price", parser.GetLow().price ) );
            c.push_back( low );

            JSONNode close( JSON_NODE );
            close.set_name( "close" );
            close.push_back( JSONNode( "precision",
                                       parser.GetClose().precision ) );
            close.push_back( JSONNode( "price", parser.GetClose().price ) );
            c.push_back( close );

            c.push_back( JSONNode( "volume", parser.GetVolume() ) );

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
    n.push_back( JSONNode( "recordsCount", recordsCount ) );

    JSONNode list( JSON_ARRAY );
    list.set_name( "atSectorList" );
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
    n.push_back( JSONNode( "sumbolsCount", symbolsCount ) );
    
    JSONNode list( JSON_ARRAY );
    list.set_name( "atConstituentList" );
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
}

/*virtual*/ void Requestor::OnATMarketMoversDbResponse(
                                    uint64_t origRequest,
                                    ATMarketMoversDbResponseType responseType,
                                    LPATMARKET_MOVERSDB_RESPONSE pResponse ) {
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
