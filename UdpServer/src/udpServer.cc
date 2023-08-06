//**********************************************************************
// file name: test.cc
//**********************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UdpServer.h"

// This structure is used to consolidate user parameters.
struct MyParameters
{
  int *listenPortPtr;
  int *maxPayloadLengthPtr;
};

/**************************************************************************

  Name: receiveCallback

  Purpose: The purpose of this function is to serve as the constructor
  of a UdpServer object.  This function opens a socket, and if
  successful, populates the peer address structure with the appropriate
  information.

  Calling Sequence: receiveCallback(peerAddressPtr,
                                    bufferPtr,
                                    bufferLength)

  Inputs:

    peerAddressPtr - A pointer to a sockaddr structure so that replies
    could be sent to the link partner.

    bufferPtr - A pointer to a buffer of received data.

    bufferLength - The number of bytes int the buffer that is
    referenced by bufferPtr.

  Outputs:

    None.

**************************************************************************/
void receiveCallback(sockaddr *peerAddressPtr,
                     void *bufferPtr,
                     uint32_t bufferLength)
{

  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
  // Write the buffer to stdout so that the data can be piped to
  // a different program.
  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
  fwrite(bufferPtr,sizeof(unsigned char),bufferLength,stdout);

  return;

} // receiveCallback

/*****************************************************************************

  Name: getUserArguments

  Purpose: The purpose of this function is to retrieve the user arguments
  that were passed to the program.  Any arguments that are specified are
  set to reasonable default values.

  Calling Sequence: exitProgram = getUserArguments(parameters)

  Inputs:

    parameters - A structure that contains pointers to the user parameters.

  Outputs:

    exitProgram - A flag that indicates whether or not the program should
    be exited.  A value of true indicates to exit the program, and a value
    of false indicates that the program should not be exited..

*****************************************************************************/
bool getUserArguments(int argc,char **argv,struct MyParameters parameters)
{
  bool exitProgram;
  bool done;
  int opt;

  // Default not to exit program.
  exitProgram = false;

  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
  // Default parameters.
  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
  // Default a resonable port.
  *parameters.listenPortPtr = 8001;

  // Default to 2048 octets.
  *parameters.maxPayloadLengthPtr = 2048;
  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

  // Set up for loop entry.
  done = false;

  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
  // Retrieve the command line arguments.
  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
  while (!done)
  {
    // Retrieve the next option.
    opt = getopt(argc,argv,"p:m:h");

    switch (opt)
    {
      case 'p':
      {
        *parameters.listenPortPtr = atoi(optarg);
        break;
      } // case

      case 'm':
      {
        *parameters.maxPayloadLengthPtr = atoi(optarg);
        break;
      } // case

      case 'h':
      {
        // Display usage.
        fprintf(stderr,"./udpClient -p <listenport> -m <maxpayloadlength>\n");

        // Indicate that program must be exited.
        exitProgram = true;
        break;
      } // case

      case -1:
      {
        // All options consumed, so bail out.
        done = true;
      } // case
    } // switch

  } // while
  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

  return (exitProgram);

} // getUserArguments

//*************************************************************************
// Mainline code.
//*************************************************************************
int main(int argc,char **argv)
{
  bool exitProgram;
  int listenPort;
  int maxPayloadLength;
  UdpServer *networkInterfacePtr;
  struct MyParameters parameters;

 // Set up for parameter transmission.
  parameters.listenPortPtr = &listenPort;
  parameters.maxPayloadLengthPtr = &maxPayloadLength;

  // Retrieve the system parameters.
  exitProgram = getUserArguments(argc,argv,parameters);

  if (exitProgram)
  {
    // Bail out.
    return (0);
  } // if

  // Create UDP server object.
  networkInterfacePtr = new UdpServer(listenPort,
                                      maxPayloadLength,
                                      receiveCallback);

  if (!networkInterfacePtr->connectionIsEstablished())
  {
    fprintf(stderr,"Failed to open socket\n");
    return (-2);
  } // if

  // Receive messages.
  networkInterfacePtr->receiveData();

  // Clean up.
  if (networkInterfacePtr != NULL)
  {
    delete networkInterfacePtr;
  } // if

  return (0);

} // main
