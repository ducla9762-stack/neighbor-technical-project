#include <bits/stdc++.h>
#include <json/json.h>
#include <fstream>
#include <chrono>
#include <iostream>
using namespace std;

struct Listing {
    string id;
    int length;
    int width;
    int price;
};

ostream& operator<<(ostream& os, const Listing& l) {
    os << "{id: " << l.id
       << ", length: " << l.length
       << ", width: " << l.width
       << ", price: " << l.price
       << "}";
    return os;
}

bool can_fit_backtrack(const vector<int>& trucks, const vector<Listing>& combo) {
    vector<int> slots;
    for (const auto& l : combo) {
        int num_slots = l.width / 10;
        for (int i = 0; i < num_slots; i++) slots.push_back(l.length);
    }

    function<bool(int, vector<int>&)> backtrack = [&](int idx, vector<int>& current) -> bool {
        if (idx == trucks.size()) return true;
        for (int i = 0; i < current.size(); i++) {
            if (trucks[idx] <= current[i]) {
                vector<int> next = current;
                next[i] -= trucks[idx];
                if (backtrack(idx + 1, next)) return true;
            }
        }
        return false;
    };

    return backtrack(0, slots);
}

vector<Listing> best_listing_combo(vector<Listing>& locations, const vector<int>& trucks) {
    sort(locations.begin(), locations.end(),
         [](const Listing& a, const Listing& b) { return a.price < b.price; });

    int n = locations.size();
    vector<Listing> best_combo;
    int best_price = INT_MAX;

    for (int r = 1; r <= n; r++) {
        vector<int> idx(r);
        iota(idx.begin(), idx.end(), 0);

        while (true) {
            vector<Listing> combo;
            for (int i : idx) combo.push_back(locations[i]);

            int total_price = 0;
            for (auto& l : combo) total_price += l.price;

            if (total_price < best_price && can_fit_backtrack(trucks, combo)) {
                best_combo = combo;
                best_price = total_price;
            }

            int i;
            for (i = r - 1; i >= 0; i--) {
                if (idx[i] != i + n - r) break;
            }
            if (i < 0) break;
            idx[i]++;
            for (int j = i + 1; j < r; j++) idx[j] = idx[j - 1] + 1;
        }
    }

    return best_combo;
}

// int main() {
//     auto start_time = chrono::high_resolution_clock::now();

//     vector<int> ex_truck_verify = {10, 20, 20, 25};

//     for (auto i : ex_truck_verify){
//         cout << i << endl; 
//     }

//     // Load JSON
//     ifstream f("listings.json");
//     Json::CharReaderBuilder builder;
//     Json::Value listings_json;
//     string errs;
//     if (!Json::parseFromStream(builder, f, &listings_json, &errs)) {
//         cerr << "Failed to parse JSON: " << errs << endl;
//         return 1;
//     }

//     unordered_map<string, vector<Listing>> locations;
//     for (const auto& l : listings_json) {
//         Listing info;
//         info.id = l["id"].asString();
//         info.length = l["length"].asInt();
//         info.width = l["width"].asInt();
//         info.price = l["price_in_cents"].asInt();
//         locations[l["location_id"].asString()].push_back(info);
//     }

//     struct ApiResponse {
//         string location_id;
//         vector<string> listing_ids;
//         int total_price;
//     };

//     vector<ApiResponse> api_response;

//     for (auto& [location_id, locs] : locations) {
//         auto best_combo = best_listing_combo(locs, ex_truck_verify);
//         if (!best_combo.empty()) {
//             vector<string> best_ids;
//             int total_price = 0;
//             for (auto& l : best_combo) {
//                 best_ids.push_back(l.id);
//                 total_price += l.price;
//             }
//             api_response.push_back({location_id, best_ids, total_price});
//         }
//     }

//     sort(api_response.begin(), api_response.end(),
//          [](const ApiResponse& a, const ApiResponse& b) { return a.total_price < b.total_price; });

//     // for (auto& res : api_response) {
//     //     cout << "{ location_id: " << res.location_id
//     //          << ", total_price_in_cents: " << res.total_price
//     //          << ", listing_ids: [";
//     //     for (size_t i = 0; i < res.listing_ids.size(); i++) {
//     //         cout << res.listing_ids[i];
//     //         if (i + 1 < res.listing_ids.size()) cout << ", ";
//     //     }
//     //     cout << "] }\n";
//     // }

//     auto end_time = chrono::high_resolution_clock::now();
//     double elapsed = chrono::duration<double>(end_time - start_time).count();
//     cout << "Process took " << fixed << setprecision(4) << elapsed << " seconds\n";

//     return 0;
// }

int main(){
    stringstream buffer; 
    buffer << cin.rdbuf(); 
    string input_json = buffer.str(); 
    
    Json::Value vehicles_json; 
    Json::CharReaderBuilder readerBuilder; 
    string errs1; 

    bool parsingSuccessful = Json::parseFromStream( readerBuilder, buffer, &vehicles_json, &errs1);
    if (!parsingSuccessful) {
        cerr << "Failed to parse JSON: " << errs1 << endl;
        return 1;
    }

    vector<int> request_trucks = {}; 
    for (const auto& v : vehicles_json){
        for (int i = 0; i < v["quantity"].asInt(); i++ ){
            request_trucks.push_back( v["length"].asInt() );
        }
    }

    // for (auto i : request_trucks){
    //     cout << i << endl; 
    // }

    // Load listings.json
    ifstream f("listings.json");
    Json::CharReaderBuilder builder;
    Json::Value listings_json;
    string errs;
    if (!Json::parseFromStream(builder, f, &listings_json, &errs)) {
        cerr << "Failed to parse JSON: " << errs << endl;
        return 1;
    }

    unordered_map<string, vector<Listing>> locations;
    for (const auto& l : listings_json) {
        Listing info;
        info.id = l["id"].asString();
        info.length = l["length"].asInt();
        info.width = l["width"].asInt();
        info.price = l["price_in_cents"].asInt();
        locations[l["location_id"].asString()].push_back(info);
    }

    struct ApiResponse {
        string location_id;
        vector<string> listing_ids;
        int total_price;
    };

    vector<ApiResponse> api_response;

    for (auto& [location_id, locs] : locations) {
        auto best_combo = best_listing_combo(locs, request_trucks);
        if (!best_combo.empty()) {
            vector<string> best_ids;
            int total_price = 0;
            for (auto& l : best_combo) {
                best_ids.push_back(l.id);
                total_price += l.price;
            }
            api_response.push_back({location_id, best_ids, total_price});
        }
    }

    sort(api_response.begin(), api_response.end(),
         [](const ApiResponse& a, const ApiResponse& b) { return a.total_price < b.total_price; });
    Json::Value response(Json::arrayValue);

    for (auto& res : api_response){
        Json::Value item; 
        item["locaion_id"] = res.location_id; 
        item["total_price_in_cents"] = res.total_price;
        
        Json::Value listing_ids(Json::arrayValue); 

        for (auto& id : res.listing_ids){
            listing_ids.append(id);
        }

        item["listing_ids"] = listing_ids;
        response.append(item); 
    }

    Json::StreamWriterBuilder writer;
    writer["indentation"] = ""; 
    cout << Json::writeString(writer, response) << endl;

    //Old print
    // for (auto& res : api_response) { cout << "{ location_id: " << res.location_id << ", total_price_in_cents: " << res.total_price << ", listing_ids: ["; for (size_t i = 0; i < res.listing_ids.size(); i++) { cout << res.listing_ids[i]; if (i + 1 < res.listing_ids.size()) cout << ", "; } cout << "] }\n"; }

    return 0;
}



