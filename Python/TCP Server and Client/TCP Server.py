'''
Script Purpose: Week Seven Assignment 10 - TCP Server
Script Version: 1.0 March 2023
Script Author:  Adam Livingston, University of Arizona

Script Revision History:
Version 1.0 February 2023, Python 3.x

'''

import socket       # import Python Standard Socket Library
import sys
import hashlib          # Hashing library
import time
import logging      # Standard Logging Library
import os


SCRIPT_NAME    = "Week Seven Assignment 10 - TCP Server"
SCRIPT_VERSION = "Version 1.0"
SCRIPT_AUTHOR  = "Author: Adam Livingston"

msgIntro = bytes(str("Your md5 hash of your message: ").encode("utf-8"))

def LogMessage(string):
    logging.info(string)


print("\n" + SCRIPT_NAME)
print(SCRIPT_VERSION)
print(SCRIPT_AUTHOR + "\n")

print("Server Starting up\n")

# Remove any old logging script
if os.path.isfile('livingstonA_Wk7_serverlog.txt'):
    os.remove("livingstonA_Wk7_serverlog.txt")

# Configure the python logger
logging.basicConfig(filename='livingstonA_Wk7_serverlog.txt', level=logging.DEBUG, format='%(process)d-%(levelname)s-%(asctime)s %(message)s')

try: 
    
    serverSocket = socket.socket()      # Create Socket for listening
    
    localHost = socket.gethostname()    # Get my local host address
    
    localPort = 5555                    # Specify a local Port 
                                        # to accept connections on
    
    serverSocket.bind((localHost, localPort))  # Bind mySocket to localHost
    
    serverSocket.listen(1)              # Listen for connections
    waiting = '\nWaiting for Connection Request'
    print(waiting)
    
    logging.info(waiting)
    
    ''' Wait for a connection request
        Note this is a synchronous Call meaning the program will halt until
        a connection is received.  Once a connection is received
        we will accept the connection and obtain the 
        ipAddress of the connecting computer
    '''
    
    conn, client = serverSocket.accept()
    
    connReceived = f"Connection Received from Client: {conn} {client}" 
    print(connReceived)
    logging.info(connReceived)
    
    while True:
        buffer = conn.recv(2048)  # Wait for Data
        buffDecode = buffer.decode("utf-8")
        printMsg = f"Message received from client: '{buffDecode}'" 
        print(printMsg)
        
        logging.info(printMsg)
        
        if b'exit' in buffer.lower():
            print("Server Terminated by User")
            logging.info("Server Terminated by client")
            break
        
        
        md5Obj = hashlib.md5()
        md5Obj.update(buffer)
        fileHash = md5Obj.hexdigest()
        
        hashMsg = f"Sending hash to client: {fileHash}"
        print(hashMsg)
        logging.info(hashMsg)
        logging.info('')
        hashBytes = bytes(str(fileHash).encode("utf-8"))
        conn.sendall(msgIntro + hashBytes)
        
        if b'exit' in buffer.lower():
            print("Server Terminated by User")
            logging.info("Server Terminated by User")
            break
    
except Exception as err:
    sys.exit(str(err))

print("Script end")

