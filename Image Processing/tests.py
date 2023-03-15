from modules.utils import show_images,color_mask
import cv2
import numpy as np
import matplotlib.pyplot as plt

def test_color_mask():
    # Testing color_mask()
    # Read Gradient Image
    gradient_img_1 = cv2.imread('./assets/colorgradient/0.png')

    # Convert BGR to RGB
    image_rgb_1 = cv2.cvtColor(gradient_img_1, cv2.COLOR_BGR2RGB)

    ## Red
    mask_1,masked_img_1=color_mask(image_rgb_1,[255,0,0])
    show_images([image_rgb_1,mask_1,masked_img_1],['RGB','Mask',' Masked Image'],windowTitle="Test color_mask(Red)")
    
    ## Green
    mask_1,masked_img_1=color_mask(image_rgb_1,[0,255,0])
    show_images([image_rgb_1,mask_1,masked_img_1],['RGB','Mask',' Masked Image'],windowTitle="Test color_mask(Green)")
    
    ## Blue
    mask_1,masked_img_1=color_mask(image_rgb_1,[0,0,255])
    show_images([image_rgb_1,mask_1,masked_img_1],['RGB','Mask',' Masked Image'],windowTitle="Test color_mask(Blue)")
    
    ## Yellow
    mask_1,masked_img_1=color_mask(image_rgb_1,[255,255,0])
    show_images([image_rgb_1,mask_1,masked_img_1],['RGB','Mask',' Masked Image'],windowTitle="Test color_mask(Yellow)")
    

    ## Gradient 2
    # Read Gradient Image
    gradient_img_1 = cv2.imread('./assets/colorgradient/1.png')

    # Convert BGR to RGB
    image_rgb_1 = cv2.cvtColor(gradient_img_1, cv2.COLOR_BGR2RGB)

    ## Red
    mask_1,masked_img_1=color_mask(image_rgb_1,[255,0,0])
    show_images([image_rgb_1,mask_1,masked_img_1],['RGB','Mask',' Masked Image'],windowTitle="Test color_mask(Red)")
    
    ## Green
    mask_1,masked_img_1=color_mask(image_rgb_1,[0,255,0])
    show_images([image_rgb_1,mask_1,masked_img_1],['RGB','Mask',' Masked Image'],windowTitle="Test color_mask(Green)")
    
    ## Blue
    mask_1,masked_img_1=color_mask(image_rgb_1,[0,0,255])
    show_images([image_rgb_1,mask_1,masked_img_1],['RGB','Mask',' Masked Image'],windowTitle="Test color_mask(Blue)")
    
    ## Yellow
    mask_1,masked_img_1=color_mask(image_rgb_1,[255,255,0])
    show_images([image_rgb_1,mask_1,masked_img_1],['RGB','Mask',' Masked Image'],windowTitle="Test color_mask(Yellow)")
    

    #
    # Read Gradient Image
    gradient_img_1 = cv2.imread('./assets/colorgradient/2.png')

    # Convert BGR to RGB
    image_rgb_1 = cv2.cvtColor(gradient_img_1, cv2.COLOR_BGR2RGB)

    ## Red
    mask_1,masked_img_1=color_mask(image_rgb_1,[255,0,0])
    show_images([image_rgb_1,mask_1,masked_img_1],['RGB','Mask',' Masked Image'],windowTitle="Test color_mask(Red)")
    
    ## Green
    mask_1,masked_img_1=color_mask(image_rgb_1,[0,255,0])
    show_images([image_rgb_1,mask_1,masked_img_1],['RGB','Mask',' Masked Image'],windowTitle="Test color_mask(Green)")
    
    ## Blue
    mask_1,masked_img_1=color_mask(image_rgb_1,[0,0,255])
    show_images([image_rgb_1,mask_1,masked_img_1],['RGB','Mask',' Masked Image'],windowTitle="Test color_mask(Blue)")
    
    ## Yellow
    mask_1,masked_img_1=color_mask(image_rgb_1,[255,255,0])
    show_images([image_rgb_1,mask_1,masked_img_1],['RGB','Mask',' Masked Image'],windowTitle="Test color_mask(Yellow)")
    
    return None


def test_car_color(path,color):
    print(color)
    '''
    color: list RGB
    '''
    gradient_img_1 = cv2.imread(path)

    # Convert BGR to RGB
    image_rgb_1 = cv2.cvtColor(gradient_img_1, cv2.COLOR_BGR2RGB)

    ## Red
    mask_1,masked_img_1=color_mask(image_rgb_1,color)
    show_images([image_rgb_1,mask_1,masked_img_1],['RGB','Mask',' Masked Image'],windowTitle="Test color_mask {color}")
    return