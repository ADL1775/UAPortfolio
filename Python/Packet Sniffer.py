'''
Script Purpose: Week Eight Assignment 12 - Sniffer
Script Version: 1.0 March 2023
Script Author:  Adam Livingston, University of Arizona

'''
import socket
import os
import ipaddress
import struct

from prettytable import PrettyTable
#from prettytable import DOUBLE_BORDER

SCRIPT_NAME    = "Week Eight Assignment 12 - Packet Sniffer"
SCRIPT_VERSION = "Version 1.0"
SCRIPT_AUTHOR  = "Author: Adam Livingston"

tbl = PrettyTable(["Occurs", "SRC", "DST", "Protocol"])

# Get the HOST to Sniff From
hostname = socket.gethostname()
HOST = socket.gethostbyname(hostname)

# HOST = 'localhost'
'''
testDict = {"Occurrances: " : count,
            "Source IP: " : sourceIp,
            "Dest IP: " : destIP,
            "Protocol: ": protocol,
            }
'''

packetDict = list()

def addPacket(dst, src, protocol):
    
    # Get the length of the packet list to find the next index
    newPacketIndex = len(packetDict)
    samePacket = False
    
    # If this is the first packet...
    if newPacketIndex == 0:
        packetDict.append({})
        packetDict[newPacketIndex] = {"Occurrances": 1, 
                         "Source IP": src, 
                         "Dest IP": dst, 
                         "Protocol": protocol}
        return
    
    # If this is not the first packet...
    else:
        # Iterate through the list 
        # If the dictionary at the list index has all key, value pairs match...
        # Increase the # of occurrances.
        for eachPacket in packetDict:
            if eachPacket["Source IP"] == src:
                samePacket = True
                if eachPacket["Dest IP"] == dst:
                    samePacket = True
                    if eachPacket["Protocol"] == protocol:
                        samePacket = True
                        eachPacket["Occurrances"] += 1
                        break
                        
                    else:
                        samePacket = False
                else:
                    samePacket = False
            else:
                samePacket = False
        
        # If even one key/value pair is different (other than occurrances)...
        # Create a new entry
        if samePacket == False:
            packetDict.append({})
            packetDict[newPacketIndex] = {"Occurrances": 1, 
                             "Source IP": src, 
                             "Dest IP": dst, 
                             "Protocol": protocol}
                
    return
    

class IP:
    def __init__(self, buff=None):
        header = struct.unpack('<BBHHHBBH4s4s', buff)
        self.ver = header[0] >> 4
        self.ihl = header[0] & 0xF

        self.tos = header[1]
        self.len = header[2]
        self.id = header[3]
        self.offset = header[4]
        self.ttl = header[5]
        self.protocol_num = header[6]
        self.sum = header[7]
        self.src = header[8]
        self.dst = header[9]

        # human readable IP addresses
        self.src_address = ipaddress.ip_address(self.src)
        self.dst_address = ipaddress.ip_address(self.dst)

        # map protocol constants to their names
        self.protocol_map = {1: "ICMP", 6: "TCP", 17: "UDP"}


def main():
    print("\n" + SCRIPT_NAME)
    print(SCRIPT_VERSION)
    print(SCRIPT_AUTHOR + "\n")
    
    socket_protocol = socket.IPPROTO_IP

    sniffer = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket_protocol)
    sniffer.bind((HOST, 0))

    sniffer.setsockopt(socket.IPPROTO_IP, socket.IP_HDRINCL, 1)

    sniffer.ioctl(socket.SIO_RCVALL, socket.RCVALL_ON)


    for i in range(1, 10000):

        packet = sniffer.recvfrom(65565)  # Wait for Packet
        basePacket = packet[0]  # Extract Packet Data from tuple
        pckHeader = basePacket[0:20]  # Extract the packet header

        ipOBJ = IP(pckHeader)  # Create the IP Object

        # Lookup the protocol name
        try:
            protocolName = ipOBJ.protocol_map[ipOBJ.protocol_num]
        except:
            protocolName = "Unknown"

        #print("SRC-IP  :", ipOBJ.src_address)
        #print("DST-IP  :", ipOBJ.dst_address)
        #print("Protocol:", protocolName)

        addPacket(str(ipOBJ.dst_address), str(ipOBJ.src_address), protocolName)

        '''
        YOUR CODE GOES HERE 
        You will create a dictionary entry for each unique packet observered
        Unique Packets combination of SRC-IP, DST-IP, Protocol
        you will keep track of the count of each unique packet in the dictionary

        Note: Before executing your script, launch Chrome and access one or more
              of the CyberApolis web-sites to generate network traffic

        Once you have processed 10,000 packets update load your results
        into the prettytable and display.
        '''

    
    for eachPacket in packetDict:
               
        occurs = 0
        src = ""
        dst = ""
        proto = ""
        
        i = 0
        for key, value in eachPacket.items():
            if i == 0:
                occurs = value
            if i == 1:
                src = value
            if i == 2: 
                dst = value
            if i == 3:
                proto = value
            i += 1
        
        tbl.add_row([occurs, src, dst, proto])
            
        #print(eachPacket)

    sniffer.ioctl(socket.SIO_RCVALL, socket.RCVALL_OFF)
    
    tbl.align = "l"
    #tbl.set_style(DOUBLE_BORDER)
    tbl.sortby = "Occurs"
    tbl.title = title = "Packet Capture (10,000 total)"
    print(tbl.get_string(reversesort=True))

    print("\nScript End.")


if __name__ == '__main__':
    main()


