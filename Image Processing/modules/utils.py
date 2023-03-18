import sys

import math
import cv2
import numpy as np
import matplotlib.pyplot as plt


def show_images(images, titles=None, BGR=False, windowTitle=None):
    """
    This function is used to show image(s) with titles by sending an array of images and an array of associated titles
    Arguments:
        images: arrays of images
        titles: arrays of strings titles for the images
        BGR: Flag if images are BGR (default) true
    Returns: None
    """
    n_ims = len(images)
    if titles is None:
        titles = ['(%d)' % i for i in range(1, n_ims + 1)]
    fig = plt.figure(windowTitle)
    n = 1
    for image, title in zip(images, titles):
        a = fig.add_subplot(2, n_ims % 4+1, n)
        if image.ndim == 2:
            plt.gray()
        if (BGR):
            image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        plt.imshow(image)
        a.set_title(title)
        n += 1
    fig.set_size_inches(np.array(fig.get_size_inches()) * n_ims)
    plt.show()

    return None


def reorderPoints(points):
    '''
    Reorder 4 Points in clockwise order, starting from top left
    Arguments:
        points: arrays of 4 points
    Returns: ordered array of 4 points
    '''

    points = points.reshape((4, 2))
    newPoints = np.zeros((4, 1, 2), dtype=np.int32)
    add = points.sum(1)

    newPoints[0] = points[np.argmin(add)]
    newPoints[3] = points[np.argmax(add)]
    diff = np.diff(points, axis=1)
    newPoints[1] = points[np.argmin(diff)]
    newPoints[2] = points[np.argmax(diff)]

    return newPoints


def drawRectangle(img, vertices, color=(0, 255, 0), thickness=10):
    '''
    Draw Rectangle between 4 points ordered clock-wise
        Arguments:
        img: image to draw on a copy of it
        contour: contour to be drawn
        color:
        thickness:
    Returns: copy of img with sent contour drawn on it
    '''
    cv2.line(img, (vertices[0][0][0], vertices[0][0][1]),
             (vertices[1][0][0], vertices[1][0][1]), color, thickness)
    cv2.line(img, (vertices[0][0][0], vertices[0][0][1]),
             (vertices[2][0][0], vertices[2][0][1]), color, thickness)
    cv2.line(img, (vertices[3][0][0], vertices[3][0][1]),
             (vertices[2][0][0], vertices[2][0][1]), color, thickness)
    cv2.line(img, (vertices[3][0][0], vertices[3][0][1]),
             (vertices[1][0][0], vertices[1][0][1]), color, thickness)

    return img


def get_biggest_rectangular_contour(contours):
    '''
    Get Biggest Contour from the given contours
    Arguments:
        contour: contours to be sorted
    Returns: biggest_contour,area of biggest_contour
    '''
    biggest_contour = np.array([])
    max_area = 0
    j = 0
    for i in contours:
        area = cv2.contourArea(i)
        # Approximate The Contour to the nearest Poly
        peri = cv2.arcLength(i, True)
        approx = cv2.approxPolyDP(i, 0.02 * peri, True)

        # If Greater than Max Aea and it is a rectangle
        if area > max_area and len(approx) == 4:
            max_area = area
            biggest_contour = approx
        j = j+1
    return biggest_contour, max_area


def draw_contour(img, contour, color=(0, 255, 0), thickness=10):
    '''
    Draw a given Rectangular contour[given 4 points] on copy of img
    Arguments:
        img: image to draw on a copy of it
        contour: 4 point contour to be drawn
        color:
        thickness:
    Returns: copy of img with sent contour drawn on it
    '''
    contour_img = np.copy(img)
    contour = reorderPoints(contour)  # reorder contour points

    cv2.drawContours(contour_img, contour, -1, color,
                     thickness)  # Will Draw only 4 points
    contour_img = drawRectangle(contour_img, contour, color, thickness)

    return contour_img


def min_rectangle(image,contour,color=[255,0,0],thickness=10):
    ''''
    Get center of min rectangle around given contour
    Arguments:
        image:RGB image to Draw on it Rectangle
        contour: 4 point contour
    Returns: center of rectangle
    Can Return Dimensions of Rect and angle of Rotation
    '''
    rectangle_image=np.copy(image)

    # Min Rectangle
    rect = cv2.minAreaRect(contour)
    box = cv2.boxPoints(rect)
    box = np.int0(box)
    if(globals.test):
        cv2.drawContours(rectangle_image,[box],0,color,thickness)


    # Center and angle of rotation of Rectangle
    ((x, y), (width, height), angle_of_rotation) = cv2.minAreaRect(box)
    height = int(max(width, height))
    width = int(min(width, height))

    return int(x), int(y)


def color_range(color):
    ''''
    Gets Upper & Lower HSV Range of the RGB color
    Arguments:
        color: RGB color i.e [0,255,0] list
    Returns: lower and upper Range of color in HSV
    Call: lower_range, upper_range=color_range([255,0,0])
    '''
    color_rgb = np.uint8([[color]])
    HSV = cv2.cvtColor(color_rgb, cv2.COLOR_RGB2HSV)
    Hue = HSV[0][0][0]

    lower_range = np.array([Hue-10, 100, 100])
    upper_range = np.array([Hue+10, 255, 255])

    return lower_range, upper_range

def color_mask(image,color):
    ''''
    Mask RGB image color in the specific range passed
    Arguments:
        image: RGB image
        color: RGB color i.e [0,255,0] list
    Returns: mask and image with mask applied on it
    Call: mask,masked_img = color_mask(image,color=[255,0,0])
    '''

    # Get Upper and Lower Range of Color
    lower_range,upper_range=color_range(color)
   
    # Convert RGB image to HSV
    image_hsv = cv2.cvtColor(image, cv2.COLOR_RGB2HSV)

    # Check Range
    mask = cv2.inRange(image_hsv, lower_range, upper_range)

    # Apply mask on result
    result=np.copy(image)
    masked_img= cv2.bitwise_and(result, result, mask=mask)

    return mask,masked_img

def color_center(image,color):
    ''''
    Get Center and Draw Rectangle Around Largest Contour of a given Color
    Arguments:
        image: RGB image
        color: RGB color i.e [0,255,0] list
    Returns: center
    Call: mask,masked_img = color_center(image,color=[255,0,0]),error flag if color not found
    '''
    # Get Mask
    mask,masked_img=color_mask(image,color)    

    # Mask Contours
    contours, hierarchy = cv2.findContours(mask, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
    # Biggest Contour
    contours = sorted(contours, key=cv2.contourArea, reverse=True)
    
    if(np.shape(contours)[0]<=0):
        return -1,-1,False
    x,y=min_rectangle(image,contours[0])
    
    return x,y,True


def skeleton(image,RGB=False):
    """{To be Tested Later}
    image: RGB or Gray Scale Image[According to RGB Flag]
    RGB: Flag to show if passed image is RGB or Gray  (default = Gray)
    """ 
    image_gray=np.copy(image)

    if(RGB):
        image_gray = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)

    size = np.size(image)
    skeleton = np.zeros(image_gray.shape,np.uint8)

    # Threshold to Binary Image
    ret,image_binary= cv2.threshold(image_gray,127,255,0)

    # Structure Element
    element = cv2.getStructuringElement(cv2.MORPH_CROSS,(3,3))
    done = False

    while( not done):
        eroded = cv2.erode(image_binary,element)
        temp = cv2.dilate(eroded,element)
        temp = cv2.subtract(image_binary,temp)
        skeleton = cv2.bitwise_or(skeleton,temp)
        image_binary = eroded.copy()

        zeros = size - cv2.countNonZero(image_binary)
        if zeros==size:
            done = True

    # show_images([image,image_gray,image_binary],["Original","Gray","Binary"])
    # show_images([skeleton],["Skeleton"])

    return skeleton


def thin(image):
    """
    image: Gray Image
    Returns: thinned Binary Image
    """
    # Convert to Binary
    ret,image_binary= cv2.threshold(image,127,255,0)

    # line is white
    image_binary=np.bitwise_not(image_binary)

    # Structuring Element
    kernel = cv2.getStructuringElement(cv2.MORPH_CROSS,(3,3))
    # Create an empty output image to hold values
    thin = np.zeros(np.shape(image_binary),dtype='uint8')

    # Loop until erosion leads to an empty set
    while (cv2.countNonZero(image_binary)!=0):
        # Erosion
        erode = cv2.erode(image_binary,kernel)
        # Opening on eroded image
        opening = cv2.morphologyEx(erode,cv2.MORPH_OPEN,kernel)
        # Subtract these two
        subset = erode - opening
        # Union of all previous sets
        thin = cv2.bitwise_or(subset,thin)
        # Set the eroded image for next iteration
        image_binary = erode.copy()


    # show_images([image,image_binary,thin],["Original","Binary","Thin"])        
    return thin


def direction(x1,y1,x2,y2):
    """
    x1,y1:point 1
    x2,y2:point 2
    return :vector x1y1-x2y2
    """
    return ((x2-x1,y2-y1))


# print(direction(10,10,15,15))
# print(direction(15,15,10,10))

def calculateDistance(x1,y1,x2,y2):
    """
    Calculate the Euclidean distance between the two vectors x1,y1 and x2,y2.
    return :Euclidean distance 
    """
    distance = math.sqrt((x2-x1)**2+(y2-y1)**2)
    return distance