import globals
from tests import *

from modules.utils import *
from modules.PaperExtractor import ExtractPaper
from modules.car import find_car

# track = cv2.imread('./assets/track/0.png')
# show_images([track],["Track 0"])

# basma = cv2.imread('./basma.jpg')
# ExtractPaper(basma)

globals.debug_code(True)
globals.test_code(False)


if (globals.test):
    test_color_mask()

# Read Track Image
# track_img = cv2.imread('./assets/track/4.jpg')

# Extract Track Paper From the Image
# paper_img,wrapped=ExtractPaper(track_img)

# if(not wrapped):
#    print("Failed to Extract Paper 😢 ")
#    sys.exit(-1)
###############################################################################################
# test_car_color('./assets/ontrack/2.jpeg', [255, 0, 0])
# test_car_color('./assets/ontrack/2.jpeg', [0, 0, 225])


# # Read an Car on Track Image
# image = cv2.imread('./assets/ontrack/1.png')

# # Find Car on Track
# x_car,y_car,car_found=find_car(image,front_color=[185,68,74],back_color=[76,116,142])
# if( not car_found):
#    print("Failed to Find Car 😯")
#    sys.exit(-1)
