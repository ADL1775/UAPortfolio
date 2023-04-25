'''
Script Purpose: Week Ten Assignment 14 - Image Embedding
Script Version: 1.0 April 2023
Script Author:  Adam Livingston, University of Arizona

After participating in this weeks live webinar, employ the concepts provided to create simple covert communication capability within the true color image provided.  You will be using the Python Image Library (PIL),  the true color bmp file provided and the simple code book provided here.

Submit:

1) Your Python Script

2) Your image embedded with the hidden content.

(note your submitted image must be of the same type and size as the original image)

'''

''' 3rd Party Library '''
from PIL import Image               # pip install pillow
import random

'''
CODE BOOK
000 Dead Drop @
001 Corner of Lexington Ave and E 48th Street
010 Corner of Madison Ave and E 34th Street
011 Drop Package in Potted Plant outside Wells Fargo
100 Drop Package in Gold Gargage Can
101 12 PM Sharp
110 7 AM Sharp
111 Abort if you see a Red Rose
'''
codeDead = [0,0,0]
codePlace = [[0,0,1],[0,1,0]]
codeDrop = [[0,1,1],[1,0,0]]
codeTime = [[1,0,1],[1,1,0]]
codeAbort = [1,1,1]

# Pixel tuple index
RED   = 0
GREEN = 1
BLUE  = 2

width = 0
height = 0

chosen_pixels = list()

SCRIPT_NAME    = "Week Ten Assignment 14 - Image Embedding"
SCRIPT_VERSION = "Version 1.0"
SCRIPT_AUTHOR  = "Author: Adam Livingston"

''' Obtain the Basic image information '''

'''
Image Orientation

         |
         |
y (rows) |
         |
         |     
         -----------------------------
                     x (columns)
'''
# Purpose of this function is to check the code book entries with the least-significant bit values of the chosen pixel.
# If the pixel cannot support the code book entry, then a new pixel must be chosen.
def CheckCode(pixCompare, codeList):
    retVal = False
    if len(codeList) == 3:
        if pixCompare == codeList:
            retVal = False
        else:
            retVal = True
    
    else:
        for sublist in codeList:
            if sublist == pixCompare:
                retVal = False
                break
            else:
                retVal = True
    
    return retVal


# Purpose of this function is to choose a pixel on the image that
# modifiable RGB values in the least significant bit. 
def ChoosePix(pix, img):
    j = 0
    i = 0
    while i < 5: 
        samePixels = False
        checkPixels = False
        xChoice = 0
        yChoice = 0        
        xChoice = random.randint(0, width - 1)
        yChoice = random.randint(0, height - 1)
        entry = [xChoice, yChoice]
        
        pixel = pix[xChoice, xChoice]
        redPx = pixel[RED]      # Extract the RGB
        grnPx = pixel[GREEN]
        bluPx = pixel[BLUE]      
        '''
        print("\nOrginal Pixel")
        print("RED: ", '{:08b}'.format(redPx))
        print("GRN: ", '{:08b}'.format(grnPx))
        print("BLU: ", '{:08b}'.format(bluPx))
        '''
        # Store the least-significant bit of each RGB value in a new list
        pixCompare = [(redPx & 1), (grnPx & 1), (bluPx & 1)]
        
        if i == 0:
            checkPixels = CheckCode(pixCompare, codeDead)
            j += 1
            if checkPixels == False:
                continue
            else:
                print(f"The first pixel was found after {j} tries!")
        elif i == 1:
            checkPixels = CheckCode(pixCompare, codePlace)
        
        elif i == 2:
            checkPixels = CheckCode(pixCompare, codeDrop)
            
        elif i == 3:
            checkPixels = CheckCode(pixCompare, codeTime)
            
        else:
            checkPixels = CheckCode(pixCompare, codeAbort)
            
        if checkPixels == True:
            for sublist in chosen_pixels:
                if sublist == entry:
                    samePixels = True
        else:
            continue
        if samePixels == False:
            chosen_pixels.append(entry)
            i += 1
    return
    

def ModPix(pix):
    i = 0
    while i < 5:
        c = 0
        r = 0
        randIndex = random.choice(range(len(chosen_pixels)))
        c, r = thePixels = chosen_pixels.pop(randIndex)
        
        # Read the Pixel
        pixel = pix[c, r]
        
        redPx = pixel[RED]      # Extract the RGB
        grnPx = pixel[GREEN]
        bluPx = pixel[BLUE]
        
        
        '''
        print("\nOrginal Pixel")
        print("RED: ", '{:08b}'.format(redPx))
        print("GRN: ", '{:08b}'.format(grnPx))
        print("BLU: ", '{:08b}'.format(bluPx))
        '''
        if i == 0:
            hide = [0,0,0]  # The initial message
        elif i == 1:
            hideIndex = random.choice(range(len(codePlace)))
            hide = codePlace[hideIndex]
        elif i == 2:
            hideIndex = random.choice(range(len(codeDrop)))
            hide = codeDrop[hideIndex]
        elif i == 3:
            hideIndex = random.choice(range(len(codeTime)))
            hide = codeTime[hideIndex]
        else:
            hide = codeAbort
        
        if hide[0] == 0:
            redPx = redPx & 0b11111110
        else:
            redPx = redPx | 0b00000001   
            
        if hide[1] == 0:
            grnPx = grnPx & 0b11111110
        else:
            grnPx = grnPx | 0b00000001       
            
        if hide[2] == 0:
            bluPx = bluPx & 0b11111110
        else:
            bluPx = bluPx | 0b00000001         
            # Print the New Value of the Pixel
        '''
        print("\nAlterned Pixel")
        print("RED: ", '{:08b}'.format(redPx))
        print("GRN: ", '{:08b}'.format(grnPx))
        print("BLU: ", '{:08b}'.format(bluPx))
        '''
        pixel = (redPx, grnPx, bluPx)
        pix[c,r] = pixel
        i += 1
        
    return pix

try:
    print("\n" + SCRIPT_NAME)
    print(SCRIPT_VERSION)
    print(SCRIPT_AUTHOR + "\n")    
    
    img = Image.open('monalisa.bmp')   
    
    pix = img.load()  
    
    width, height = img.size
    
    totPixels = width * height
    print("Total pixels in the image: ", totPixels)
    
    ChoosePix(pix, img)
    pix = ModPix(pix)
    img.save('livingstonA_Wk10_image.bmp')
    
    
    print("\nSingle Pixel Steganography Done")

except Exception as err:
    print("Steg Failed: ", str(err))