from utils import *


def increase_decrease_speed(x_car_front, y_car_front, x_car_back, y_car_back, line, dist_threshold):
    """
    x_car_front,y_car_front : front Center of the Car
    x_car_back,y_car_back : back Center of the Car
    line:[x1,y1,x2,y2] : start and end point of the line
    return: Boolean True => increase False Decrease [FIXME: To Be Modified Later to Send Speed Depending on Distance not only a flag 😉]
    """

    # 1. Get Car Direction [always from back to front]
    car = direction(x1=x_car_back, y1=y_car_back,
                    x2=x_car_front, y2=y_car_front)
    print("car Vector", car)

    line_point = [0, 0]

    # 2. Direction P1P2
    P1P2 = direction(x1=line[0], y1=line[1], x2=line[2], y2=line[3])
    print("P1P2", P1P2)

    if (np.sign(P1P2[0]) == np.sign(car[0]) and np.sign(P1P2[1]) == np.sign(car[1])):
        print("Car is Moving towards P2")
        line_point = [line[2], line[3]]

    # 3. Direction P2P1
    P2P1 = direction(x2=line[0], y2=line[1], x1=line[2], y1=line[3])
    print("P2P1", P2P1)
    if (np.sign(P2P1[0]) == np.sign(car[0]) and np.sign(P2P1[1]) == np.sign(car[1])):
        print("Car is Moving towards P1")
        line_point = [line[0], line[1]]

    # 4. Take Action Depending on Distance between Car and the endpoint
    if (calculateDistance(x1=x_car_front, y1=y_car_front, x2=line_point[0], y2=line_point[1]) > dist_threshold):
        # Increase Speed
        print("Increase Speed")
        return True

    else:
        # Don't Increase Speed you are toward the line End
        print("Decrease Speed")
        return False


# Test
increase_decrease_speed(x_car_front=15, y_car_front=15, x_car_back=10,
                        y_car_back=10, line=[0, 0, 100, 100], dist_threshold=50)
# car Vector (5, 5)
# P1P2 (100, 100)
# P2P1 (-100, -100)
