'''
Script Purpose: Week Five Assignment 7 - Email and URL Processing
Script Version: 1.0 February 2023
Script Author:  Adam Livingston, University of Arizona

Script Revision History:
Version 1.0 January 2023, Python 3.x
'''

'''
Develop a script that:

1) Copy the memory dump to the virtual desktop environment persistent storage area.

2) Develop a python script and regular expressions to extract and report ALL the e-mail and urls found in the memory dump.
'''


''' LIBRARY IMPORT SECTION '''

import os                       # Python Standard Library : Operating System Methods
import sys                      # Python Standard Library : System Methods
import re
from prettytable import PrettyTable
from prettytable import DOUBLE_BORDER   # Format style
from binascii import hexlify


# Psuedo Constants
SCRIPT_NAME    = "Week Five Assignment 7 - Email and URL Processing"
SCRIPT_VERSION = "Version 1.0"
SCRIPT_AUTHOR  = "Author: Adam Livingston"
tblURL = PrettyTable(["Occurrences","URL"])
tblEMAIL = PrettyTable(["Occurrences","Email"])

# File Chunk Size
CHUNK =  4096

# regular expressions
ePatt = re.compile(b'[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,4}')
uPatt = re.compile(b'\w+:\/\/[\w@][\w.:@]+\/?[\w.\.?=%&=\-@$,]*')

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
    
    urlDict   = {}
    emailDict = {}
    try:
        with open(FILE, 'rb') as targetFile:
            overlap = b''
            while True:
                fileChunk = targetFile.read(CHUNK)
                
                if fileChunk:
                    
                    fileChunk = fileChunk + overlap
                    
                    urlMatches = uPatt.findall(fileChunk)
                    emailMatches = ePatt.findall(fileChunk)
                    
                    for eachUrl in urlMatches:
                        eachUrl = eachUrl.decode("utf-8") 
                        try:
                            ucnt = urlDict[eachUrl]
                            ucnt += 1
                            urlDict[eachUrl] = ucnt
                        except:
                            urlDict[eachUrl] = 1
                            
                    for eachEmail in emailMatches:
                        eachEmail = eachEmail.decode("utf-8")
                        try:
                            ecnt = emailDict[eachEmail]
                            ecnt += 1
                            emailDict[eachEmail] = ecnt
                        except:
                            emailDict[eachEmail] = 1                        
                    overlap   = fileChunk[-20:]
                    
                else:         
                    break
            
            for url, cnt in urlDict.items():
                tblURL.add_row([cnt, url])
            for email, cnt in emailDict.items():
                tblEMAIL.add_row([cnt, email])
            
            print("\nGenerating Sorted Result Tables")
            tblURL.align = "l"
            tblURL.set_style(DOUBLE_BORDER)
            tblURL.title = "Sorted URL Results"
            tblEMAIL.align = "l"
            tblEMAIL.set_style(DOUBLE_BORDER)
            tblEMAIL.title = "Sorted Email Results"            
            print(tblURL.get_string(sortby="Occurrences", reversesort=True))
            print("\n\n")
            print(tblEMAIL.get_string(sortby="Occurrences", reversesort=True))
            
                
            
    except Exception as err:
        print(err)    
    

if __name__ == '__main__':
    main()
    print("\nScript End")