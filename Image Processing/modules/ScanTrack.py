import globals

from modules.utils import *
from modules.PaperExtractor import ExtractPaper
def scan_track(track_image):
    '''
    track_image: BGR Track Image
    '''
    # # 1.Extract Track Paper From the Image
    # paper_img,wrapped=ExtractPaper(track_image)

    # if(not wrapped):
    #     return False
    
    # 2.Detect Straight Lines in the Track
    # TODO:
    return None


def extract_lines(image,rho = 1,theta = 1*np.pi/180,threshold = 100,minLineLength = 100,maxLineGap = 50,thickness=3):
    """
    Extract Line out of the RGB image 
    Arguments:
        image: RGB Track image
        threshold: no of min votes so that line is detected as line by Hough
        minLineLength: min length so the line is detected
        maxLineGap: max allowed gap between lines to be treated as a 1 line (this breaks long un connected lines to 2 lines :D)
        thickness: thickness of white line drawn for straight line (returned image)
    Returns: Binary image of size = image size where 1's detect line
    """
    
    # Convert the image to gray-scale
    image_gray = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)

    # Thinning Image
    thinned=thin(image_gray)

    # Find the edges in the image using canny detector
    edges = cv2.Canny(thinned, 50, 200)

    # Hough Lines
    # Syntax:HoughLinesP( Edged image , Rho Resolution, Angle Resolution,threshold,min length of line,max distance between lines)
    # minLineLength: Lines shorter than that are rejected
    # maxLineGap: max allowed gap between lines to be treated as a 1 line (this breaks long un connected lines to 2 lines :D)
    # 0<=rho<Rmax
    lines = cv2.HoughLinesP(edges,rho =rho,theta = theta,threshold = threshold,minLineLength = minLineLength,maxLineGap = maxLineGap)
    print("Hough Lines Detected",np.shape(lines))

    # Draw lines on the image
    image_lines=np.zeros(np.shape(image)) # Inverted image
    for line in lines:
        x1, y1, x2, y2 = line[0]
        cv2.line(image_lines, (x1, y1), (x2, y2), (255, 255, 255), thickness)
    
    # Take one channel (Binary)
    image_lines=image_lines[:,:,0]
    
    if(globals.debug):
        show_images([image,image_gray], ['Original',"Gray"],windowTitle="extract_lines",BGR=False)
        show_images([thinned,edges,image_lines], ['Thinned','Edges','Lines Matrix'],windowTitle="extract_lines",BGR=False)
    
    # Return a Binary Image (1 Channel) for the lines
    return image_lines