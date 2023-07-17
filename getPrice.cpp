#include <curl/curl.h>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

std::string currencyPair;

// Alias for convenience
using json = nlohmann::json;

// Callback function to write the response data
size_t WriteCallback(char* data, size_t size, size_t nmemb, std::string* response)
{
    size_t totalSize = size * nmemb;
    response->append(data, totalSize);
    return totalSize;
}

float getPriceForCurrencyPair(const std::string& currencyPair)
{
    std::string price;
    CURL* connection;
    CURLcode code;

    connection = curl_easy_init();
    if (connection)
    {
        std::string response;

        // Set the URL
        std::string url = "https://api-fxpractice.oanda.com/v3/accounts/101-001-26231816-001/pricing?instruments=" + currencyPair;
        code = curl_easy_setopt(connection, CURLOPT_URL, url.c_str());
        if (code != CURLE_OK)
        {
            std::cout << "Failed to set URL" << std::endl;
            return 0.0;
        }

        struct curl_slist* headers = NULL;

        // Set the headers
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Authorization: Bearer f18a87ad84976c1010503042a9621689-8ff03716635ad7204a2c83b52cea3d36");

        code = curl_easy_setopt(connection, CURLOPT_HTTPHEADER, headers);
        if (code != CURLE_OK)
        {
            std::cout << "Failed to set headers" << std::endl;
            return 0.0;
        }

        // Set the response callback function
        code = curl_easy_setopt(connection, CURLOPT_WRITEFUNCTION, WriteCallback);
        if (code != CURLE_OK)
        {
            std::cout << "Failed to set write callback function" << std::endl;
            return 0.0;
        }

        // Set the response data pointer
        code = curl_easy_setopt(connection, CURLOPT_WRITEDATA, &response);
        if (code != CURLE_OK)
        {
            std::cout << "Failed to set write data" << std::endl;
            return 0.0;
        }

        // Perform the request
        code = curl_easy_perform(connection);
        if (code != CURLE_OK)
        {
            std::cout << "Failed to perform connection" << std::endl;
            return 0.0;
        }

        // Parse the JSON response
        json jsonResponse = json::parse(response);

        // Output the price of the instrument
        for (const auto& instrument : jsonResponse["prices"])
        {
            price = instrument["asks"][0]["price"];
            return std::stof(price);
        }

        curl_easy_cleanup(connection);
        curl_slist_free_all(headers);
    }
    return 0.0;
}



//g++ main.cpp -o output -I. -lcurl -std=c++11