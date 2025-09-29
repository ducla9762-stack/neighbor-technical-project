#pragma once
#include <vector>
#include <string>
#include <json/json.h>

struct Listing {
    std::string id;
    int length;
    int width;
    int price;
};

// Load all listings from a file into a Json::Value
Json::Value loadListings(const std::string &filename);

// Run the DP/bin-packing algorithm and return results as Json::Value
Json::Value find_vehicle_storage(const Json::Value &vehicles, const Json::Value &listings);

// #pragma once
// #include <vector>
// #include <string>
// #include <climits>

// struct Listing {
//     std::string id;
//     int length;
//     int width;
//     int price;
// };

// // Checks if trucks can fit into the given listings combo
// bool can_fit_dp(const std::vector<int>& trucks, const std::vector<Listing>& combo);

// // Finds the best combination of listings for given trucks
// std::vector<Listing> best_listing_combo(std::vector<Listing> locations, const std::vector<int>& trucks);