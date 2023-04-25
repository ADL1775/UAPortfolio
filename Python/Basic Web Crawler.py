'''
Script Purpose: Week Six Assignment 9 - Extract information from a web page
Script Version: 1.0 February 2023
Script Author:  Adam Livingston, University of Arizona

Script Revision History:
Version 1.0 February 2023, Python 3.x
'''

'''
Using the sample scripts below as a guide along with this weeks lecture you are to create a script that will extract the following information from a web-page.  
You will be using the Python Standard Library requests and the 3rd Party Library BeautifulSoup4 along with other Python standard libraries as necessary.  

For the web-page, you will extract the following information:
-page-title
-page-links URLs
-images found on the page

'''
import os                       # Python Standard Library : Operating System Methods
import sys                      # Python Standard Library : System Methods
import re
import requests
from bs4 import BeautifulSoup           # 3rd Party BeautifulSoup Library - pip install Beautifulsoup4


SCRIPT_NAME    = "Week Six Assignment 9 - Extract information from a web page"
SCRIPT_VERSION = "Version 1.0"
SCRIPT_AUTHOR  = "Author: Adam Livingston"

baseUrl = "https://casl.website/"
baseDomain = "https://casl.website"
mustInclude = "casl"

pageImages = set()
pageLinks = set()
titleSet = set()


def RecurseURL(newURL, base, local):
    try:
        page = requests.get(newURL)                         # retrieve a page from your favorite website
        soup = BeautifulSoup(page.text, 'html.parser')      # convert the page into soup

        links = soup.findAll('a')       # Find all the possible links
        images = soup.findAll('img')    # Find all possible images
        
        titleSet.add(soup.title.string)
        
        if images:
            for eachImage in images:
                try:
                    imgURL = eachImage['src']
                    #print(imgURL)
                    if imgURL[0:4] != 'http':       # If URL path is relative
                        imgURL = baseDomain+imgURL         # try prepending the base url
                    response = requests.get(imgURL)                 # Get the image from the URL
                    imageName = os.path.basename(imgURL)
                    pageImages.add(imageName)
                    
                    # SAVES IMAGES TO PROGRAM DIRECTORY
                    with open(imageName, 'wb') as outFile:
                        outFile.write(response.content)
                    
                    
                except Exception as err:
                    print(imgURL, err)
                    continue
        
        if links:
            for eachLink in links:
                
                newLink = eachLink.get('href') 
                
                if not newLink:
                    continue
                
                if 'http' not in newLink:
                    newLink = base+newLink
                    
                if not local in newLink:
                    continue   
                
                if newLink not in pageLinks: 
                    # verify this is a true new link
                    pageLinks.add(newLink)              # add the link to our set of unique links 
                    RecurseURL(newLink, base, local)           # Process this link
                else:
                    continue
                    
    except Exception as err:
        # display any errors that we encounter
        print(err)
    
    '''
    print("Images on the page: ")
    for eachImage in pageImages:
        print(eachImage)
        
    print("\nImage URLs: ")
    for eachImage in imgListURL:
        print(eachImage)
    
    print("\neachPage URL AND TITLE")
    for eachPage in pageLinks:
        try:           
            linkURL = eachPage['href']
            #
            if linkURL[0:4] != 'http':
                linkURL = url+linkURL
                pageURLS.append(linkURL)
                nxtSib = soup.fetchNextSiblings(linkURL)
                print(nextSib)
            
            
            #print("Link Title: ", linkTitle)
            #print("Link URL: ", linkURL)
        
        except Exception as err:
            continue            
        
        try:
            titleCheck = eachPage['title']
            titleList.append(titleCheck)
            
            
        except Exception as err:
            continue
        
    '''
        
    '''
    print("\nPRINTING eachPage")
    for eachPage in pageLinks:
        print(eachPage)
        print("\n")
    '''
if __name__ == '__main__':
    print("\n" + SCRIPT_NAME)
    print(SCRIPT_VERSION)
    print(SCRIPT_AUTHOR + "\n")
    
    pageLinks.add(baseUrl)
    
    RecurseURL(baseUrl, baseDomain, mustInclude)  
    
    
    ''' PRINT THE INFORMATION '''
    print("PAGE LINKS: ")
    for link in pageLinks:
        print(link)    
    print("\nPAGE TITLES: ")
    for titles in titleSet:
        print(titles)
    print("\nIMAGE NAMES: ")
    for eachImg in pageImages:
        print(eachImg)
    
    print("\nScript End")