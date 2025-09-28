from itertools import combinations, permutations

#locations: id, length, width, price
test_1_locations = [ ('random_id_0', 10, 20, 100) ] #Fit 1 truck, Fit 2 trucks along the width
test_2_locations = [ ('random_id_0', 10, 20, 100), ('random_id_1', 10, 20, 1000) ] 
test_3_locations = [ ('random_id_0', 20, 10, 100) ] #Fit 2 trucks along the length
test_4_locations = [ ('random_id_0', 40, 10, 100), ('random_id_1', 20, 10, 100) ] #Trick test that breaks greedy of fitting trucks in order...

#for bigger quantities, just duplicate in the list becuase quantity <= 5
#trucks: length
test_1_trucks_1 = [ 10 ] #Should fit
test_1_trucks_2 = [ 10, 10 ] #Should fit

test_3_trucks_1 = [10, 10] #Should fit
test_4_trucks_1 = [10, 30, 20]

def can_fit( trucks, location_combo ): 
    #Greedily fit the trucks because we will already generate all possible combinations anyways
    #Try to fit trucks from locations with biggest length to smallest
    sorted_locations = sorted(location_combo, key=lambda x : x[1] )
    sorted_trucks = trucks.sort()
    trucks_filled = set()


    for location in sorted_locations: 
        #Also, try to fit multiple trucks per location
        for truck in trucks: 
            if truck not in trucks_filled: 


    #Informal exchange argument for the algorithm is that it doesn't matter


def best_listing_combo( locations, trucks ):
    total_locations_combo = []
    best_location  = None
    best_price = float( 'inf' )

    for i in range(1, len(locations) + 1 ): 
        total_locations_combo.extend( combinations(locations, i) )

    for location_combo in total_locations_combo: 
        if can_fit( trucks, location_combo ): 

         

best_listing_combo(test_2)

# total_combos = []

# for i in range(1, len(test) + 1 ): 
#     combo_list = combinations(test, i) 
#     total_combos.extend(combo_list)

