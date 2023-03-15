import globals
from modules.utils import *


def ExtractPaper(img_BGR):
    """
    Extract Paper out of the image 
    Arguments:
        img_BGR: BGR image
    Returns:

    """
    # Get image dimensions
    imgHeight = img_BGR.shape[0]
    imgWidth = img_BGR.shape[1]

    # Convert to RGB Scale
    img_RGB = cv2.cvtColor(img_BGR, cv2.COLOR_BGR2RGB)

    # Convert to Gray Scale
    img_gray = cv2.cvtColor(img_BGR, cv2.COLOR_BGR2GRAY)

    # ===========================================================PreProcessing=========================================================
    # Gaussian Filter on the image to remove noise
    # Syntax: Gray Scale image,Kernel size(positive and odd),Sigma
    Blurred_image_Gaussian = cv2.GaussianBlur(img_gray, (5, 5), 1)

    # # Median Filter to remove Salt and Pepper
    # # Syntax: Image-Kernel size
    # Blurred_image_Median=cv2.medianBlur(img_gray, 5)

    # ===========================================================Edge Detection=========================================================
    # Using Canny (Optimal Edge Detector)
    # Steps of Algorithm
    # 1.Gaussian Smoothing:
    # 2.Sobel: to get first derivative in horizontal direction (Gx) and vertical direction (Gy) => the n it finds find edge gradient and direction for each pixel as follows:
    # Edge_Gradient(G)=root(G2x+G2y),Angle(θ)=tan−1(Gy/Gx)
    # 3.Non Maxima Suppression: to get rid of Double response of edges [Thin Edges] ie pixel is checked if it is a local maximum in its neighborhood in the direction of gradient.
    # 4.Hysteresis threshold's:decides which are all edges are really edges and which are not Any edges with intensity gradient more than maxVal are sure to be edges and those below minVal are sure to be non-edges, so discarded. Those who lie between these two thresholds are classified edges or non-edges based on their connectivity.
    # Syntax: Image(GrayScale),MinThreshold,MaxThreshold for hysteresis threshold
    Edged_Image = cv2.Canny(Blurred_image_Gaussian, 180, 255)

    # ===========================================================Erosion & Dilation=======================================================
    Dilated_img = Edged_Image
    Errored_img = Edged_Image

    # ============================================================Getting Contours=========================================================
    # cv2.findContour() works best on binary images
    # so we should first apply threshold techniques, Sobel edges, etc. Already Done Above :D
    # WARNING : Use a copy of the image =>since findContours alters the image
    contours_temp = np.copy(Errored_img)
    # Syntax: image,contour retrieval mode,contour approximation method
    # Contour retrieval mode: RETR_EXTERNAL(outer contours) - RETR_LIST(all no hierarchy) - RETR_TREE(all in hierarchy)
    # Contour approximation method(how coordinates are stored): CHAIN_APPROX_NONE(all the boundary points are stored) - CHAIN_APPROX_SIMPLE(It removes all redundant points)
    # Return: Contours is a Python list of all the contours in the image. Each individual contour is a Numpy array of (x,y) coordinates of boundary points of the object.
    #         hierarchy containing information about the image topology
    contours, hierarchy = cv2.findContours(
        contours_temp, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    if (globals.debug):
        # Drawing Contours
        # Syntax::image(So we can draw on it the contours),contours list, index of contours (useful when drawing individual contour. To draw all contours, pass -1) ,color, thickness
        img_contours = np.copy(img_RGB)
        cv2.drawContours(img_contours, contours, -1, (0, 255, 0), 10)

    # Getting Biggest Rectangular Contour
    biggest_contour, max_area = get_biggest_rectangular_contour(contours)

    if (globals.debug):
        print("Max_Area", max_area)
        print("Height*Width", img_gray.shape[0]*img_gray.shape[1])

    # Solving problem of False Contours The max area must be Greater than 10% of the Image area
    if (max_area < 0.10*imgHeight*imgWidth):
        # Couldn't Extract Paper
        WarpedColoredImage = img_gray
        Wrapped = False
        return WarpedColoredImage, Wrapped

    img_biggest_contours = None

    if (globals.debug):
        # Draw Biggest Contours
        img_biggest_contours = draw_contour(
            img_RGB, biggest_contour, color=(255, 255, 0))

    # =============================================================Perspective============================================================
    # Re order Contour Points
    biggest_contour = reorderPoints(biggest_contour)
    pts1 = np.float32(biggest_contour)
    pts2 = np.float32(
        [[0, 0], [imgWidth, 0], [0, imgHeight], [imgWidth, imgHeight]])

    matrix = cv2.getPerspectiveTransform(pts1, pts2)
    WarpedColoredImage = cv2.warpPerspective(
        img_RGB, matrix, (imgWidth, imgHeight))

    if (globals.debug):
        show_images([img_RGB, img_gray, Blurred_image_Gaussian, Edged_Image, img_contours, img_biggest_contours, WarpedColoredImage], [
                    "RGB Paper", "Gray Paper", "Gaussian", "Edges", "Contours", "Biggest Contour", "Wrapped"], BGR=False, windowTitle="ExtractPaper")

    Wrapped = True
    return WarpedColoredImage, Wrapped
