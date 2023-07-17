#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Google Sheet URL
const std::string GOOGLE_SHEET_URL = "YOUR_GOOGLE_SHEET_URL";

// Google Sheets API key
const std::string API_KEY = "YOUR_API_KEY";

// Callback function to write server response
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response)
{
    size_t total_size = size * nmemb;
    response->append((char*)contents, total_size);
    return total_size;
}

// Function to send data to Google Sheet
void SendDataToGoogleSheet(const std::string& data)
{
    // Create JSON payload
    json payload;
    payload["values"] = { { data } };
    std::string jsonStr = payload.dump();

    // Initialize libcurl
    CURL* curl = curl_easy_init();
    if (curl) {
        // Set POST URL
        std::string url = GOOGLE_SHEET_URL + "?key=" + API_KEY;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set payload data
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonStr.c_str());

        // Set callback function to capture server response
        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        else {
            std::cout << "Data sent successfully!" << std::endl;
            std::cout << "Server response: " << response << std::endl;
        }

        // Cleanup
        curl_easy_cleanup(curl);
    }
}

int main()
{
    std::string data = "Hello, Google Sheet!";

    // Send data to Google Sheet
    SendDataToGoogleSheet(data);

    return 0;
}
