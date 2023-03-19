import cv2
import numpy as np
import matplotlib.pyplot as plt
from modules.utils import show_images, color_mask
from modules.ScanTrack import extract_lines


def test_color_mask():
    # Testing color_mask()
    # Read Gradient Image
    gradient_img_1 = cv2.imread('./assets/colorgradient/0.png')

    # Convert BGR to RGB
    image_rgb_1 = cv2.cvtColor(gradient_img_1, cv2.COLOR_BGR2RGB)

    # Red
    mask_1, masked_img_1 = color_mask(image_rgb_1, [255, 0, 0])
    show_images([image_rgb_1, mask_1, masked_img_1], ['RGB', 'Mask',
                ' Masked Image'], windowTitle="Test color_mask(Red)")

    # Green
    mask_1, masked_img_1 = color_mask(image_rgb_1, [0, 255, 0])
    show_images([image_rgb_1, mask_1, masked_img_1], ['RGB', 'Mask',
                ' Masked Image'], windowTitle="Test color_mask(Green)")

    # Blue
    mask_1, masked_img_1 = color_mask(image_rgb_1, [0, 0, 255])
    show_images([image_rgb_1, mask_1, masked_img_1], ['RGB', 'Mask',
                ' Masked Image'], windowTitle="Test color_mask(Blue)")

    # Yellow
    mask_1, masked_img_1 = color_mask(image_rgb_1, [255, 255, 0])
    show_images([image_rgb_1, mask_1, masked_img_1], ['RGB', 'Mask',
                ' Masked Image'], windowTitle="Test color_mask(Yellow)")

    # Gradient 2
    # Read Gradient Image
    gradient_img_1 = cv2.imread('./assets/colorgradient/1.png')

    # Convert BGR to RGB
    image_rgb_1 = cv2.cvtColor(gradient_img_1, cv2.COLOR_BGR2RGB)

    # Red
    mask_1, masked_img_1 = color_mask(image_rgb_1, [255, 0, 0])
    show_images([image_rgb_1, mask_1, masked_img_1], ['RGB', 'Mask',
                ' Masked Image'], windowTitle="Test color_mask(Red)")

    # Green
    mask_1, masked_img_1 = color_mask(image_rgb_1, [0, 255, 0])
    show_images([image_rgb_1, mask_1, masked_img_1], ['RGB', 'Mask',
                ' Masked Image'], windowTitle="Test color_mask(Green)")

    # Blue
    mask_1, masked_img_1 = color_mask(image_rgb_1, [0, 0, 255])
    show_images([image_rgb_1, mask_1, masked_img_1], ['RGB', 'Mask',
                ' Masked Image'], windowTitle="Test color_mask(Blue)")

    # Yellow
    mask_1, masked_img_1 = color_mask(image_rgb_1, [255, 255, 0])
    show_images([image_rgb_1, mask_1, masked_img_1], ['RGB', 'Mask',
                ' Masked Image'], windowTitle="Test color_mask(Yellow)")

    # Read Gradient Image
    gradient_img_1 = cv2.imread('./assets/colorgradient/2.png')

    # Convert BGR to RGB
    image_rgb_1 = cv2.cvtColor(gradient_img_1, cv2.COLOR_BGR2RGB)

    # Red
    mask_1, masked_img_1 = color_mask(image_rgb_1, [255, 0, 0])
    show_images([image_rgb_1, mask_1, masked_img_1], ['RGB', 'Mask',
                ' Masked Image'], windowTitle="Test color_mask(Red)")

    # Green
    mask_1, masked_img_1 = color_mask(image_rgb_1, [0, 255, 0])
    show_images([image_rgb_1, mask_1, masked_img_1], ['RGB', 'Mask',
                ' Masked Image'], windowTitle="Test color_mask(Green)")

    # Blue
    mask_1, masked_img_1 = color_mask(image_rgb_1, [0, 0, 255])
    show_images([image_rgb_1, mask_1, masked_img_1], ['RGB', 'Mask',
                ' Masked Image'], windowTitle="Test color_mask(Blue)")

    # Yellow
    mask_1, masked_img_1 = color_mask(image_rgb_1, [255, 255, 0])
    show_images([image_rgb_1, mask_1, masked_img_1], ['RGB', 'Mask',
                ' Masked Image'], windowTitle="Test color_mask(Yellow)")
    return None


def test_car_color(path, color):
    print(color)
    '''
    color: list RGB
    '''
    gradient_img_1 = cv2.imread(path)

    # Convert BGR to RGB
    image_rgb_1 = cv2.cvtColor(gradient_img_1, cv2.COLOR_BGR2RGB)

    # Red
    mask_1, masked_img_1 = color_mask(image_rgb_1, color)
    show_images([image_rgb_1, mask_1, masked_img_1], ['RGB', 'Mask',
                ' Masked Image'], windowTitle="Test color_mask {color}")
    return


def test_extract_lines(path, rho=1, theta=1*np.pi/180, threshold=100, minLineLength=100, maxLineGap=50, thickness=3):
    '''
    Test extract_lines
    '''
    image = cv2.imread(path)

    # Convert to RGB
    image_RGB = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

    extract_lines(image_RGB, rho == rho, theta=theta, threshold=threshold,
                  minLineLength=minLineLength, maxLineGap=maxLineGap, thickness=thickness)
    return None

##########################################################################################################
def draw_car(image,x_car,y_car,normalized=False):
    """
    image:Binary Image to Draw Car on it 
    x_car,y_car: center of rectangle to be Drawn
    normalized: if True = range from 0-1 if false range 0-255
    return : New Drawn Image
    """
    draw_car_image=np.copy(image)
    if(x_car-20<0):
        x_car=20
    if(x_car+20>np.shape(image)[1]):
        x_car=x_car-20

    if(y_car-20<0):
        y_car=20
    if(y_car+20>np.shape(image)[0]):
        y_car=y_car-20
        
    draw_car_image[y_car-20:y_car+20,x_car-20:x_car+20]= 1 if(normalized) else 255

    show_images([draw_car_image], ['Car Location'],windowTitle="Car_Location",BGR=False)

    return None