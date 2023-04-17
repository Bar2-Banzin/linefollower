import os
import sys
import difflib
import subprocess
import getopt

path_of_the_directory="./assets/TestCases"
path_of_the_exe="./x64/Debug/main.exe"


def compareFiles(path1,path2):
    similar=True
    with open(path1) as file_1:
        file_1_text = file_1.readlines()
    
    with open(path2) as file_2:
        file_2_text = file_2.readlines()
    
    # Find and print the diff:
    for line in difflib.unified_diff(
            file_1_text, file_2_text, fromfile='result.txt',
            tofile='result_true.mem', lineterm=''):
        print(line)
        similar=False

    return similar



def runTestCase_minor(i,j):
    print("Running Test Case ",i,",",j)
    
    # Run C++
    return_code=subprocess.run([path_of_the_exe,str(i),str(j)])
    # print(return_code.returncode)


    # Write the result in the file
    with open(path_of_the_directory+"/TestCase"+str(i)+"/result.txt", 'r') as file:
        # read a list of lines into data
        data = file.readlines()

    data[j-1]=str(j)+":"+" "+str(return_code.returncode)

    # and write everything back
    with open(path_of_the_directory+"/TestCase"+str(i)+"/result.txt", 'w') as file:
        file.writelines( data )


    # Compare only this line

    with open(path_of_the_directory+"/TestCase"+str(i)+"/result_true.txt", 'r') as file:
        # read a list of lines into data
        data_true = file.readlines()
    
    if(not (data_true[j-1]==data[j-1])):
        print("TestCase ("+str(i)+"),("+str(j)+") Failed ")
    else:
        print("TestCase ("+str(i)+"),("+str(j)+") Succeeded ")

    return None

def runTestCase(i):
    print("Running Test Case ",i)

    result=[]

    # f = os.path.join(path_of_the_directory,"/TestCase"+str(i))
    f=path_of_the_directory+"/TestCase"+str(i)

    j=1
    for sub_testcase in os.listdir(f):
        # 1 2 result.txt result_test.txt
        f_sub_testcase = os.path.join(f,sub_testcase)
        if os.path.isdir(f_sub_testcase):
            #  track.jpeg car.jpeg result
            # print(f_sub_testcase)    
            # Send to C++ Program
            return_code=subprocess.run([path_of_the_exe,str(i),str(j)])
            # print("Main:",i,"Sub:",j)
            result.append(str(j)+":"+" "+str(return_code.returncode)+"\n")
            j=j+1

    result[-1]=result[-1].strip()
    # Save Results in the file
    with open(path_of_the_directory+"/TestCase"+str(i)+"/result.txt", 'w') as file:
        file.writelines(result)


    
    if( not compareFiles(path1=path_of_the_directory+"/TestCase"+str(i)+"/result.txt",path2=path_of_the_directory+"/TestCase"+str(i)+"/result_true.txt")):
        print("TestCase ("+str(i)+") Failed ")
        return 0

    else:
        print("TestCase ("+str(i)+") Succeeded ")
        return 1


def runAllTestCases():
    print("Hello Python")

    passed=[]
    failed=[]

    i=1
    for filename in os.listdir(path_of_the_directory):
        print(filename)
        # TestCase1 TestCase2
        test_case_res=runTestCase(i)
        if(test_case_res):
            passed.append(i)
        else:
            failed.append(i)

        i=i+1 #new TestCase
    
    print("*************Results****************")
    if(len(passed)==i-1):
        print("All TestCases Passed ")
    
    else:
        print("Test Cases Succeeded ",passed)
        
        print("Test Cases Failed ",failed)
            
    return None


def main(argv):

    argumentList =argv

    # Options :=requires argument
    options = "aij"
    
    # Long options
    # long_options = ["all", "My_file", "Output="]
    long_options = ["all","large","small"]

    
    try:
        # Parsing argument
        arguments, values = getopt.getopt(argumentList, options, long_options)
        # print(arguments)
        # print(values)

        
        # checking for all argument
        for currentArgument, currentValue in arguments:
            # print(currentArgument)
    
            if currentArgument in ("-a", "--all"):
                print("Running All TestCases")
                runAllTestCases()
                return
            
            if currentArgument in ("-i", "--large"):
                i=values[0]
                print("Running Test Case "+i)
                runTestCase(int(i))
                return

            if currentArgument in ("-j", "--small"):
                i=values[0]
                j=values[1]
                print("Running Test Case "+i+"-"+j)
                runTestCase_minor(int(i),int(j))
                return
    
                
    except getopt.error as err:
    # output error, and    return with an error code
             print (str(err))

if __name__ == "__main__":
    main(sys.argv[1:])