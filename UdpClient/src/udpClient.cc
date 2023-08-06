//**********************************************************************
// file name: test.cc
//**********************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UdpClient.h"

#define DEFAULT_HOST_IP_ADDRESS "192.93.16.87"
#define DEFAULT_HOST_PORT (8001)

// This structure is used to consolidate user parameters.
struct MyParameters
{
  char *hostIpAddressPtr;
  int *hostPortPtr;
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
  // The host IP address.
  strcpy(parameters.hostIpAddressPtr,DEFAULT_HOST_IP_ADDRESS);
 
  // The host listener port.
  *parameters.hostPortPtr = DEFAULT_HOST_PORT;
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
        strcpy(parameters.hostIpAddressPtr,optarg);
        break;
      } // case

      case 'p':
      {
        // Retrieve the host listener port.
        *parameters.hostPortPtr = atoi(optarg);
        break;
      } // case

      case 'h':
      {
        // Display usage.
        fprintf(stderr,"./radioDiags -a <hostIpAddress: x.x.x.x> "
                "-p <hostPort>\n");

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
  int hostPort;
  char hostIpAddress[32];
  UdpClient *networkInterfacePtr;
  struct MyParameters parameters;

  // Set up for parameter transmission.
  parameters.hostPortPtr = &hostPort;
  parameters.hostIpAddressPtr = hostIpAddress;

  // Retrieve the system parameters.
  exitProgram = getUserArguments(argc,argv,parameters);

  if (exitProgram)
  {
    // Bail out.
    return (0);
  } // if

  // Create UDP client object.
  networkInterfacePtr = new UdpClient(hostIpAddress,hostPort);

  if (!networkInterfacePtr->connectionIsEstablished())
  {
    fprintf(stderr,"Failed to open socket\n");
    return (-2);
  } // if

  // Set up for loop entry.
  done = false;

  while (!done)
  {
    // Read the data.
    count = fread(inputBuffer,sizeof(char),2048,stdin);

    if (count == 0)
    {
      // We're done.
      done = true;
    } // if
    else
    {
      // Send the message.
      success = networkInterfacePtr->sendData(inputBuffer,count,2048);
    } // else
  } // while

  // Clean up.
  if (networkInterfacePtr != NULL)
  {
    delete networkInterfacePtr;
  } // if

  return (0);

} // main
