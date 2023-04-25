'''
Script Purpose: Week 14 Assignment 19 - Virus Total
Script Version: 1.0 April 2023
Script Author:  Adam Livingston, University of Arizona

For this week's assignment, you will be creating a simple VirusTotal Client.

1. You will extract the files provided in the scanFiles.zip, into a directory named: TARGET

2. Your script will process each file in the TARGET folder.  In other words you will generate an md5 hash for each file. 

3. Your script will submit each hash value to VirusTotal for analysis

4. Your script will generate a simple report with the results provided by VirusTotal for each file.

You will submit your script and the resulting report.


'''

import json
import hashlib
import sys
import os
from virus_total_apis import PublicApi as VirusTotalPublicApi
from prettytable import PrettyTable

# You will need to obtain an API Key from Virus Total
API_KEY = '5ba8c025b3190f67f3867d514c30ada5727d2e01368b3c26284e9ac1fa3fd714'

SAMPLE = b"X5O!P%@AP[4\PZX54(P^)7CC)7}$EICAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*"
SAMPLE_MD5 = hashlib.md5(SAMPLE).hexdigest()

tbl = PrettyTable(['FileName', 'MD5Hash', 'PositiveHits', 'TotalVendors'])

hashDict = {}
objList = []
targetDir = "./TARGET"

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



def main():
    VTList = []
    vt = VirusTotalPublicApi(API_KEY)
    SCRIPT_NAME    = "Week 14 Assignment 19 - Virus Total"
    SCRIPT_VERSION = "Version 1.0"
    SCRIPT_AUTHOR  = "Author: Adam Livingston"
    
    print("\n" + SCRIPT_NAME)
    print(SCRIPT_VERSION)
    print(SCRIPT_AUTHOR + "\n")
          
    for root, dirs, files in os.walk(targetDir):
        for nextFile in files:                          
            path = os.path.join(root, nextFile)         # Join user-inputted DIR with file names in that DIR.
            absPath = os.path.abspath(path)             # Return a normalized absolutized version of filePath.
            theHash = HashFile(absPath)
            hashDict[nextFile] = {"MD5 Hash": theHash,
                                  "Positives": "",
                                  "Total": ""}
                                  
            
    
    for key, value in hashDict.items():
        hitTotal = 0
        posTotal = 0         
        for key2, val2 in value.items():
            if key2 == "MD5 Hash":
                response = vt.get_file_report(val2)
                for key3, val3 in response.items():
                    if key3 == "results":                        
                        for key4, val4 in val3.items():                    
                            if key4 == "total":
                                value["Total"] = val4
                            elif key4 == "positives":
                                value["Positives"] = val4
                            else:
                                continue
                    
    for key, value in hashDict.items():
        newList = [key]
        for key2, val2 in value.items():
            newList.append(val2)
        tbl.add_row([newList[0], newList[1], newList[2], newList[3]])
    #tbl = PrettyTable(['FileName', 'MD5Hash', 'PositiveHits', 'TotalVendors'])
    
    print(tbl)

if __name__ == '__main__':
    main()