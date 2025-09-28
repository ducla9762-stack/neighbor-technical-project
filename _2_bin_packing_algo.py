from itertools import combinations, permutations
import copy

#locations: id, length, width, price
test_1_locations = [ ('random_id_0', 10, 20, 100) ] #Fit 1 truck, Fit 2 trucks along the width
test_2_locations = [ ('random_id_0', 10, 20, 100), ('random_id_1', 10, 20, 1000) ] 
test_3_locations = [ ('random_id_0', 20, 10, 100) ] #Fit 2 trucks along the length
test_4_locations = [ ('random_id_0', 40, 10, 100), ('random_id_1', 20, 10, 100) ] #Tricky test that breaks greedy of fitting trucks in order...

#for bigger quantities, just duplicate in the list becuase quantity <= 5
#trucks: length
test_1_trucks_1 = [ 10 ] #Should fit
test_1_trucks_2 = [ 10, 10 ] #Should fit

test_2_trucks_1 = [ 10, 10 ]

test_3_trucks_1 = [10, 10] #Should fit
test_4_trucks_1 = [10, 30, 20] #Should fit


def can_fit_greedy(trucks, location_combo):
    # Generate all slots from location combo
    slots = []
    for _, length, width, _ in location_combo:
        slots.extend([length] * (width // 10))

    slots.sort(reverse=True)
    trucks_sorted = sorted(trucks, reverse=True)

    for truck in trucks_sorted:
        placed = False
        for i in range(len(slots)):
            if truck <= slots[i]:
                slots[i] -= truck
                placed = True
                break
        if not placed:
            return False
    return True


def can_fit_dp(trucks, location_combo):
    # Expand slots
    slots = []
    for _, length, width, _ in location_combo:
        slots.extend([length] * (width // 10))

    n = len(trucks)
    full_mask = (1 << n) - 1
    dp = [False] * (1 << n)
    dp[0] = True  # no trucks placed is always valid

    for mask in range(1 << n):
        if not dp[mask]:
            continue
        # Try to assign next slot
        for slot in slots:
            # Subset-sum: find subset of unused trucks that fit
            remaining = [trucks[i] for i in range(n) if not (mask & (1 << i))]
            m = len(remaining)
            # Try all subsets of remaining trucks (brute force here, but could use knapsack DP)
            for sub in range(1 << m):
                total = sum(remaining[j] for j in range(m) if sub & (1 << j))
                if total <= slot:
                    new_mask = mask
                    for j in range(m):
                        if sub & (1 << j):
                            truck_idx = [i for i in range(n) if not (mask & (1 << i))][j]
                            new_mask |= 1 << truck_idx
                    dp[new_mask] = True

    return dp[full_mask]


def best_listing_combo(locations, trucks):
    """
    Finds cheapest combo of listings that can fit all trucks.
    Implements early pruning: if a combo works, skip supersets.
    """
    # Sort locations by price ascending for early pruning
    locations_sorted = sorted(locations, key=lambda x: x[3])

    n = len(locations_sorted)
    best_combo = None
    best_price = float('inf')

    # Generate combinations incrementally by size
    for r in range(1, n + 1):
        for combo in combinations(locations_sorted, r):
            # Early pruning by total price
            total_price = sum(l[3] for l in combo)
            if total_price >= best_price:
                continue  # Can't beat current best

            if can_fit_dp(trucks, combo):
                best_combo = combo
                best_price = total_price
                # Important: prune supersets, so break here
                break
        # If we found a valid combo of size r, no need to check larger combos
        if best_combo:
            break

    return best_combo

# def best_listing_combo( locations, trucks ):
#     # print(locations)
#     # total_truck_length = sum(trucks)

#     total_locations_combo = []
#     best_location  = None
#     best_price = float( 'inf' )

#     for i in range(1, len(locations) + 1 ): 
#         total_locations_combo.extend( combinations(locations, i) )

#     #locations: id, length, width, price
#     for location_combo in total_locations_combo:
#         # total_slot_length = sum(length * (width // 10) for _, length, width, _ in location_combo) 
#         # if total_slot_length < total_truck_length:
#         #     continue

#         location_combo_price = sum( location[3] for location in location_combo)
#         if can_fit_dp( trucks, location_combo ) and location_combo_price < best_price: #If tie, keep current combo    
#             best_location = location_combo
#             best_price = location_combo_price
    
#     return best_location
