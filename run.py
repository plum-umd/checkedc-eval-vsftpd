#!/usr/bin/python3

# This file contains a script that will run the conversion tools on all files in this directory 
# replace them, and then diff the output. All the programmer needs to do is run this script

import os 
import sys
import fileinput
import subprocess

def update_script_locations(cconv_loc, include_loc, python_loc): 
    for line in fileinput.input("convert_all.sh", inplace=1):
        if "CCONV=" in line:
            line = "CCONV=" + cconv_loc
        elif "INCLUDES=" in line: 
            line = "INCUDES=" + include_loc
        sys.stdout.write(line) 
    for line in fileinput.input("update_database.py", inplace=1):
        if "#!" in line: 
            line = "#!" + python_loc 
        sys.stdout.write(line)
    for line in fileinput.input("replace.py", inplace=1):
        if "#!" in line: 
            line = "#!" + python_loc 
        sys.stdout.write(line)


def run_diff_command(command, filename): 
    os.system(command)  
    addon = "" 
    if filename: 
        addon = "for the file {}".format(filename)
    file = open("GIT_DIFF_GENERATOR.txt", "r") 
    empty = True 
    for line in file.readlines(): 
        if(empty): 
            empty = False 
            print("\nThe following diffs were generated " + addon + "\n\n") 
        print(line) 
    if empty: 
        print("No diffs to report!")
    os.system("rm GIT_DIFF_GENERATOR.txt")


def run_file_by_file(): 
    f = []
    for file in os.listdir("./"): 
        if file.endswith(".c"):
            f.extend(file)
    for filename in f: 
        command = "git diff baseline-converted..baseline --output=GIT_DIFF_GENERATOR.txt -- {}".format(filename) 
        run_diff_command(command, filename)


# asking the user information about their path locations
print("Please provide the path to the cconv-standalone tool:") 
cconv_loc = input("[CCONV LOCATION] > ") + "\n"
print("Please provide the path to the checkedc include directory:")
include_loc = input("[INCLUDE LOCATION] > ") + "\n"
print("Please provide the path to checked c's clang:")
clang_loc = input("[CLANG LOCATION] > ") + "\n"
print("Please provide the path to python (which python3 in a new tab)")
python_loc = input("[PYTHON LOCATION] > ") + "\n"

print("##########\tupdating scripts with new locations...\t##########")

update_script_locations(cconv_loc, include_loc, python_loc)

print("##########\tupdating the database with your locations...\t##########")
os.system("python3 update_database.py") 

print("##########\trunning the conversion tool...\t##########")
os.system("sh convert_all.sh")

print("##########\treplacing the output\t##########")
os.system("python replace.py")

compiles = True
print("##########\tcompiling...\t##########")
out = subprocess.Popen(['make', 'CC={}'.format(clang_loc), '-i'], stdout=subprocess.PIPE, stderr=subprocess.STDOUT) 
stdout, stderr = out.communicate()
stdout = str(stdout) 
if "error:" in stdout: 
    compiles = False
if not compiles: 
    print("\nFailure to compile!\n")
os.system("rm *.o")  


# reset everything 
os.system("git reset --HARD") 
update_script_locations(cconv_loc, include_loc, python_loc) 
for line in fileinput.input("convert_all.sh", inplace=1):
    if "output-postfix" in line: 
        line = "-output-postfix=checked \\\n-alltypes \\"
    sys.stdout.write(line) 
print("##########\tupdating the database with your locations...\t##########")
os.system("python3 update_database.py") 

print("##########\trunning the conversion tool...\t##########")
os.system("sh convert_all.sh")

print("##########\treplacing the output\t##########")
os.system("python replace.py")



### Interactive version of the diff checker for regressions : support coming soon
menu = """
####################################################################################
Press
[1]\tfor a raw diff between the latest conversion and your conversion
[2]\tfor a diff that goes through the files individually (skips files without diffs)
[<filename>]\tif you'd like to view a diff for a specific file
[q]\tto quit
####################################################################################
"""
print(menu)
choice = input("[CHOICE] > ").rstrip()
if choice=="1": 
    command = "git diff baseline-converted..baseline --output=GIT_DIFF_GENERATOR.txt -- '*.c'" 
    run_diff_command(command, None)
elif choice=="2": 
    run_file_by_file()
elif choice=="q": 
    print("All done!")