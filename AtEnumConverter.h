#pragma once
#include <string>
#include <map>
#include "./import/include/Shared/ATServerAPIDefines.h"

class AtEnumConverter {

public:
    AtEnumConverter();
    ~AtEnumConverter();

    ATExchangeType toAtExchange( std::string exchangeType );
    ATCountryType toAtCountry( std::string countryType );
    ATStreamRequestType toAtStreamRequest( std::string requestType );
    ATQuoteFieldType toAtQuoteField( std::string quoteFieldType );

private:
    std::map<std::string, ATExchangeType> m_strToExchange;
    std::map<std::string, ATCountryType> m_strToCountry;
    std::map<std::string, ATStreamRequestType> m_strToStreamRequest;
    std::map<std::string, ATQuoteFieldType> m_strToQuoteField;

};