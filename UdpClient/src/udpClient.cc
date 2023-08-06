//**********************************************************************
// file name: udpClient.cc
//**********************************************************************


//*************************************************************************
// This program provides the functionality of a UDP client.  The program
// reads its input from stdin and sends a UDP datagram with the payload
// equal to the contents of what was read from stdin.
//
// To run this program type,
// 
//     ./udpClient -p <listenport> -m <maxpayloadlength>
//
// where,
//
//    listenport - the port number for which the server will listen.
//
//    maxpayloadlength - The maximum amount of octets that will be
//    read when a UDP datagram arrives.
//*************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UdpClient.h"

#define DEFAULT_SERVER_IP_ADDRESS "192.93.16.87"
#define DEFAULT_SERVER_PORT (8001)
#define MAX_PAYLOAD_LENGTH (2048)

// This structure is used to consolidate user parameters.
struct MyParameters
{
  char *serverIpAddressPtr;
  int *serverPortPtr;
};

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
  int i;
  int opt;

  // Default not to exit program.
  exitProgram = false;

  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
  // Default parameters.
  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
  // The server IP address.
  strcpy(parameters.serverIpAddressPtr,DEFAULT_SERVER_IP_ADDRESS);
 
  // The server listener port.
  *parameters.serverPortPtr = DEFAULT_SERVER_PORT;
  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

  // Set up for loop entry.
  done = false;

  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
  // Retrieve the command line arguments.
  //_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
  while (!done)
  {
    // Retrieve the next option.
    opt = getopt(argc,argv,"a:p:h");

    switch (opt)
    {
      case 'a':
      {
        // Retrieve the IP address.        
        strcpy(parameters.serverIpAddressPtr,optarg);
        break;
      } // case

      case 'p':
      {
        // Retrieve the host listener port.
        *parameters.serverPortPtr = atoi(optarg);
        break;
      } // case

      case 'h':
      {
        // Display usage.
        fprintf(stderr,"./udpClient -a <serverIpAddress: x.x.x.x> "
                "-p <serverPort>\n");

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

//***********************************************************************
// Mainline code.
//***********************************************************************
int main(int argc,char **argv)
{
  bool exitProgram;
  bool done;
  bool success;
  uint32_t count;
  char inputBuffer[16384];
  int serverPort;
  char serverIpAddress[32];
  UdpClient *networkInterfacePtr;
  struct MyParameters parameters;

  // Set up for parameter transmission.
  parameters.serverPortPtr = &serverPort;
  parameters.serverIpAddressPtr = serverIpAddress;

  // Retrieve the system parameters.
  exitProgram = getUserArguments(argc,argv,parameters);

  if (exitProgram)
  {
    // Bail out.
    return (0);
  } // if

  // Create UDP client object.
  networkInterfacePtr = new UdpClient(serverIpAddress,serverPort);

  if (!networkInterfacePtr->connectionIsEstablished())
  {
    fprintf(stderr,"Failed to establish link\n");
    return (-1);
  } // if

  // Set up for loop entry.
  done = false;

  while (!done)
  {
    // Read the data.
    count = fread(inputBuffer,sizeof(char),MAX_PAYLOAD_LENGTH,stdin);

    if (count == 0)
    {
      // We're done.
      done = true;
    } // if
    else
    {
      // Send the message.
      success = networkInterfacePtr->sendData(inputBuffer,
                                              count,
                                              MAX_PAYLOAD_LENGTH);
    } // else
  } // while

  // Clean up.
  if (networkInterfacePtr != NULL)
  {
    delete networkInterfacePtr;
  } // if

  return (0);

} // main
