import time
import json 
import pandas as pd
import _2_bin_packing_algo

start_time = time.time()


ex_truck = [10]
ex_truck_verify = [10, 20, 20, 25]
ex_truck_worst_case_1 = [10, 20, 30, 40, 50]
# ex_truck_worst_case = [ 50,50,50,50,50 ]
with open("listings.json", "r") as f:
    listings = json.load(f)

from collections import defaultdict
locations = defaultdict(list)
for l in listings:
    info = ( l['id'], l['length'], l['width'], l['price_in_cents'] )
    locations[l['location_id']].append( info )





#Iterate through locations and give algo a list of listing_ids in the location
api_response = []
for location in locations: 
    # print(location)
    # print( _2_bin_packing_algo.best_listing_combo( locations[location] , ex_truck) )
    # best_combo = None
    # if location == "a849a23c-e3b9-487b-8afe-7963af83bd9e":
        # print(location) 
        # best_combo = _2_bin_packing_algo.best_listing_combo( locations[location], ex_truck_verify)
    best_combo = _2_bin_packing_algo.best_listing_combo( locations[location] , ex_truck_worst_case_1)

    if best_combo: 
        best_listings = [ l[0] for l in best_combo ]
        total_price = sum( l[3] for l in best_combo )

        api_response.append( {
            "location_id" : location, 
            "listing_ids" : best_listings, 
            "total_price_in_cents" : total_price
        })

print(api_response)
api_response.sort( key=lambda x: x['total_price_in_cents'] )

end_time = time.time()
elapsed = end_time - start_time
print(f"Process took {elapsed:.4f} seconds")
