import time
from elements.bin import Bin
from elements.item import Item
import algorithms.utils as utils
from algorithms.greedyalgorithm import greedyAlgorithm
import random


def print_list(items, name):
    print("----------------------------------------------------------------")
    print("list for:", name)
    for item in items:
        print(item.number, item.x, item.y, item.bin_number)


def check_lowest_value(free_places, item, bin):
    lowest_value = float('inf')
    best_free_edge = [-1, -1]
    for [i, j] in free_places:
        is_Ok = bin.matrix.try_put_item(i, j, item.width, item.height)
        if is_Ok:
            bin.matrix.put_item(i, j, item.width, item.height, item.number)
            free_edges = bin.matrix.calculate_free_edges()

            if lowest_value > free_edges:
                lowest_value = free_edges
                best_free_edge = [i, j]

            bin.matrix.remove_item(item.number)
    return [lowest_value, best_free_edge]


def check_item(item, bin):
    free_places = bin.matrix.find_free_placeholders(item.width, item.height)
    free_places_rot = bin.matrix.find_free_placeholders(
        item.height, item.width)

    if len(free_places) == 0 and len(free_places_rot) == 0:
        return [float('inf'),[-1, -1], False]

    [lowest_value, best_space] = check_lowest_value(free_places, item, bin)
    [lowest_value_rot, best_space_rot] = check_lowest_value(
        free_places_rot, item, bin)

    selected_space = []
    rotated = False
    if lowest_value_rot < lowest_value:
        selected_space = best_space_rot
        rotated = True
    else:
        selected_space = best_space

    lowest_value = min(lowest_value, lowest_value_rot)
    return [lowest_value, selected_space, rotated]

def pack_items(items, bins):
    print("Let's pack!")
    items.sort(key=lambda x: (x.area, x.width, x.height), reverse=True)
    print_list(items, "Items to pack:")
    sorted_items = []
    still_not_every_item_packed = True
    current_bin = 0
    while still_not_every_item_packed:
        number_of_packed_items = 0
        best_items_place = {}

        for item in items:
            if (item.is_packed):
                number_of_packed_items = number_of_packed_items + 1
                continue
            
            print_list(items,"What still to pack?")

            if len(bins) <= current_bin:
                still_not_every_item_packed = False
                break

            best_items_place[item] = check_item(item, bins[current_bin])


        if len(best_items_place) == 0:
            current_bin = current_bin + 1
            break
    
        min_value = float('inf')
        for key, value in best_items_place.items():
            if value[0] < min_value:
                min_value = value[0]
                min_key = key

        change_bin = True
        for key, value in best_items_place.items():
            if not value[0] == float('inf'):
                change_bin = False

        if change_bin:
            current_bin = current_bin + 1
            continue

        print("Let's gooo!", current_bin) 
        item = min_key
        
        [i, j] = best_items_place[item][1]
        rotated = best_items_place[item][2]
        print([i, j])
        if rotated:
            item.rotate()
        bins[current_bin].matrix.put_item(i, j, item.width, item.height, item.number)
        item.x = i
        item.y = j
        item.bin_number = bins[current_bin].number
        item.is_packed = True
        sorted_items.append(item)
        items.remove(item)

        if number_of_packed_items == len(items):
            still_not_every_item_packed = False

    print_list(sorted_items,"items")

    unpacked_items = []
    # i tutaj zaczynamy wymianę elementów
    # usuwamy te, które dotykają postyc przestrzeli i nie tworzą zamkniętego obszaru:
    for bin in bins:
        items_to_remove = bin.matrix.get_items_to_remove()

        for number in range(items_to_remove):
            if item.number == number:
                unpacked_items.append(item)

        print("Aaa", items_to_remove)

    

    return sorted_items

def addItem(item, bin):







    free_places = bin.matrix.find_free_placeholders(item.width, item.height)
    free_places_for_rotation = bin.matrix.find_free_placeholders(
        item.height, item.width)

    if len(free_places) == 0 and len(free_places_for_rotation) == 0:
        return [1000000000000, False]

    lowest_free_edge_value = 1000000000000
    best_free_edge_value = [1000, 1000]
    best_free_edge_rotated = False

    for [i, j] in free_places:
        bin.matrix.put_item(i, j, item.width, item.height, item.number)
        free_edges = bin.matrix.calculate_free_edges()

        if lowest_free_edge_value > free_edges:
            lowest_free_edge_value = free_edges
            best_free_edge_value = [i, j]

        bin.matrix.remove_item(item.number)

    item.rotate()

    for [i, j] in free_places_for_rotation:
        bin.matrix.put_item(i, j, item.width, item.height, item.number)
        free_edges = bin.matrix.calculate_free_edges()

        if lowest_free_edge_value > free_edges:
            lowest_free_edge_value = free_edges
            best_free_edge_value = [i, j]
            best_free_edge_rotated = True

        bin.matrix.remove_item(item.number)

    if not best_free_edge_rotated:
        item.rotate()

    if lowest_free_edge_value == 1000000000000:
        return [1000000000000, False]

    [i, j] = best_free_edge_value

    bin.matrix.put_item(i, j, item.width, item.height, item.number)
    item.x = i
    item.y = j
    item.bin_number = bin.number
    item.is_packed = True

    return [lowest_free_edge_value, True]

# actual_list - aktualna lista zapisanych itemów wraz z położeniem i rotacją
# bim - pojemnik do którego wsadzamy rzeczy
# current_item - aktualnie układamy
# items_to_pack - co zostało do zapakowania
# best_solve - najlepsze możliwe aktualne zapakowanie
# memo - zapisane wyniki
# warunek stopu, jesli wszystkie pojemniki zostaną ułożone czyli items_to_pack jest pusta


# , memo):
def pack_item(actual_list, bin, item, items_to_pack, best_solve, depth):
    if len(items_to_pack) == 0:
        print(best_solve)
        return int(best_solve)
    item = items_to_pack[0]

    [lowest_free_edge_value, item_has_been_packed] = addItem(item, bin)
    if not item_has_been_packed:
        return int(lowest_free_edge_value)

    if lowest_free_edge_value < best_solve:
        best_solve = lowest_free_edge_value

    print(bin.matrix)
    actual_list.append(item)
    items_to_pack.remove(item)
    # print("KOT", len(items_to_pack))
    if len(items_to_pack) == 0:
        print(best_solve)
        return int(best_solve)

    for itemm in items_to_pack:
        new_best_solve = pack_item(
            actual_list, bin, itemm, items_to_pack, best_solve, depth+1)  # , memo)
        if len(items_to_pack) == 0:
            return new_best_solve

    # Prepare before state
    bin.matrix.remove_item(item.number)
    actual_list.remove(item)
    items_to_pack.append(item)
    return best_solve


def get_items_area(items):
    items_area = 0
    for item in items:
        items_area = items_area + item.area
    return items_area


def try_to_pack(unpacked_items, bin, bin_area):

    free_places = bin.matrix.find_free_placeholders(item.width, item.height)
    bin.matrix.calculate_free_edges()

    if len(free_places) == 0:
        return False

    [i, j] = free_places[0]

    bin.matrix.put_item(i, j, item.width, item.height, item.number)

    item.x = i
    item.y = j
    item.bin_number = bin_number
    item.is_packed = True

    return True

    ###
    bin.sort(key=lambda x: (x.area, x.width, x.height), reverse=True)

    still_not_packed = True
    packed = False
    if len(unpacked_items) == 0:
        return True
    item = unpacked_items[0]
    print("Now we have to check item nr: ", item.number)

    while still_not_packed:
        items_area = get_items_area(bin)
        if items_area + item.area < bin_area*0.99:
            bin.append(item)
            unpacked_items.remove(item)
            packed = True
            still_not_packed = False
        else:
            last_item = bin[-1]
            print("Last items id: ", last_item.number)

            if last_item.area >= item.area:
                return False

            last_item.is_packed = False
            unpacked_items.append(last_item)
            bin.remove(last_item)
    return packed


def pack_to_bins(items, bins, bin_area, down_limit):
    still_not_every_item_packed = True

    while still_not_every_item_packed:
        print("Forever")
        if len(items) == 0:
            still_not_every_item_packed = False

        for bin in bins:
            if len(items) == 0:
                still_not_every_item_packed = False
                break

            print("For bin checking", items[0].number)
            item = items[0]
            if get_items_area(bin) + item.area < bin_area:
                bin.append(item)
                items.remove(item)

    print("CHECK POINT")
    a = 0
    for bin in bins:
        name = "List: " + str(a)
        print_list(bin, name)
        a = a+1

    #     unpacked_items.sort(key=lambda x: (x.area, x.width, x.height), reverse=True)

    #     for bin in items_in_bins:
    #         if try_to_pack(unpacked_items, bin, bin_area):
    #             break

    # for item in items:

    # unpacked_items = []

    # for bin in items_in_bins:
    #     bin.sort(key=lambda x: (x.area, x.width, x.height), reverse=True)

    #     for item in bin:
    #         if item.bin_number >= down_limit:
    #             item.is_packed = False
    #             unpacked_items.append(item)
    #             # bin.remove(item)

    # for bin in items_in_bins:
    #     should_be_removed = False
    #     for item in bin:
    #         if item.bin_number >= down_limit:
    #            should_be_removed = True
    #     if should_be_removed:
    #         items_in_bins.remove(bin)

    # for bin in items_in_bins:
    #     for item in bin:
    #         item.x = 0
    #         item.y = 0
    #         item.is_packed = True

    # for item in unpacked_items:
    #     item.x = 0
    #     item.y = 0
    #     item.is_packed = False

    # still_not_every_item_packed = True
    # while still_not_every_item_packed:
    #     number_of_packed_items = 0

    #     if len(unpacked_items) == 0:
    #         still_not_every_item_packed = False

    #     unpacked_items.sort(key=lambda x: (x.area, x.width, x.height), reverse=True)

    #     for bin in items_in_bins:
    #         if try_to_pack(unpacked_items, bin, bin_area):
    #             break

    # for bin in items_in_bins:
    #     for unpacked_item in unpacked_items:
    #         if get_items_area(bin) + unpacked_item.area < bin_area:
    #             bin.append(unpacked_item)
    #             unpacked_items.remove(unpacked_item)

    # # for bin in items_in_bins:
    # #     if len(bin) == 0:
    # #         items_in_bins.remove(bin)

    # #     # if item.bin_number >= down_limit:
    # #     #     unpacked_items.append(item)
    # #     #     bin.remove(item)

    a = 0
    for bin in bins:
        name = "List: " + str(a)
        print_list(bin, name)
        a = a+1

        # if item.bin_number >= down_limit:
        #     unpacked_items.append(item)
        #     bin.remove(item)


def differentAlgorithm(items, bin_width, bin_height):
    items.sort(key=lambda x: (x.area, x.width, x.height), reverse=True)

    [area, down_limit] = utils.get_down_limit(items, bin_width, bin_height)

    bins = []
    beta_bins = []
    for number in range(0, down_limit):
        bin = Bin(bin_width, bin_height, number)
        bins.append(bin)
        beta_bins.append([])

    items.sort(key=lambda x: (x.bin_number), reverse=False)

    current_bin = 0
    # pack_to_bins(items, beta_bins, area, down_limit)

    # for bin in beta_bins:
    #     for item in bin:
    #         item.x = 0
    #         item.y = 0
    #         item.is_packed = False
    #         # bin.matrix.remove_item(item.number)

    # current_bin = 0    
    solved_items_full = []
    solved_items = pack_items(items, bins)
    print(solved_items, "bbbbb")

    # for bin in beta_bins:
    #     solved_items = pack_items(items, bins)
    #     # bin.sort(key=lambda x: (x.area, x.width, x.height), reverse=True)
    #     # print_list(bin, "yass")
    # for items2 in solved_items:
    #     for item in items2:
    #         solved_items_full.append(item)
    #     #     [lowest_free_edge_value, item_has_been_packed] = addItem(
    #     #         item, bins[current_bin])
    #     current_bin = current_bin + 1
    # print_list(items, "aa")
    # return solved_items_full, len(bins)
    # print_list(solved_items_full, "aaaaaaa")

    return [solved_items, down_limit]

    still_not_every_item_packed = True
    while still_not_every_item_packed:
        number_of_packed_items = 0

        for item in items:
            if (item.is_packed):
                number_of_packed_items = number_of_packed_items + 1
                continue

            for i in range(0, len(bins)):
                [lowest_free_edge_value, item_has_been_packed] = addItem(
                    item, bins[i])
                if item_has_been_packed:
                    number_of_packed_items = number_of_packed_items + 1
                    break

        if number_of_packed_items == len(items):
            still_not_every_item_packed = False
        else:
            bin = Bin(bin_width, bin_height, down_limit+1)
            bins.append(bin)

    # still_not_every_item_packed = True

    # while still_not_every_item_packed:
    # if len(items) == 0:
    #     still_not_every_item_packed = False

    #     for item in items:
    #         if (item.is_packed):
    #             continue

    #         for bin in range bins:
    #             if try_to_pack(item, bin):
    #                 break

    # # for bin in bins:
    # #     if len(items) == 0:
    # #         still_not_every_item_packed = False
    # #         break

    # #     print("For bin checking", items[0].number)
    # #     item = items[0]
    # #     if get_items_area(bin) + item.area < bin_area:
    # #         bin.append(item)
    # #         items.remove(item)

    # # print("CHECK POINT")
    # # a = 0
    # # for bin in bins:
    # #     name = "List: "+ str(a)
    # #     print_list(bin, name)
    # #     a = a+1

    # # while still_not_every_item_packed:
    # #     number_of_packed_items = 0

    # #     for item in items:
    # #         if (item.is_packed):
    # #             number_of_packed_items = number_of_packed_items + 1
    # #             continue

    # #         for i in range(0, len(bins)):
    # #             if try_to_pack(item, bins[i], i):
    # #                 number_of_packed_items = number_of_packed_items + 1
    # #                 break

    # #     if number_of_packed_items == len(items):
    # #         still_not_every_item_packed = False
    # #     else:
    # #         bin = Bin(bin_width, bin_height, down_limit+1)
    # #         bins.append(bin)

    # # for item in items:
    # #     print(item.width, item.height, item.x, item.y, item.bin_number)

    # # return items, len(bins)
    # # current_bin = 0
    # # pack_to_bins(items, beta_bins, area, down_limit)

    # actual_list_full = []
    # for items_for_selected_bin in beta_bins:
    #     actual_list = []
    #     bin = bins[current_bin]
    #     items_to_pack = items_for_selected_bin
    #     item = items_to_pack[0]
    #     best_solve = 1000000000000
    #     depth = 0
    #     pack_item(actual_list, bin, item, items_to_pack, best_solve, depth)
    # for bin in bins:
    #     for item in bin:
    #         actual_list_full.append(item)
    # #     current_bin = current_bin + 1

    # # for item in items:
    # #     if not item.is_packed:
    # #         print("ITEM :", item.number, "NOT PACKED!!")

    # return actual_list_full, len(bins)
