import os
import sys
import shutil
import glob

def main(argv):

    j=argv[0]

    # Take from Shared Folder
    dir_name = './assets/TestCases/SharedFolder'
    dest_name="./assets/TestCases/TestCase"+str(j)


    track_dire = './assets/TestCases/SharedFolder/track'

    # Clear folder

    shutil.rmtree(dest_name, ignore_errors=False, onerror=None)
    os.mkdir(dest_name)

    # Make txt files
    open(os.path.join(dest_name , "result.txt"), "w")
    open(os.path.join(dest_name , "result_true.txt"), "w")


    files = os.listdir(dir_name)
    # print(files)

    for i in files:
        new_dir=os.path.join(dest_name , i.split(".")[0])
        if(i.split(".")[0]=="track"):
            continue
        if os.path.exists(new_dir):
            shutil.rmtree(new_dir)
        os.mkdir(new_dir)
        shutil.copy(os.path.join(dir_name , i), os.path.join(dest_name , i.split(".")[0]))
        # print(os.path.join(dest_name , i.split(".")[0],i.split(".")[0])+".jpg")
        # print(os.path.join(dest_name , i.split(".")[0],"car.jpg"))
        os.rename(os.path.join(dest_name , i.split(".")[0],i.split(".")[0])+".jpg",os.path.join(dest_name , i.split(".")[0],"car.jpg"))
        os.makedirs(os.path.join(dest_name , i.split(".")[0],"results"))
        # Move Track Image
        shutil.copy(os.path.join(track_dire ,"eslam.jpg"), os.path.join(dest_name , i.split(".")[0],"track.jpg"))
    return 

if __name__ == "__main__":
    main(sys.argv[1:])