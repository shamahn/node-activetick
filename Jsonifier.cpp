#include "Jsonifier.h"
#include "AtEnumConverter.h"
#include <string.h>
#include <utility>
#include "import/libjson/libjson.h"
#include "import/example/Helper.h"

std::map<uint8_t, std::string> createSymbolTypeMap()
{
    std::map<uint8_t, std::string> symbolTypeToStr;
    symbolTypeToStr.insert( std::pair<uint8_t, std::string>(SymbolStock, "SymbolStock") );
    symbolTypeToStr.insert( std::pair<uint8_t, std::string>(SymbolIndex, "SymbolIndex") );
    symbolTypeToStr.insert( std::pair<uint8_t, std::string>(SymbolStockOption, "SymbolStockOption") );
    symbolTypeToStr.insert( std::pair<uint8_t, std::string>(SymbolBond, "SymbolBond") );
    symbolTypeToStr.insert( std::pair<uint8_t, std::string>(SymbolMutualFund, "SymbolMutualFund") );
    symbolTypeToStr.insert( std::pair<uint8_t, std::string>(SymbolTopMarketMovers, "SymbolTopMarketMovers") );
    symbolTypeToStr.insert( std::pair<uint8_t, std::string>(SymbolCurrency, "SymbolCurrency") );
    return symbolTypeToStr;
}

std::map<uint8_t, std::string> symbolTypeToStr = createSymbolTypeMap();

std::map<uint8_t, std::string> createExchangeTypeMap()
{
    std::map<uint8_t, std::string> exchangeTypeToStr;
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeAMEX, "ExchangeAMEX") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeNasdaqOmxBx, "ExchangeNasdaqOmxBx") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeNationalStockExchange, "ExchangeNationalStockExchange") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeFinraAdf, "ExchangeFinraAdf") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeCQS, "ExchangeCQS") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeForex, "ExchangeForex") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeInternationalSecuritiesExchange, "ExchangeInternationalSecuritiesExchange") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeEdgaExchange, "ExchangeEdgaExchange") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeEdgxExchange, "ExchangeEdgxExchange") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeChicagoStockExchange, "ExchangeChicagoStockExchange") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeNyseEuronext, "ExchangeNyseEuronext") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeNyseArcaExchange, "ExchangeNyseArcaExchange") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeNasdaqOmx, "ExchangeNasdaqOmx") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeCTS, "ExchangeCTS") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeCTANasdaqOMX, "ExchangeCTANasdaqOMX") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeOTCBB, "ExchangeOTCBB") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeNNOTC, "ExchangeNNOTC") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeChicagoBoardOptionsExchange, "ExchangeChicagoBoardOptionsExchange") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeNasdaqOmxPhlx, "ExchangeNasdaqOmxPhlx") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeBatsYExchange, "ExchangeBatsYExchange") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeBatsExchange, "ExchangeBatsExchange") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeCanadaToronto, "ExchangeCanadaToronto") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeCanadaVenture, "ExchangeCanadaVenture") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeOpra, "ExchangeOpra") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeOptionBoston, "ExchangeOptionBoston") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeOptionCboe, "ExchangeOptionCboe") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeOptionNyseArca, "ExchangeOptionNyseArca") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeOptionC2, "ExchangeOptionC2") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeOptionNasdaqOmxBx, "ExchangeOptionNasdaqOmxBx") );
    exchangeTypeToStr.insert( std::pair<uint8_t, std::string>(ExchangeComposite, "ExchangeComposite") );
    return exchangeTypeToStr;
}

std::map<uint8_t, std::string> exchangeTypeToStr = createExchangeTypeMap();

std::map<uint8_t, std::string> createCountryTypeMap()
{
    std::map<uint8_t, std::string> countryTypeToStr;
    countryTypeToStr.insert( std::pair<uint8_t, std::string>(CountryInternational, "CountryInternational") );
    countryTypeToStr.insert( std::pair<uint8_t, std::string>(CountryUnitedStates, "CountryUnitedStates") );
    countryTypeToStr.insert( std::pair<uint8_t, std::string>(CountryCanada, "CountryCanada") );
    return countryTypeToStr;
}

std::map<uint8_t, std::string> countryTypeToStr = createCountryTypeMap();

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
    s.push_back( JSONNode( "symbolType", symbolTypeToStr[pSymbol->symbolType] ) );
    s.push_back( JSONNode( "exchangeType", exchangeTypeToStr[pSymbol->exchangeType] ) );
    s.push_back( JSONNode( "countryType", countryTypeToStr[pSymbol->countryType] ) );

    return s;
}

/*static*/ std::string Jsonifier::getSymbolType( uint8_t symbolType ) {
    return symbolTypeToStr[symbolType];
}
/*static*/ std::string Jsonifier::getExchangeType( uint8_t exchangeType ) {
    return exchangeTypeToStr[exchangeType];
}
/*static*/ std::string Jsonifier::getCountryType( uint8_t countryType ) {
    return countryTypeToStr[countryType];
}
