int main() {
    auto start_time = chrono::high_resolution_clock::now();

    vector<int> ex_truck_verify = {10, 20, 20, 25};

    // Load JSON
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
        auto best_combo = best_listing_combo(locs, ex_truck_verify);
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

    for (auto& res : api_response) {
        cout << "{ location_id: " << res.location_id
             << ", total_price_in_cents: " << res.total_price
             << ", listing_ids: [";
        for (size_t i = 0; i < res.listing_ids.size(); i++) {
            cout << res.listing_ids[i];
            if (i + 1 < res.listing_ids.size()) cout << ", ";
        }
        cout << "] }\n";
    }

    auto end_time = chrono::high_resolution_clock::now();
    double elapsed = chrono::duration<double>(end_time - start_time).count();
    cout << "Process took " << fixed << setprecision(4) << elapsed << " seconds\n";

    return 0;
}