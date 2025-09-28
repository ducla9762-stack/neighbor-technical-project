import json 
import pandas as pd
import _2_bin_packing_algo

with open("listings.json", "r") as f:
    listings = json.load(f)

from collections import defaultdict
locations = defaultdict(list)
for l in listings:
    info = ( l['id'], l['length'], l['width'], l['price_in_cents'] )
    locations[l['location_id']].append( info )


#Iterate through locations and give algo a list of listing_ids in the location


# location_id_count = {}

# for listing in listings: 
#     if listing['location_id'] not in location_id_count: 
#         location_id_count[ listing['location_id'] ] = 0
#     location_id_count[ listing['location_id'] ] += 1


# # df_list = []
# # df_list.append( location_id_count)
# df = pd.DataFrame( list(location_id_count.items() ), columns=['location_id', 'count'])
# df_sorted = df.sort_values(by='count', ascending=False)

# df_sorted.to_csv("example.csv", index=True)