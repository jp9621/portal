#include <iostream>
#include <vector>
#include <string>
//tracker
float getPriceForCurrencyPair(const std::string& currencyPair);

std::vector<bool> track(const std::string& currencyName, const std::vector<float>& targets, const float stoploss)
{
    // Get the price for the currency pair using getPriceForCurrencyPair() function
    float price = getPriceForCurrencyPair(currencyName);

    std::vector<bool> targetsMet(targets.size(), false);

    // Check if targets are met
    for (size_t i = 0; i < targets.size(); i++)
    {
        if ((price >= targets[i] && stoploss < price) || (price <= targets[i] && stoploss > price))
        {
            targetsMet[i] = true;
            std::cout << "Target " << targets[i] << "met";
        }
    }



    // Return the bool list of targets met
    return targetsMet;
}
