import sys
import globals
from tests import *

from modules.ScanTrack import scan_track, extract_lines
from modules.car import find_car

# track = cv2.imread('./assets/track/0.png')
# show_images([track],["Track 0"])

# basma = cv2.imread('./basma.jpg')
# ExtractPaper(basma)

globals.debug_code(True)
globals.test_code(False)


if (globals.test):
    test_color_mask()


# Scan Track()
# Read Track Image
track_img = cv2.imread('./assets/track/1.png')

# Scanning Track Initially
wrapped,track=scan_track(track_img,thickness=20)

if(not wrapped):
   print("Failed to Extract Paper 😢 ")
   sys.exit(-1)


# while(1):
    # # Find Car on Track
    # Read Image Car on Track

    # x_car,y_car,car_found=find_car(image,front_color=[185,68,74],back_color=[76,116,142])
    # if( not car_found):
    #    print("Failed to Find Car 😯")
    #    sys.exit(-1)

# Find Car on Track
# Read an Car on Track Image
image = cv2.imread('./assets/ontrack/1.png')
x_car,y_car,car_found=find_car(image,front_color=[185,68,74],back_color=[76,116,142])
if(not car_found):
    print("Failed to Find Car 😯")
    sys.exit(-1)


###############################################################################################
##################################################Tests#########################################
###############################################################################################
# test_car_color('./assets/colorgradient/3.jpeg', [0, 255, 0])
# test_car_color('./assets/colorgradient/4.jpeg', [0, 0, 225])


# # Read an Car on Track Image
# image = cv2.imread('./assets/ontrack/1.png')

# # Find Car on Track
# x_car,y_car,car_found=find_car(image,front_color=[185,68,74],back_color=[76,116,142])
# if( not car_found):
#    print("Failed to Find Car 😯")
#    sys.exit(-1)


# 3
# # track = cv2.imread('./assets/track/1.png')

# track = cv2.imread('./assets/track/Straight_line.jpg')
# # BGR to RGB
# track_RGB = cv2.cvtColor(track, cv2.COLOR_BGR2RGB)


# track_RGB[:, :, :] = 255
# track_RGB = track_RGB[0:200, 0:200, :]
# track_RGB[100, :, :] = 0
# track_RGB[105, :, :] = 0

# # track_RGB[100,:,:]=255
# print(np.shape(track_RGB))
# show_images([track_RGB], ["Track 0"])
# extract_lines(track_RGB, threshold=100, minLineLength=100,
#               maxLineGap=50, thickness=3)

# # track = cv2.imread('./assets/track/1.png')
# track = cv2.imread('./assets/track/1.png')


# # BGR to RGB
# track_RGB = cv2.cvtColor(track, cv2.COLOR_BGR2RGB)

# extract_lines(track_RGB, threshold=100, minLineLength=100,
#               maxLineGap=50, thickness=1)


# # track = cv2.imread('./assets/track/1.png')
# track = cv2.imread('./assets/track/table.png')


# # BGR to RGB
# track_RGB = cv2.cvtColor(track, cv2.COLOR_BGR2RGB)

# extract_lines(track_RGB)

# track = cv2.imread('./assets/track/1.png')


# # BGR to RGB
# track_RGB = cv2.cvtColor(track, cv2.COLOR_BGR2RGB)

# extract_lines(track_RGB,threshold = 100,minLineLength = 100,maxLineGap = 50,thickness=3)


# track = cv2.imread('./assets/track/1.png')

# BGR to RGB
# track_RGB = cv2.cvtColor(track, cv2.COLOR_BGR2RGB)

# result3=extract_lines(track_RGB,rho = 50,theta = 1*np.pi/180,threshold = 100,minLineLength = 100,maxLineGap = 50,thickness=3)
# result4=extract_lines(track_RGB,rho = 10,theta = 1*np.pi/180,threshold = 100,minLineLength = 100,maxLineGap = 50,thickness=3)
# result5=extract_lines(track_RGB,rho = 30,theta = 1*np.pi/180,threshold = 100,minLineLength = 100,maxLineGap = 50,thickness=3)

# show_images([result3,result4,result5],["Rho 3","Rho 4","Rho 5"])

# image = cv2.imread('./assets/track/1.png')
# image = cv2.imread('./assets/track/Straight_line.jpg')
# show_images([image])
# D:\linefollower\Image Processing\assets\track\Straight_line.jpg

# # Read as Gary Scale
# img_gray = cv2.imread('./assets/track/Straight_line.jpg',0)
# size = np.size(img_gray)
# skel = np.zeros(img_gray.shape,np.uint8)

# # Syntax: cv.threshold(	src, thresh, maxval, type[, dst]	) ->
# # return retval(the computed threshold value if Otsu's or Triangle methods used), dst
# ret,img_binary = cv2.threshold(img_gray,127,255,cv2.THRESH_BINARY)

# print(ret,"eeeee")
# Convert to Binary
# element = cv2.getStructuringElement(cv2.MORPH_CROSS,(3,3))
# done = False

# show_images([img_gray,img_binary])

# # while( not done):
#     eroded = cv2.erode(img,element)
#     temp = cv2.dilate(eroded,element)
#     temp = cv2.subtract(img,temp)
#     skel = cv2.bitwise_or(skel,temp)
#     img = eroded.copy()

#     zeros = size - cv2.countNonZero(img)
#     if zeros==size:
#         done = True

#
# image = cv2.imread('./assets/track/Straight_line.jpg')

# # Convert the image to gray-scale
# image_gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

# skeleton(image_gray)
# from skimage.morphology import skeletonize


# image_gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
# # Threshold to Binary Image
# ret,image_binary= cv2.threshold(image_gray,127,255,0)
# image_binary=image_binary//255
# # print(np.max(image_binary))
# sk=skeleton(image_gray)

# show_images([image],["Original"])

# show_images([image_gray,image_binary,sk],["Gary","Binary","Skeleton"])

# image_gray = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
# skeleton=skeleton(image_gray,RGB=True)
# print(np.shape(skeleton))
# print(np.sum(skeleton))
# print(np.max(skeleton))
# skeleton=skeleton>0
# # skeleton = cv2.erode(skeleton,np.ones((10,10)))
# show_images([skeleton],["skeleton"])
# print(np.sum(skeleton>0))

##############################################################################################
# from modules.utils import thin

# image = cv2.imread('./assets/track/1.png')

# # Convert the image to RGB
# image_gray = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

# thinned=thin(image_gray)
# print(np.shape(thinned))


# # track = cv2.imread('./assets/track/1.png')
# track = cv2.imread('./assets/track/1.png')


# # BGR to RGB
# track_RGB = cv2.cvtColor(track, cv2.COLOR_BGR2RGB)

# extract_lines(track_RGB)