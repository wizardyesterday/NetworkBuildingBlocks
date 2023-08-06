//**********************************************************************
// file name: UdpServer.cc
//**********************************************************************

#include "UdpServer.h"
#include <stdio.h>

/**************************************************************************

  Name: UdpServer

  Purpose: The purpose of this function is to serve as the constructor
  of a UdpServer object.  This function opens a socket, and if
  successful, populates the peer address structure with the appropriate
  information.

  Calling Sequence: UdpServer(port,maxPayloadLength)

  Inputs:

    port - The port number for this server to listen to.

    maxPayloadLength - The maximum payload lengththat this server will
    accept from a UDP datagram.  In other words, this server will
    attempt to read maxPayloadLength octets from the socket buffer.
    As an example, if a value of 1 is chosen for this parameter, it
    is possible to get bogged down with system calls, and the result
    will be dropped UDP datagrams.

  Outputs:

    None.

**************************************************************************/
UdpServer::UdpServer(int port,int maxPayloadLenth,
  void (*receiveCallbackPtr)(sockaddr *peerAddressPtr,
                             void *bufferPtr,
                             uint32_t bufferLength))
{
  int bufferLength;
  int status;
  void *kludgePtr;
  sockaddr *myAddressPtr;

  if (maxPayloadLenth > 32768)
  {
    // Keep it reasonable.
    maxPayloadLenth = 32768;
  } // if

  // Save in the attributes.
  this->maxPayloadLenth = maxPayloadLenth;
  this->receiveCallbackPtr = receiveCallbackPtr;

  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
  // Okay, this is a kludge.  I'm doing this
  // because the sockets API accepts, both the
  // sockaddr_in and the sockaddr structures for
  // different functions.  Keep in mind that these
  // two contexts represent the same data structure.
  // Yes, I know why this was done.  The books, by
  // Stevens, describe the rational to this
  // insanity.
  // Anyway, this was my way of beating C++ type
  // checking into submission.  I'm sure I'll find
  // a better way of doing this.
  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
  kludgePtr = &myAddress;
  myAddressPtr = (sockaddr *)kludgePtr;
  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

  // Let's make a large enough buffer.
  bufferLength = maxPayloadLenth;

  if (bufferLength < 2048)
  {
    // Make it reasonable.
    bufferLength = 2048;
  } // if

  // Create UDP socket.
  socketDescriptor = socket(PF_INET,SOCK_DGRAM,0);

  if (socketDescriptor != -1)
  {
    // Always zero out the address structure to avoid side effects.
    bzero(&myAddress,sizeof(myAddress));

    // Populate the fields.
    myAddress.sin_family = AF_INET;
    myAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    myAddress.sin_port = htons(port);

    // Associate an address to the socket.
    status = bind(socketDescriptor,myAddressPtr,sizeof(*myAddressPtr));

    if (status == 0)
    {
      // Set the socket transmit buffer size.
      status = setsockopt(socketDescriptor,
                          SOL_SOCKET,
                          SO_RCVBUF,
                          &bufferLength,
                          sizeof(int));

      if (status != 0)
      {
        // Indicate that the socket has not been opened.
        socketDescriptor = 0;
      } // if
    } // if
    else
    {
      // Indicate that the socket has not been opened.
      socketDescriptor = 0;
    } // else
  } // if
  else
  {
    // Indicate that the socket has not been opened.
    socketDescriptor = 0;
  } // else

  return;
 
} // UdpServer

/**************************************************************************

  Name: ~UdpServer

  Purpose: The purpose of this function is to serve as the destructor
  of a UdpServer object.

  Calling Sequence: ~UdpServer()

  Inputs:

    None.

  Outputs:

    None.

**************************************************************************/
UdpServer::~UdpServer(void)
{

  if (socketDescriptor != 0)
  {
    close(socketDescriptor);
  } // if

  return;
 
} // ~UdpServer

/**************************************************************************

  Name: connectionIsEstablished

  Purpose: The purpose of this function is to serve as the destructor
  of a UdpServer object.

  Calling Sequence: status = connectionIsEstablished()

  Inputs:

    None.

  Outputs:

    status - A boolean that indicates whether or not a connection is
    established.  A value of true indicates that a connect has been
    established, and a value of false indicates that a connection has
    not been established.

**************************************************************************/
bool UdpServer::connectionIsEstablished(void)
{
  bool status;

  if (socketDescriptor != 0)
  {
    status = true;
  } // if
  else
  {
    status = false;
  } // else

  return (status);
 
} // connectionIsEstablished

/**************************************************************************

  Name: receiveData

  Purpose: The purpose of this function is to accept UDP datagrams
  received from the link partner and notify the callback function of
  the arrival of data.

  Calling Sequence: receiveData()

  Inputs:

    None.

  Outputs:

    None.

**************************************************************************/
void UdpServer::receiveData(void)
{
  bool done;
  ssize_t count;
  int i;
  socklen_t peerAddressLength;
  void *kludgePtr;
  sockaddr *peerAddressPtr;

  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
  // Okay, this is a kludge.  I'm doing this
  // because the sockets API accepts, both the
  // sockaddr_in and the sockaddr structures for
  // different functions.  Keep in mind that these
  // two contexts represent the same data structure.
  // Yes, I know why this was done.  The books, by
  // Stevens, describe the rational to this
  // insanity.
  // Anyway, this was my way of beating C++ type
  // checking into submission.  I'm sure I'll find
  // a better way of doing this.
  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
  kludgePtr = &peerAddress;
  peerAddressPtr = (sockaddr *)kludgePtr;
  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

  peerAddressLength = sizeof(*peerAddressPtr);

 if (socketDescriptor != 0)
  {
    // Set up for loop entry.
    done = false;

    while (!done)
    {
      count = recvfrom(socketDescriptor,
                       receiveBuffer,
                       maxPayloadLenth,
                       0,
                       peerAddressPtr,
                       &peerAddressLength);

      // Notify the higher level client application.
      receiveCallbackPtr(peerAddressPtr,receiveBuffer,count);
    } // while
  } // if

  return;

} // receiveData
