//**********************************************************************
// file name: test.cc
//**********************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UdpServer.h"


void receiveCallback(sockaddr *peerAddressPtr,
                     void *bufferPtr,
                     uint32_t bufferLength)
{

  printf("Buffer Length: %u\n",bufferLength);
  printf("%s",bufferPtr);

  return;

} // receiveCallback


int main(int argc,char **argv)
{
  UdpServer *networkInterfacePtr;

  if (argc != 2)
  {
    fprintf(stderr,"Usage: %s  <port>\n",argv[0]);
    return (-1);
  } // if

  // Create UDP server object.
  networkInterfacePtr = new UdpServer(atoi(argv[1]),2048,receiveCallback);

  if (!networkInterfacePtr->connectionIsEstablished())
  {
    fprintf(stderr,"Failed to open socket\n");
    return (-2);
  } // if

  // Receive messages.
  networkInterfacePtr->receiveData();

  // Clean up.
  delete networkInterfacePtr;

  return (0);

} // main
