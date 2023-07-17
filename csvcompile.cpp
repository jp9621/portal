#include <iostream>
#include <fstream>
#include <string>


void csv(std::string currencyPair, std::string session, std::string biastf, std::string entrytf, std::string conftf, std::string conftype, float maxrr, std::string setuptype, std::string be, std::string TP1, std::string TP2, std::string rs, std::string comments) {
    std::string csvcontent = "";
    csvcontent += "\"" + currencyPair + "\"," +
        "\"" + session + "\"," +
        "\"" + biastf + "\"," +
        "\"" + entrytf + "\"," +
        "\"" + conftf + "\"," +
        "\"" + conftype + "\"," +
        std::to_string(maxrr) + "," +
        "\"" + setuptype + "\"," +
        "\"" + be + "\"," +
        "\"" + TP1 + "\"," +
        "\"" + TP2 + "\"," +
        "\"" + rs + "\"," +
        "\"" + comments + "\"\n";

        std::ofstream outputFile("log.csv", std::ios::app); // Open the file in append mode
        if (outputFile.is_open()) {
            outputFile << csvcontent;
            outputFile.close();
            std::cout << "CSV data appended successfully." << std::endl;
        }
        else {
            std::cerr << "Unable to open the CSV file." << std::endl;
        }
}

