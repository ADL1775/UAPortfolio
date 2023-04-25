'''
Script Purpose: Week Four Assignment 5 - File Processing Object
Script Version: 1.0 February 2023
Script Author:  Adam Livingston, University of Arizona

Script Revision History:
Version 1.0 January 2023, Python 3.x
'''

'''
Develop a script that:

1) Prompts the user for a directory path to search

2) Verify that the path provided exists and is a directory

3) Iterate through each file in that directory and examine it using PIL.
'''


''' LIBRARY IMPORT SECTION '''

import os                       # Python Standard Library : Operating System Methods
import sys                      # Python Standard Library : System Methods
from datetime import datetime   # Python Standard Libary datetime method from Standard Library
from prettytable import PrettyTable
from prettytable import DOUBLE_BORDER   # Format style

# import the Python Image Library 
# along with TAGS and GPS related TAGS
# Note you must install the PILLOW Module
# pip install PILLOW
from PIL import Image
from PIL.ExifTags import TAGS, GPSTAGS

# Psuedo Constants
SCRIPT_NAME    = "Week Four Assignment 5 - Image Processing"
SCRIPT_VERSION = "Version 1.0"
SCRIPT_AUTHOR  = "Author: Adam Livingston"
tbl = PrettyTable(['Image?', 'File', 'FileSize(kb)', 'Ext', 'Format', 'Width', 'Height', 'Type'])


def GetUserInput():
    while True:
        DIR = input("Enter Directory to Examine | or input Q to Quit: ")    # User specifies the directory.
        
        if DIR.lower() == 'q':  # If user enters "q" and just "q", then the program terminates. Capitalization doesn't matter.
            print("\nPROGRAM TERMINATED")
            sys.exit()
        elif os.path.isdir(DIR):
            return DIR
        else:
            # Error message if invalid directory is inputted by user for DIR variable.
            print("\nError: Invalid Directory. Please try again.\n")          
            continue    



def main():
    print("\n" + SCRIPT_NAME)
    print(SCRIPT_VERSION)
    print(SCRIPT_AUTHOR + "\n")

    DIR = GetUserInput()
    
    fileList = os.listdir(DIR) 

    for eachFile in fileList:
        path = os.path.join(DIR, eachFile) # Join user-inputted DIR with file names in that DIR.

        if os.path.isfile(path): 
            data = os.stat(path)
            fileSize = data.st_size / 1024
            fileSize = round(fileSize, 1)
            fileSize = '{:,}'.format(fileSize)
            ext = os.path.splitext(path)[1] # Isolate the file extension of the path in variable ext (like .jpg, .txt, etc.)

            try:
                with Image.open(path) as im:    # Opens and identifies the given image file.
                    tbl.add_row(['Yes', path, fileSize, ext, im.format, im.width, im.height, im.mode])

            # What if the file is not a photo?
            # Add a line that states doesn't display the nonexistent photo format, width, height, or mode data.
            except Exception as err:
                tbl.add_row(['No', path, fileSize, ext, "[N/A]", "[N/A]", "[N/A]", "[N/A]"])
                pass
        else:
            continue    

        tbl.align = "l"
        tbl.set_style(DOUBLE_BORDER)
    tbl.sortby = "FileSize(kb)"
    tbl.reversesort = True
    tbl.title = title = "Image Details"    
    printTable = tbl.get_string()
    print(printTable)


if __name__ == '__main__':
    main()
    print("\nScript End")