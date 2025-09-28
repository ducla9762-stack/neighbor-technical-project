#include <bits/stdc++.h>
#include "json.hpp"
#include <fstream>
#include <chrono>
using namespace std;
using json = nlohmann::json;

struct Listing {
    string id;
    int length;
    int width;
    int price;
};

// ---------- Helper: generate all subsets ------------
bool can_fit_dp(const vector<int>& trucks, const vector<Listing>& combo) {
    // Expand slots
    vector<int> slots;
    for (auto& l : combo) {
        for (int i = 0; i < l.width / 10; i++) {
            slots.push_back(l.length);
        }
    }

    int n = trucks.size();
    int full_mask = (1 << n) - 1;
    vector<bool> dp(1 << n, false);
    dp[0] = true;

    for (int mask = 0; mask < (1 << n); mask++) {
        if (!dp[mask]) continue;
        for (int slot : slots) {
            // collect remaining trucks
            vector<int> remaining;
            vector<int> rem_idx;
            for (int i = 0; i < n; i++) {
                if (!(mask & (1 << i))) {
                    remaining.push_back(trucks[i]);
                    rem_idx.push_back(i);
                }
            }
            int m = remaining.size();
            for (int sub = 1; sub < (1 << m); sub++) {
                int total = 0;
                for (int j = 0; j < m; j++) {
                    if (sub & (1 << j)) total += remaining[j];
                }
                if (total <= slot) {
                    int new_mask = mask;
                    for (int j = 0; j < m; j++) {
                        if (sub & (1 << j)) {
                            new_mask |= (1 << rem_idx[j]);
                        }
                    }
                    dp[new_mask] = true;
                }
            }
        }
    }
    return dp[full_mask];
}

vector<Listing> best_listing_combo(vector<Listing> locations, const vector<int>& trucks) {
    sort(locations.begin(), locations.end(),
         [](const Listing& a, const Listing& b) { return a.price < b.price; });

    int n = locations.size();
    vector<Listing> best_combo;
    int best_price = INT_MAX;

    // Try combos by size
    for (int r = 1; r <= n; r++) {
        vector<int> idx(r);
        iota(idx.begin(), idx.end(), 0);

        while (true) {
            vector<Listing> combo;
            for (int i : idx) combo.push_back(locations[i]);

            int total_price = 0;
            for (auto& l : combo) total_price += l.price;

            if (total_price < best_price && can_fit_dp(trucks, combo)) {
                best_combo = combo;
                best_price = total_price;
                break; // prune supersets
            }

            // next combination
            int i;
            for (i = r - 1; i >= 0; i--) {
                if (idx[i] != i + n - r) break;
            }
            if (i < 0) break;
            idx[i]++;
            for (int j = i + 1; j < r; j++) {
                idx[j] = idx[j - 1] + 1;
            }
        }
        if (!best_combo.empty()) break;
    }

    return best_combo;
}

int main() {
    auto start_time = chrono::high_resolution_clock::now();

    vector<int> ex_truck = {10};
    vector<int> ex_truck_worst_case = {10, 20, 30, 40, 50};

    // Load JSON
    ifstream f("listings.json");
    json listings_json;
    f >> listings_json;

    unordered_map<string, vector<Listing>> locations;
    for (auto& l : listings_json) {
        Listing info = {
            l["id"].get<string>(),
            l["length"].get<int>(),
            l["width"].get<int>(),
            l["price_in_cents"].get<int>()
        };
        locations[l["location_id"].get<string>()].push_back(info);
    }

    struct ApiResponse {
        string location_id;
        vector<string> listing_ids;
        int total_price;
    };

    vector<ApiResponse> api_response;

    for (auto& [location_id, locs] : locations) {
        auto best_combo = best_listing_combo(locs, ex_truck);
        // auto best_combo = best_listing_combo(locs, ex_truck_worst_case);

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
         [](const ApiResponse& a, const ApiResponse& b) {
             return a.total_price < b.total_price;
         });

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