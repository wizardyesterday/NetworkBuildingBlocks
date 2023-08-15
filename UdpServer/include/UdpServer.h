//**********************************************************************
// file name: UdpServer.h
//**********************************************************************

#ifndef _UDPSERVER_H_
#define _UDPSERVER_H_

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// I originally had a default value of 2048.  This resulted in dropping
// UDP datagrams when I set up the UDP client to send a payload of 1
// octet per datagram.  This value improved performance significantly.
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#define PAYLOAD_READ_LENGTH (32768)

class UdpServer
{
  public:

  UdpServer(int port,
            void (*receiveCallbackPtr)(sockaddr *peerAddressPtr,
                                       void *bufferPtr,
                                       uint32_t bufferLength));
  ~UdpServer(void);

  bool connectionIsEstablished(void);
  void receiveData(void);

  private:

  // Attributes
  int socketDescriptor;
  struct sockaddr_in myAddress;
  struct sockaddr_in peerAddress;

  unsigned char receiveBuffer[PAYLOAD_READ_LENGTH];

  // Receive callback support.  This allows an application to send a reply.
  void (*receiveCallbackPtr)(sockaddr *peerAddressPtr,
                             void *bufferPtr,
                             uint32_t bufferLength);
};

#endif // _UDPSERVER_H_
