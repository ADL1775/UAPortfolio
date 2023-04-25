'''
Script Purpose: Week Four Assignment 6 - EXIF Data Acquistion
Script Version: 1.0 February 2023
Script Author:  Adam Livingston, University of Arizona

Script Revision History:
Version 1.0 January 2023, Python 3.x
'''


'''
Using this script provide as a baseline.
Expand the script as follows:

1) Allow the user to enter a path to a directory containing jpeg files.
2) Using that path, process all the .jpg files contained in that folder  (use the testimages.zip set of images)
3) Extract the GPS Coordinates for each jpg (if they exist) and then report the results in a prettytable like this

+------------------------------+--------------------+---------------------+---------------------+-------------------+---------------+
|          File-Name           |        Lat         |         Lon         |      TimeStamp      |        Make       |     Model     |
+------------------------------+--------------------+---------------------+---------------------+-------------------+---------------+
|    ./TST-IMAGES\barn.jpg     |     42.501235      |  -83.25066333333334 | 2009:03:14 13:46:34 |       NIKON       | COOLPIX P6000 |
| ./TST-IMAGES\kinderscout.jpg | 53.40716666666667  | -1.9036666666666666 | 2008:01:12 12:06:52 | NIKON CORPORATION |   NIKON D50   |
|   ./TST-IMAGES\Turtle.jpg    | 25.338400000002647 |  34.73966599999203  | 2008:05:08 16:55:58 |       Canon       |  Canon EOS 5D |
+------------------------------+--------------------+---------------------+---------------------+-------------------+---------------+

4) Map the GPS Coordinates along with the name of the file 

NOTE: There are several ways to do this, however, the easiest method would be to use the MapMaker App, at https;//mapmakerapp.com/
      you can either manually enter the lat/lon values your script generates or you can place your results in a CSV file and upload
      the data to the map.

'''
# Usage Example:
# python Assignment 6
#
# Requirement: Python 3.x
#
# Requirement: 3rd Party Library that is utilized is: PILLOW
#                   pip install PILLOW  from the command line
#                   this is already installed in the Virtual Desktop


''' LIBRARY IMPORT SECTION '''

import os                       # Python Standard Library : Operating System Methods
import sys                      # Python Standard Library : System Methods
from datetime import datetime   # Python Standard Libary datetime method from Standard Library

# import the Python Image Library 
# along with TAGS and GPS related TAGS
# Note you must install the PILLOW Module
# pip install PILLOW

from PIL import Image
from PIL.ExifTags import TAGS, GPSTAGS


# import the prettytable library
from prettytable import PrettyTable
from prettytable import DOUBLE_BORDER   # Format style


SCRIPT_NAME    = "Week Four Assignment 6 - EXIF Data Acquistion"
SCRIPT_VERSION = "Version 1.0"
SCRIPT_AUTHOR  = "Author: Adam Livingston"
tbl = PrettyTable(['File-Name', 'Lat', 'Long', 'TimeStamp', 'Camera-Make', 'Camera-Model'])

def ExtractGPSDictionary(fileName):
    ''' Function to Extract GPS Dictionary '''
    try:
        pilImage = Image.open(fileName)
        exifData = pilImage._getexif()

    except Exception:
        # If exception occurs from PIL processing
        # Report the 
        return None, None

    # Interate through the exifData
    # Searching for GPS Tags

    imageTimeStamp = "NA"
    cameraModel = "NA"
    cameraMake = "NA"
    gpsData = False

    gpsDictionary = {}

    if exifData:

        for tag, theValue in exifData.items():

            # obtain the tag
            tagValue = TAGS.get(tag, tag)
            #print(tagValue)
            # Collect basic image data if available

            if tagValue == 'DateTimeOriginal':
                imageTimeStamp = exifData.get(tag).strip()

            if tagValue == "Make":
                cameraMake = exifData.get(tag).strip()

            if tagValue == 'Model':
                cameraModel = exifData.get(tag).strip()

            # check the tag for GPS
            if tagValue == "GPSInfo":

                gpsData = True;

                # Found it !
                # Now create a Dictionary to hold the GPS Data

                # Loop through the GPS Information
                for curTag in theValue:
                    gpsTag = GPSTAGS.get(curTag, curTag)
                    gpsDictionary[gpsTag] = theValue[curTag]

        basicExifData = [imageTimeStamp, cameraMake, cameraModel]    

        return gpsDictionary, basicExifData

    else:
        return None, None

# End ExtractGPSDictionary ============================


def ExtractLatLon(gps):
    ''' Function to Extract Lattitude and Longitude Values '''

    # to perform the calcuation we need at least
    # lat, lon, latRef and lonRef
    
    try:
        latitude     = gps["GPSLatitude"]
        latitudeRef  = gps["GPSLatitudeRef"]
        longitude    = gps["GPSLongitude"]
        longitudeRef = gps["GPSLongitudeRef"]

        lat, lon = ConvertToDegreesV1(latitude, latitudeRef, longitude, longitudeRef)

        gpsCoor = {"Lat": lat, "LatRef":latitudeRef, "Lon": lon, "LonRef": longitudeRef}

        return gpsCoor

    except Exception as err:
        return None

# End Extract Lat Lon ==============================================


def ConvertToDegreesV1(lat, latRef, lon, lonRef):
    
    degrees = lat[0]
    minutes = lat[1]
    seconds = lat[2]
    try:
        seconds = float(seconds)
    except:
        seconds = 0.0

    latDecimal = float ( (degrees +(minutes/60) + (seconds)/(60*60) ) )
        
    if latRef == 'S':
        latDecimal = latDecimal*-1.0
        
    degrees = lon[0]
    minutes = lon[1]
    seconds = lon[2]
    try:
        seconds = float(seconds)
    except:
        seconds = 0.0
    
    lonDecimal = float ( (degrees +(minutes/60) + (seconds)/(60*60) ) )
    
    if lonRef == 'W':
        lonDecimal = lonDecimal*-1.0
    
    return(latDecimal, lonDecimal)


''' MAIN PROGRAM ENTRY SECTION '''



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

if __name__ == "__main__":
    '''
    pyExif Main Entry Point
    '''
    print("\n" + SCRIPT_NAME)
    print(SCRIPT_VERSION)
    print(SCRIPT_AUTHOR + "\n")    
    print("\nExtract EXIF Data from JPEG Files")
    
    print("Script Started", str(datetime.now()))
    print()
    
    DIR = GetUserInput()        # Call the "GetUserInput" function
    
    fileList = os.listdir(DIR)  # Make a list of all the files in the directory
    
    
    ''' PROCESS EACH JPEG FILE SECTION '''
    
    for eachFile in fileList:
        path = os.path.join(DIR, eachFile) # Join user-inputted DIR with file names in that DIR.
    
        #latLonList = []
        #targetFile = "barn.jpg"                 # file must be located in the same folder
        if os.path.isfile(path):
            gpsDictionary, exifList = ExtractGPSDictionary(path)

            if exifList:
                TS = exifList[0]
                MAKE = exifList[1]
                MODEL = exifList[2]
            
            else:
                TS = 'NA'
                MAKE = 'NA'
                MODEL = 'NA'
            '''
            print("Photo Details")
            print("-------------")
            print("TimeStamp:    ", TS)
            print("Camera Make:  ", MAKE)
            print("Camera Model: ", MODEL)
            '''
            if (gpsDictionary != None):
    
                # Obtain the Lat Lon values from the gpsDictionary
                # Converted to degrees
                # The return value is a dictionary key value pairs
    
                dCoor = ExtractLatLon(gpsDictionary)
    
                #print("\nGeo-Location Data")
                #print("-----------------")
    
                if dCoor:
                    lat = dCoor.get("Lat")
                    latRef = dCoor.get("LatRef")
                    lon = dCoor.get("Lon")
                    lonRef = dCoor.get("LonRef")
                    
                    if ( lat and lon and latRef and lonRef):
                        #print("Lattitude: ", '{:4.4f}'.format(lat))
                        #print("Longitude: ", '{:4.4f}'.format(lon))
                        
                        tbl.add_row([path, '{:4.4f}'.format(lat), '{:4.4f}'.format(lon), TS, MAKE, MODEL])
                        
                    else:
                        #print("WARNING No GPS EXIF Data")
                        tbl.add_row([path, 'No GPS EXIF Data', 'No GPS EXIF Data', TS, MAKE, MODEL])
                else:
                    #print("WARNING No GPS EXIF Data")
                    tbl.add_row([path, 'No GPS EXIF Data', 'No GPS EXIF Data', TS, MAKE, MODEL])
            else:
                #print("WARNING", " not a valid file", targetFile)
                tbl.add_row([path, 'N/A', 'N/A', TS, MAKE, MODEL])
        tbl.align = "l"
        tbl.set_style(DOUBLE_BORDER)
    tbl.title = title = "Image Details"
    print(tbl.get_string())
    
    
    with open("livingstonA_WK4-6_CSV.csv", 'w') as outFile:
        csvString = tbl.get_csv_string()
        outFile.write(csvString)       
    print("\n\nCheck the root directory of this .py program for a .csv file to upload to mapmaker.")
    print("\n\nScript end.\n")