import globals

from modules.utils import *
from modules.PaperExtractor import ExtractPaper
# track = cv2.imread('./assets/track/0.png')
# show_images([track],["Track 0"])

# basma = cv2.imread('./basma.jpg')
# ExtractPaper(basma)

globals.debug_code(True)

# Read Track Image
track_img = cv2.imread('./assets/track/4.jpg')

#Extract Track Paper From the Image
paper_img,wrapped=ExtractPaper(track_img)

if(wrapped):
   print("Failed to Extract Paper 😢 ")
   sys.exit(-1)