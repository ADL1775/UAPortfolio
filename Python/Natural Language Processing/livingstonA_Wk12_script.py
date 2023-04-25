'''
Script Purpose: Week 12 Assignment 15 - Natural Language Processing
Script Version: 1.0 April 2023
Script Author:  Adam Livingston, University of Arizona

This week you will be utilizing the Python 3rd Party Natural Language Tookit to analyze and extract information from the Trump Indictment

The Indictment is provided below, you will need to create a folder and include the indictment file in that folder.

You will develop a script that will:

1) import the proper nltk libraries

2) Initialize the Corpus

Provide a selection loop for the following

Print the Corpus Length
Print the number of word Tokens found
Print the size of the vocabulary

Print the occurrences of specific test words:
    COUNT, JURY, INTENT, COMMIT, DEFRAUD and other words of interest.

Print a word concordance: using the same test words.

Print a word index: using the same test words

Create a Prettytable with each word and the number of occurrences.  
Words included must be at least 4 characters in length, and include only ascii characters.
Submit:

1) Your final Python script

2) a Transcript of your output

'''
import os       # Standard Library OS functions
import sys
import logging  # Standard Library Logging functions
import nltk     # Import the Natural Language Toolkit
from nltk.corpus import PlaintextCorpusReader   #Import the PlainTextCorpusReader Module
from nltk.corpus import stopwords
from time import sleep
from prettytable import PrettyTable

stopSet = set(stopwords.words('english'))
SCRIPT_NAME    = "Week 12 Assignment 15 - Natural Language Processing"
SCRIPT_VERSION = "Version 1.0"
SCRIPT_AUTHOR  = "Author: Adam Livingston"   

WORDLIST = ["COUNT", "JURY", "INTENT", "COMMIT", "DEFRAUD"]

class classNLTKQuery:
    
    def __init__(self, thePath):
        
        # Validate the path is a directory
        if not os.path.isdir(thePath):
            return "Path is not a Directory"

        # Validate the path is readable
        if not os.access(thePath, os.R_OK):
            return "Directory is not Readable"

        # Attempt to Create a corpus with all .txt files found in the directory
        try:

            self.Corpus = PlaintextCorpusReader(thePath, '.*')
            print ("Processing Files : ")
            print (self.Corpus.fileids())
            print ("Please wait ...")
            self.rawText = self.Corpus.raw()
            self.tokens = nltk.word_tokenize(self.rawText)
            self.TextCorpus = nltk.Text(self.tokens)  
            self.status = True
            
            #print(dir(self.TextCorpus))
        except:
            self.status = False

    def printCorpusLength(self):
        print ("\n\nCorpus Text Length: ", '{:,}'.format(len(self.rawText)))

    def printTokensFound(self):
        print ("\n\nTokens Found: ", '{:,}'.format(len(self.tokens)))

    def printVocabSize(self):
        print ("\n\nCalculating ...")
        vocabularyUsed = set(self.TextCorpus)
        vocabularySize = len(vocabularyUsed) 
        print ("Vocabulary Size: ", '{:,}'.format(vocabularySize))
    
    def searchWordOccurrence(self):
        myWord = input("\n\nEnter Search Word : ")
        if myWord:
            wordCount = self.TextCorpus.count(myWord)
            print (myWord+" occured: ", wordCount, " times")
        else:
            print ("Word Entry is Invalid")    

    def generateConcordance(self):
        myWord = input("\n\nEnter word to Concord : ")
        if myWord:
            print ("Compiling First 100 Concordance Entries ...")
            ''' YOUR CODE GOES HERE '''
            concordanceText = self.TextCorpus
            concordanceText.concordance(myWord, lines=5)
            

    def printWordIndex(self):
        myWord = input("\n\nFind first occurrence of what Word? : ")
        myWord = myWord.upper()
        if myWord:
            print("Searching for first occurrence of: ", myWord)
            ''' YOUR CODE GOES HERE '''
            indexText = self.TextCorpus
            index = indexText.index(myWord)
            print(f"The first occurrence of the word {myWord} occurred at the index of '{index}'")
            #indexText.concordance(myWord, lines=1)
            
    def printVocabulary(self):
        print ("\n\nCompiling Vocabulary Frequencies")
        nonAsciiDetected = False
        tbl = PrettyTable(["Vocabulary", "Occurs"])
        ''' YOUR CODE GOES HERE '''
        text = self.TextCorpus
        freq = nltk.FreqDist(text)
        vocabulary = freq.keys()
        vocDict = {}
        
        for eachWord in vocabulary:
            nonAsciiDetected = False
            if len(eachWord) > 3:
                for eachChar in eachWord:
                    decimalVal = ord(eachChar)
                    if decimalVal < 91 and decimalVal > 65:
                        continue
                    else:
                        nonAsciiDetected = True
                        break
                if nonAsciiDetected != True:
                    vocDict[eachWord] = freq[eachWord]
                    
        for key, value in vocDict.items():
            tbl.add_row([key, int(value)])
        tbl.sortby = "Occurs"
        tbl.reversesort = True
        tbl.align = "l"
        printTable = tbl.get_string
        print(tbl)

def printMenu():
    
    # Function to print the NLTK Query Option Menu
    print("\n\n")
    print ("==========NLTK Query Options =========")
    print ("[1]    Print Length of Corpus")
    print ("[2]    Print Number of Token Found")
    print ("[3]    Print Vocabulary Size")
    print ("[4]    Search for Word Occurrence")
    print ("[5]    Generate Concordance")
    print ("[6]    Print Word Index")
    print ("[7]    Print Vocabulary")
    print()
    print ("[0]    Exit NLTK Experimentation")
    
    print()
  
 # Function to obtain user input 
    
def getUserSelection():
    printMenu()
    
    while True:
        try:
            sel = input('Enter Selection (0-7) >> ')
            menuSelection = int(sel)
        except ValueError:
            print ('Invalid input. Enter a value between 0-8.')
            continue
    
        if not menuSelection in range(0, 9):
            print ('Invalid input. Enter a value between 0 - 8.')
            continue
    
        return menuSelection

if __name__ == '__main__':
    print("\n" + SCRIPT_NAME)
    print(SCRIPT_VERSION)
    print(SCRIPT_AUTHOR + "\n")    
    print ("Welcome to the NLTK Query Experimentation")
    print ("Please wait loading NLTK ... \n")
    
    print ("Input full path name where intended corpus file or files are stored")
    print ("Format for Windows e.g. ./CORPUS \n")
    
    userSpecifiedPath = input("Path: ") 
    
    # Attempt to create a text Corpus
    oNLTK = classNLTKQuery(userSpecifiedPath)
    
    if oNLTK.status:
        
        menuSelection = -1
        
        while menuSelection != 0:
                
            menuSelection = getUserSelection()        
            
            if menuSelection == 1:
                oNLTK.printCorpusLength()
            
            elif menuSelection == 2:
                oNLTK.printTokensFound()
    
            elif menuSelection == 3:
                oNLTK.printVocabSize()
    
            elif menuSelection == 4:         
                oNLTK.searchWordOccurrence()      
    
            elif menuSelection == 5:    
                oNLTK.generateConcordance()        
    
            elif menuSelection == 6:    
                oNLTK.printWordIndex()
    
            elif menuSelection == 7:    
                oNLTK.printVocabulary()
                
            elif menuSelection == 0:    
                print("Goodbye")
                print()
            
            else:
                print ("unexpected error condition")
                menuSelection = 0

            sleep(2)
    
    else:
            print ("Closing NLTK Query Experimentation")
    
    
