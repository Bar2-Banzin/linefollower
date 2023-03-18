from modules.utils import *

lines=np.array([12,3,45,6])
for index, line in enumerate(lines):
# for line in lines:
    print(index,"d",line)
    # print(line)


x=np.array([1,1,0,1,21,0])
y=np.array([1,2,3,4,5,6])
print(y[x==0])

line_car=np.bitwise_and(np.array([255,2,6,0,4]),np.array([0,255,0,0,0]))
print(line_car)
