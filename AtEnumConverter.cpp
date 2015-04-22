#include "AtEnumConverter.h"
#include <utility>

AtEnumConverter::AtEnumConverter() {
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeAMEX",ExchangeAMEX) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeNasdaqOmxBx",ExchangeNasdaqOmxBx) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeNationalStockExchange",ExchangeNationalStockExchange) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeFinraAdf",ExchangeFinraAdf) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeCQS",ExchangeCQS) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeForex",ExchangeForex) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeInternationalSecuritiesExchange",ExchangeInternationalSecuritiesExchange) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeEdgaExchange",ExchangeEdgaExchange) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeEdgxExchange",ExchangeEdgxExchange) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeChicagoStockExchange",ExchangeChicagoStockExchange) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeNyseEuronext",ExchangeNyseEuronext) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeNyseArcaExchange",ExchangeNyseArcaExchange) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeNasdaqOmx",ExchangeNasdaqOmx) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeCTS",ExchangeCTS) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeCTANasdaqOMX",ExchangeCTANasdaqOMX) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeOTCBB",ExchangeOTCBB) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeNNOTC",ExchangeNNOTC) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeChicagoBoardOptionsExchange",ExchangeChicagoBoardOptionsExchange) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeNasdaqOmxPhlx",ExchangeNasdaqOmxPhlx) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeBatsYExchange",ExchangeBatsYExchange) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeBatsExchange",ExchangeBatsExchange) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeCanadaToronto",ExchangeCanadaToronto) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeCanadaVenture",ExchangeCanadaVenture) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeOpra",ExchangeOpra) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeOptionBoston",ExchangeOptionBoston) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeOptionCboe",ExchangeOptionCboe) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeOptionNyseArca",ExchangeOptionNyseArca) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeOptionC2",ExchangeOptionC2) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeOptionNasdaqOmxBx",ExchangeOptionNasdaqOmxBx) );
    this->m_strToExchange.insert( std::pair<std::string,ATExchangeType>("ExchangeComposite",ExchangeComposite) );

    this->m_strToCountry.insert( std::pair<std::string,ATCountryType>("CountryInternational",CountryInternational) );
    this->m_strToCountry.insert( std::pair<std::string,ATCountryType>("CountryUnitedStates",CountryUnitedStates) );
    this->m_strToCountry.insert( std::pair<std::string,ATCountryType>("CountryCanada",CountryCanada) );
}

AtEnumConverter::~AtEnumConverter() {
}

ATExchangeType AtEnumConverter::toAtExchange( std::string exchangeType ) {
    ATExchangeType retVal;
    retVal = this->m_strToExchange[ exchangeType ]; // TODO catch for unknown type
    return retVal;
}

ATCountryType AtEnumConverter::toAtCountry( std::string countryType ) {
    ATCountryType retVal;
    retVal = this->m_strToCountry[ countryType ]; // TODO catch for unknown type
    return retVal;
}