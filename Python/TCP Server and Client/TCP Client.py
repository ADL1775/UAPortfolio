'''
Script Purpose: Week Seven Assignment 11 - TCP Client
Script Version: 1.0 March 2023
Script Author:  Adam Livingston, University of Arizona

'''

import socket           # Import Python Standard Socket Library
import sys
import hashlib          # Hashing library
import time
import logging      # Standard Logging Library
import os


SCRIPT_NAME    = "Week Seven Assignment 11 - TCP Client"
SCRIPT_VERSION = "Version 1.0"
SCRIPT_AUTHOR  = "Author: Adam Livingston"
MESSAGE = "Message"

def LogMessage():
    print()

print("\n" + SCRIPT_NAME)
print(SCRIPT_VERSION)
print(SCRIPT_AUTHOR + "\n")

print("Client Application")
print("Establish a connection to a server")
print("Available on the same host using PORT 5555")

PORT = 5555          # Port Number of Server

# Remove any old logging script
if os.path.isfile('livingstonA_Wk7_clientlog.txt'):
    os.remove("livingstonA_Wk7_clientlog.txt")

# Configure the python logger
logging.basicConfig(filename='livingstonA_Wk7_clientlog.txt', level=logging.DEBUG, format='%(process)d-%(levelname)s-%(asctime)s %(message)s')
    
try:
    # Create a Socket
    clientSocket = socket.socket()
    
    # Get my local host address
    localHost = socket.gethostname()
    
    connAtt = f"\nAttempt Connection to: {localHost} {PORT}"
    print(connAtt)
    logging.info(connAtt)
    clientSocket.connect((localHost, PORT))
    
    # Sending message if there was a connection
    print("Socket Connected ...")
    logging.info("Socket Connected ...")
    print("Sending Message to Server")
    logging.info("Sending Message to Server")
    
    i = 1
    
    while i <= 10:
        message = f"Message {i}"
        sentMsg = f"Message sent: '{message}'"
        print(sentMsg)
        logging.info(sentMsg)
        
        messageBytes = bytes(str(message).encode("utf-8"))
        clientSocket.sendall(messageBytes)
        
        buffer = clientSocket.recv(2048)
        buffer = buffer.decode("utf-8")

        logging.info(buffer)
        print(buffer)
        logging.info('')
        
        i += 1
    

    while True:
        msg = input("Enter Message to Send or EXIT to End: ")
        
        if msg.lower() == 'exit':
            messageBytes = bytes(str(msg).encode("utf-8"))
            clientSocket.sendall(messageBytes)
            logging.info(msg)
            logging.info('Connection terminated.')
            break
        
        msgLog = f"Message sent: {msg}"
        logging.info(msgLog)
        
        messageBytes = bytes(str(msg).encode("utf-8"))
        clientSocket.sendall(messageBytes)
        buffer = clientSocket.recv(2048)
        buffer = buffer.decode("utf-8")
        logging.info(buffer)
        
        print(buffer)
        logging.info('')
    
    
except Exception as err:
    sys.exit(err)

            
print("Script end")