#include "StdAfx.h"
#include "Streamer.h"
#include <string>
#include <stdio.h>

#include "import/example/Helper.h"

using namespace std;

Streamer::Streamer( const APISession& session, 
                    std::queue< JSONNode > *pInboundMsgs)
: ActiveTickStreamListener(session.GetSessionHandle())
, Requestor(session, pInboundMsgs)
, m_session(session)
{
}

Streamer::~Streamer(void)
{
}

/*virtual*/ void Streamer::OnATStreamTradeUpdate(
                                    LPATQUOTESTREAM_TRADE_UPDATE pUpdate ) {   
}

/*virtual*/ void Streamer::OnATStreamQuoteUpdate(
                                    LPATQUOTESTREAM_QUOTE_UPDATE pUpdate ) {
}

/*virtual*/ void Streamer::OnATStreamTopMarketMoversUpdate(
                                    LPATMARKET_MOVERS_STREAM_UPDATE pUpdate ) {
}