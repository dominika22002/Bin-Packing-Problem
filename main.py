import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
import random
import os

class Item:
    def __init__(self, binNumber: int, number: int, width: int, height: int, x: int, y: int, isRotated: int):
        self.binNumber = binNumber
        self.number = number
        self.width = width
        self.height = height
        self.x = x
        self.y = y
        self.isRotated = isRotated

def getNumberOfBins(items):
    lastBin = 0
    for item in items:
        print(item.binNumber, item.number, item.width, item.height, item.x, item.y, item.isRotated)
        lastBin = max(item.binNumber,lastBin)

    return lastBin

def readFromInputFile():
    items = []

    # Check if file exists, if it is not exist, return
    if (not os.path.isfile("files/output.txt")):
        return

    # Prepare items and add them to the list
    with open("files/output.txt", 'r+') as input_file:
        bin_width, bin_height = list(map(int, input_file.readline().split()))

        for line in input_file:
            binNumber, number, width, height, x, y, isRotated = list(map(int, line.split()))
            items.append(Item(binNumber, number, width, height, x, y, isRotated))

    #return list of items
    return [[bin_width, bin_height], items]


[pallet_dimensions, items] = readFromInputFile()

print(pallet_dimensions)
numberOfBins = getNumberOfBins(items)

print("Try to plot solution!")

plot_patterns = {1:(1,1), 2:(1,2), 3:(1,3), 4:(2,2), 5:(2,3), 6:(2,3), 7:(2,4), 8:(2,4), 9:(3,3)}
plots = plot_patterns[numberOfBins]
fig, ax = plt.subplots(plots[0], plots[1])

fig.suptitle("aaa", fontsize=10)

# Define colors list
colors = plt.get_cmap("Set1").colors

for p in items:
    # Define Matplotlib figure and axis
    ax = plt.subplot(plots[0], plots[1], p.binNumber)
    # Define pallet dimensions
    ax.set_xlim([0, pallet_dimensions[0]])
    ax.set_ylim([0, pallet_dimensions[1]])
    # Turn off autoscale and set equal scale on both axis
    ax.autoscale(enable=False)
    ax.set_aspect('equal', adjustable='box')
    # Create title and labels
    ax.set_title(f"pallet {p.binNumber}")
    ax.set_xlabel(f"{pallet_dimensions[0]}")
    ax.set_ylabel(f"{pallet_dimensions[1]}")

    # Set package dimensions
    package_height, package_width = p.height, p.width

    # Create rectangle representing package
    rectangle = Rectangle((p.x, p.y), package_width, package_height,
                           facecolor = colors[random.randint(0, len(colors)-1)],
                           fill=True,
                           alpha=0.5,
                           )

    # Add border to the rectangle
    border = Rectangle((p.x, p.y), package_width, package_height, fill=None, alpha=0.5)

    # Add rectangle to plot
    ax.add_patch(rectangle)
    ax.add_patch(border)

    # Add package_id to ploted rectangle
    ax.annotate(f"{p.number}", rectangle.get_center(), color="#333333", weight='bold', fontsize=8, ha='center', va='center')

# Delete empty plots if 5 or 7 pallets
if numberOfBins == 5 or numberOfBins == 7:
    ax = plt.subplot(plots[0], plots[1], numberOfBins + 1)
    ax.set_axis_off()

# Display plot
plt.show()



# import consolemenu as menu
# from gui.plot_solved import plot_solved
# from algorithms.greedyalgorithm import greedyAlgorithm
# from algorithms.precisealgorithm import preciseAlgorithm
# from algorithms.differentalgorithm import differentAlgorithm
# import read_write_file.readinputfile as readFileManager

# selectedFilePath = menu.selectFilePath()
# [[bin_width, bin_height], items]= readFileManager.readFromInputFile(
#     selectedFilePath)

# selected_algorithm = menu.selectAlgorithm()
# solved_items = []
# number_of_bins = 0

# if (selected_algorithm == 0):
#     [solved_items, number_of_bins] = greedyAlgorithm(
#         items, bin_width, bin_height)
# elif (selected_algorithm == 1):
#     [solved_items, number_of_bins] = preciseAlgorithm(
#         items, bin_width, bin_height)
# elif (selected_algorithm == 2):
#     [solved_items, number_of_bins] = differentAlgorithm(
#         items, bin_width, bin_height)

# print(solved_items)
# print(number_of_bins)

# print("Try to plot solution!")
# plot_solved(solved_items, [bin_width, bin_height],
#                number_of_bins, selectedFilePath)
