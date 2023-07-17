//g++ main.cpp getPrice.cpp track.cpp -o output -I. -lcurl -pthread


#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <nlohmann/json.hpp>
#include <algorithm>
#include <cmath>
#include <cstdio>

float getPriceForCurrencyPair(const std::string& currencyPair);
std::vector <bool> track(const std::string& currencyName, const std::vector<float>& targets, const float stoploss, const float currentprice);
void csv(std::string currencyPair, std::string session, std::string biastf, std::string entrytf, std::string conftf, std::string conftype, float maxrr, std::string setuptype, std::string be, std::string TP1, std::string TP2, std::string rs, std::string comments);

int main()
{
    std::remove("log.csv");
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
        std::cout << "+TP? [y/n]: ";
        std::cin >> newTP;
        if (newTP == 'y')
        {
            std::cout << "TP" << targets.size() + 1 << ": ";
            float newTarget;
            std::cin >> newTarget;
            targets.push_back(newTarget);
        }
    }

    std::vector<float> rrlist; 
    std::vector<bool> targetsMet;

    while (true)
    {
        rrlist.push_back(getPriceForCurrencyPair(currencyPair));
        targetsMet = track(currencyPair, targets, stoploss, currentprice);
        std::this_thread::sleep_for(std::chrono::seconds(3));
        if ((targetsMet[targets.size() - 1]) || (getPriceForCurrencyPair(currencyPair) < stoploss < currentprice) || (getPriceForCurrencyPair(currencyPair) > stoploss > currentprice)) {
            break;
        }
    }

    std::string log = "n";
    std::cout << "Log trade? [y/n]: ";
    std::cin >> log;
    if (log == "y") {
        float maxprice;
        std::string session;
        std::string biastf;
        std::string entrytf;
        std::string conftf;
        std::string conftype;
        float maxrr;
        std::string setuptype;
        std::string be = "";
        std::string tp1 = "";
        std::string tp2 = "";
        std::string rs = "";
        std::string comments;

        std::cout << "Session: ";
        std::cin >> session;

        std::cout << "Bias TF: ";
        std::cin >> biastf;

        std::cout << "Entry TF: ";
        std::cin >> entrytf;

        std::cout << "Confirmation TF: ";
        std::cin >> conftf;

        std::cout << "Confirmation Type: ";
        std::cin >> conftype;

        std::cout << "Setup Type: ";
        std::cin >> setuptype;

        std::cout << "Comments: ";
        std::cin >> comments;

        if (stoploss < currentprice) {
            auto maxelement = std::max_element(rrlist.begin(), rrlist.end());
            maxprice = *maxelement;
        }
        else if (stoploss > currentprice) {
            auto minelement = std::min_element(rrlist.begin(), rrlist.end());
            maxprice = *minelement;
        }

        maxrr = std::abs((maxprice - currentprice) / (currentprice - stoploss));
            int i = 0;
        if (targetsMet[3]) {
            rs = "W";
        }
        else {
            rs = "L";
        }
        if (targetsMet[2]) {
            tp2 = "W";
        }
        else {
            tp2 = "L";
        }
        if (targetsMet[1]) {
            tp1 = "W";
        }
        else {
            tp1 = "L";
        }
        if (targetsMet[0]) {
            be = "W";
        }
        else {
            be = "L";
        }
        
        csv(currencyPair, session, biastf, entrytf, conftf, conftype, maxrr, setuptype, be, tp1, tp2, rs, comments);
    }


    return 0;
}
