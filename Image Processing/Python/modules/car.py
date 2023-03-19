import globals
from modules.utils import *

def find_car(image,front_color,back_color):
    """
    This function is used to find car in the picture
    Arguments:
        image : RGB img with car
        front_color: front color of the car RGB color i.e [0,255,0] list
        back_color: back color of the car RGB color i.e [0,255,0] list
    Returns: center,front point,back point,found
    """

    # Detect front of the car
    x_f,y_f,found=color_center(image,front_color)
    if(not found):
        # front_color not detected
        print("Front_color not detected")
        return -1,-1,-1,-1,-1,-1,False

    # Detect back of the car
    x_b,y_b=color_center(image,back_color)
    if(not found):
        # back_color not detected
        print("back_color not detected")
        return -1,-1,-1,-1,-1,-1,False


    # Car center
    x_center=(x_f+x_b)//2
    y_center=(y_f+y_b)//2

    return x_center,y_center,x_f,y_f,x_b,y_b,True


def car_on_line(x_car,y_car,lines_matrix,threshold=1):
    """
    x_car,y_car: Center of the Car
    lines_matrix: Binary Matrix with 1's = lines
    threshold : min sum to consider Car on line [With the uncommented part to make Region for the Car]
    """
    car_matrix=np.zeros(np.shape(lines_matrix),np.uint8)
    # print(np.shape(car_matrix))

    car_matrix[y_car,x_car]=1

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
    # FIXME: Take Car that It is Assumed no of lines max 255
    line_car=np.bitwise_and(lines_matrix,car_matrix)
    # Convert to Binary to Count no of Points where there is an intersection
    line_car=line_car>0
    # print(np.max(line_car))
    # print(np.shape(line_car))

    if(globals.debug):
        kernel = cv2.getStructuringElement(cv2.MORPH_RECT,(10,10))
        print("line_car",1*line_car)
        Dilate = cv2.dilate(np.copy(1*line_car).astype('uint8') ,kernel)
        show_images([car_matrix,lines_matrix],["car_matrix","line_matrix"],windowTitle="car_on_line",BGR=False)
        show_images([Dilate],["line_car"],windowTitle="car_on_line",BGR=False)

    if(np.sum(line_car)>=threshold):
        # On Line
        # Color of this Line
        # print("Index",lines_matrix[line_car==1])
        line_index=255-lines_matrix[line_car==1] # 255 means line 1
        return True,line_index
    else:
        # Not on Line
        return False,-1