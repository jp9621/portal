//g++ main.cpp getPrice.cpp track.cpp -o output -I. -lcurl -pthread


#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <nlohmann/json.hpp>

float getPriceForCurrencyPair(const std::string& currencyPair);
std::vector <bool> track(const std::string& currencyName, const std::vector<float>& targets, const float stoploss);

int main()
{
    std::string currencyPair;
    std::cout << "Pair: ";
    std::cin >> currencyPair;
    float currentprice = getPriceForCurrencyPair(currencyPair);
    std::cout << "Price for " << currencyPair << ": " << currentprice << std::endl;

    float stoploss = 0.0;
    std::cout << "Stop Loss: ";
    std::cin >> stoploss;

    std::vector<float> targets;
    std::cout << "TP1: ";
    float target1;
    std::cin >> target1;
    targets.push_back(target1);
    char newTP = 'y';
    while (newTP == 'y')
    {
        std::cout << "+TP? [y/n]";
        std::cin >> newTP;
        if (newTP == 'y')
        {
            std::cout << "TP" << targets.size() + 1 << ": ";
            float newTarget;
            std::cin >> newTarget;
            targets.push_back(newTarget);
        }
    }


    while (true)
    {
        track(currencyPair, targets, stoploss);
        std::this_thread::sleep_for(std::chrono::minutes(1));
    }

    return 0;
}
