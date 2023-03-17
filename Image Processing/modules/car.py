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


def car_on_line(x_car,y_car,lines_matrix,threshold=1):
    """
    x_car,y_car: Center of the Car
    lines_matrix: Binary Matrix with 1's = lines
    threshold : min sum to consider Car on line [With the uncommented part to make Region for the Car]
    """
    car_matrix=np.zeros(np.shape(lines_matrix),np.uint8)

    car_matrix[x_car,y_car]=1

    # # Out of Range Error :(
    # if(x_car-10<0):
    #     x_car=10
    # if(x_car+10>=np.shape(lines_matrix)[0]):
    #     x_car=x_car-10 # Check Practically
    
    # if(y_car-10<0):
    #     y_car=10
    # if(y_car+10>=np.shape(lines_matrix)[1]):
    #     y_car=y_car-10 # Check Practically
    
    # car_matrix[x_car-10:x_car+10,y_car-10:y_car+10]=1

    # line_car=lines_matrix*car_matrix
    line_car=np.bitwise_and(lines_matrix,car_matrix)
    # print(np.max(line_car))
    # print(np.shape(line_car))

    if(globals.debug):
        show_images([car_matrix,lines_matrix],["car_matrix","line_matrix"])

    if(np.sum(line_car)>=threshold):
        # On Line
        return True
    else:
        # Not on Line
        return False