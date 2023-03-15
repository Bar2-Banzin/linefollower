import globals
from modules.utils import *

def find_car(image,front_color,back_color):
    """
    This function is used to find car in the picture
    Arguments:
        image : RGB img with car
        front_color: front color of the car RGB color i.e [0,255,0] list
        back_color: back color of the car RGB color i.e [0,255,0] list
    Returns: center,found
    """

    # Detect front of the car
    x_f,y_f,found=color_center(image,front_color)
    if(not found):
        # front_color not detected
        print("Front_color not detected")
        return -1,-1,False

    # Detect back of the car
    x_b,y_b=color_center(image,back_color)
    if(not found):
        # back_color not detected
        print("back_color not detected")
        return -1,-1,False


    # Car center
    x_center=(x_f+x_b)//2
    y_center=(y_f+y_b)//2

    return x_center,y_center,True