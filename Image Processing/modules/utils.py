import sys

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



def drawRectangle(img, vertices,color=(0, 255, 0), thickness=10):
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
    Draw a given Rectangular contour on copy of img
    Arguments:
        img: image to draw on a copy of it
        contour: 4 point contour to be drawn
        color:
        thickness:
    Returns: copy of img with sent contour drawn on it
    '''
    contour_img=np.copy(img)
    contour= reorderPoints(contour)#reorder contour points

    cv2.drawContours(contour_img, contour, -1, color , thickness) # Will Draw only 4 points
    contour_img = drawRectangle(contour_img,contour,color,thickness)

    return contour_img