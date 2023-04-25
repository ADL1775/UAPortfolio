'''
Script Purpose: Week Five Assignment 8 - Unique String Search
Script Version: 1.0 February 2023
Script Author:  Adam Livingston, University of Arizona

Script Revision History:
Version 1.0 January 2023, Python 3.x
'''

'''
Develop a script that:

1) Processes the memory dump and identify unique strings of 5-12 characters along with the number of occurrences of each unique string.   

2) Then display the resulting list of strings and occurrences in a prettytable sorted by the highest number of occurrences. 
'''


''' LIBRARY IMPORT SECTION '''

import os                       # Python Standard Library : Operating System Methods
import sys                      # Python Standard Library : System Methods
import re
from prettytable import PrettyTable
from prettytable import DOUBLE_BORDER   # Format style
from binascii import hexlify


# Psuedo Constants
SCRIPT_NAME    = "Week Five Assignment 8 - Unique String Search"
SCRIPT_VERSION = "Version 1.0"
SCRIPT_AUTHOR  = "Author: Adam Livingston"

tbl = PrettyTable(["Occurrences","String"])

# File Chunk Size
CHUNK =  4096

# regular expressions
wPatt = re.compile(b'[a-zA-Z]{5,15}')

def GetUserInput():
    while True:
        FILE = input("Enter File to Examine | or input Q to Quit: ")    # User specifies the directory.
        
        if FILE.lower() == 'q':  # If user enters "q" and just "q", then the program terminates. Capitalization doesn't matter.
            print("\nPROGRAM TERMINATED")
            sys.exit()
        elif os.path.isfile(FILE):
            return FILE
        else:
            # Error message if invalid directory is inputted by user for FILE variable.
            print("\nError: Invalid File. Please try again.\n")          
            continue    



def main():
    print("\n" + SCRIPT_NAME)
    print(SCRIPT_VERSION)
    print(SCRIPT_AUTHOR + "\n")

    FILE = GetUserInput()
    
    stringDict = {}
    
    try:
        with open(FILE, 'rb') as targetFile:
            overlap = b''
            while True:
                fileChunk = targetFile.read(CHUNK)
                
                if fileChunk:
                    
                    fileChunk = fileChunk + overlap
                    
                    strMatches = wPatt.findall(fileChunk)
                    
                    for eachStr in strMatches:
                        eachStr = eachStr.decode("utf-8") 
                        try:
                            cnt = stringDict[eachStr]
                            cnt += 1
                            stringDict[eachStr] = cnt
                        except:
                            stringDict[eachStr] = 1
                                 
                    overlap   = fileChunk[-20:]
                    
                else:         
                    break
            totCount = 0
            for string, cnt in stringDict.items():
                tbl.add_row([cnt, string])
                totCount += 1
                if totCount == 50:
                    break
            
            
            print("\nGenerating Sorted Result Table")
            tbl.align = "l"
            tbl.set_style(DOUBLE_BORDER)
            tbl.title = "Sorted String Results (Top 50) "


            print(tbl.get_string(sortby="Occurrences", reversesort=True))
            
            print("Total number of unique strings found: ", len(stringDict))

    except Exception as err:
        print(err)    
    

if __name__ == '__main__':
    main()
    
    print("\nScript End")