from __future__ import print_function

'''
Script Purpose: Week Three Assignment 4 - File Processing Object
Script Version: 1.0 February 2023
Script Author:  Adam Livingston, University of Arizona

Script Revision History:
Version 1.0 January 2023, Python 3.x

'''

''' Import Standard Libraries '''
import os
import sys

from binascii import hexlify
import time
import hashlib
''' Import 3rd Party Libraries'''

from prettytable import PrettyTable
from prettytable import DOUBLE_BORDER   # Format style

# Psuedo Constants
SCRIPT_NAME    = "Week Three Assignment 4 - File Processing Object"
SCRIPT_VERSION = "Version 1.0"
SCRIPT_AUTHOR  = "Author: Adam Livingston"
tbl = PrettyTable(['Path', 'FileSize(kb)', 'Hash', 'LastModified', 'FileHeader', 'Status', 'Error'])


def HashFile(filePath):
    ''' 
        function takes one input a valid filePath
        returns the hexdigest of the file
        or error 
    '''
    try:
        with open(filePath, 'rb') as fileToHash:
            fileContents = fileToHash.read()
            hashObj = hashlib.md5()
            hashObj.update(fileContents)
            digest = hashObj.hexdigest()
            return digest
    except Exception as err:
        return str(err)

class FileProcessor:
    ''' Class to Create User Dictionary'''

    def __init__(self, fileName): #, fileName, modTime, size, hashVal, header, status, error):
        ''' initialize instance attributes'''
        ''' Create object variables and Constants '''
        if os.path.isfile(fileName):
            
            return None
        else:
            print("Path is not a file")
        
        self.filePath         = ''
        
        # You will add code here to extract the following information
        # First, verify if the path is a file
        #if os.path.isfile(fileName):            
        self.lastModifiedTime = ''#modTime
        self.fileSize         = ''#size
        self.md5Hash          = ''#hashVal
        self.fileHeader       = ''#header
        self.status           = ''#status
        self.errorInfo        = ''#error
        

        
    def GetFileDetails(self, path): 
        self.md5Hash = HashFile(path)
        self.filePath = path
        try:         
            with open(path, 'rb') as target:           
                metaData              = os.stat(path)       # Use the stat method to obtain meta data.
                #print(metaData)
                
                ''' Modified Time '''
                timeLastModified = metaData.st_mtime        # Extract Mac Last-modified time.
                modified = time.strftime("%Y-%m-%d %H:%M:%S", time.gmtime(timeLastModified)) # Convert lastModified time
                self.lastModifiedTime = modified
                
                ''' File Size '''
                tempSize = metaData.st_size                     # Extract fileSize.
                convertSize = (tempSize / 1024)                 # convert to kb
                convertSize = round(convertSize, 1)             # Round to the tens decimal place
                
                #convertSize = '{:,}'.format(convertSize)        # Format with commas
                #convertSize += " kb"                            # Add "kb" to the end to show unit of measure
                
                self.fileSize = convertSize                     # Set the object's file size to the extracted file size.
                self.errorInfo = ''
                self.status = 'OK'
                
                data = target.read(20)
                fileHeader = hexlify(data) #hexlify(path)
                self.fileHeader = fileHeader
        
                

            '''
            We have the times, but the system's representation of the time isn't human-readable
            We need to convert the epoch times
            Take our modified time we already got, and convert it to utcTime
            '''
            
            
            #accessed = time.strftime("%Y-%m-%d %H:%M:%S", time.gmtime(timeLastAccess))   # Convert lastAccessed time
            #created = time.strftime("%Y-%m-%d %H:%M:%S", time.gmtime(timeCreated))       # Convert timeCreated time 
            
            #return True, None, fileSize, fileHash, modified#, metaData #accessed, created 
        
        # Error message if an exception happens, such as if a file's metadata could not be obtained.
        except Exception as err:
            self.status = 'ERROR'
            self.errorInfo = str(err)
            self.fileSize = 'Unk'
            self.lastModifiedTime = 'Unk'
            self.fileHeader = 'Unk'
            self.md5Hash = 'Unk'
            return #break
            #return False, str(err), None, None, None, None, None
    '''
    def GetFileHeader(self, path):
        with open(path, 'rb') as target:
'''
        #print("PLACEHOLDER GetFileHeader PLACEHOLDER")
    
    def PrintFileDetails(self):
        print(self.filePath, self.lastModifiedTime, self.fileSize, self.md5Hash, self.fileHeader)
        
def main():
    print("\n" + SCRIPT_NAME)
    print(SCRIPT_VERSION)
    print(SCRIPT_AUTHOR + "\n")

    while True:
        DIR = input("Enter Target Folder: ")    # Create variable "DIR", being the directory that the user inputs.
        if os.path.isdir(DIR):
            break
        else:
            # Error message if invalid directory is inputted by user for DIR variable.
            print("\nError: Invalid Directory. Please try again.\n")          
            continue 
    
    objList = list()
    i = 0
    #obj = FileProcessor()
    for root, dirs, files in os.walk(DIR):

        # Walk the path from top to bottom.
        # For each file obtain the absolute path 
        for nextFile in files:                          # Traverse file system from DIR and all other branching directories' files.       
            
            path = os.path.join(root, nextFile)         # Join user-inputted DIR with file names in that DIR.
            absPath = os.path.abspath(path)             # Return a normalized absolutized version of filePath.
            obj = FileProcessor(absPath)
            obj.GetFileDetails(absPath)
            #obj.GetFileHeader(absPath)
            objList.append(obj)
            
    
    for eachObj in objList:
        tbl.add_row([eachObj.filePath, eachObj.fileSize, eachObj.md5Hash, eachObj.lastModifiedTime, eachObj.fileHeader, eachObj.status, eachObj.errorInfo]) 
        
        #eachObj.PrintFileDetails()
    tbl.align = "l"
    tbl.set_style(DOUBLE_BORDER)
    tbl.sortby = "FileSize(kb)"
    tbl.reversesort = True
    tbl.title = title = "File Inventory"
    

    printTable = tbl.get_string()
    print(printTable)
    
    csvFile = tbl.get_csv_string(sortby = "FileSize(kb)", reversesort = True, title = "File Inventory")
    
    try:
        with open('livingstonA_WK3_prettytable.csv', 'w') as output:
            output.write(csvFile)
    except Exception as err:
        print(str(err))
        
    print("\nScript Complete")    
    '''
    #FileProcessor.GetFileDetails()
    success, errInfo, fileSize, fileHash, modified = GetFileDetails(absPath)
    #FileProcessor(absPath)                      # Pass the absPath into the FileProcessor class
    
    theFile = FileProcessor(absPath)
    '''
    
   
    # dirPath = input("Enter a valid directory path: ")
    ''' Process all the files in the path '''
        
if __name__ == '__main__':
    main()