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

class UdpServer
{
  public:

  UdpServer(int port,int payloadSize,
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

  socklen_t maxPayloadSize;
  unsigned char receiveBuffer[65536];

  // Receive callback support.  This allows an application to send a reply.
  void (*receiveCallbackPtr)(sockaddr *peerAddressPtr,
                             void *bufferPtr,
                             uint32_t bufferLength);
};

#endif // _UDPSERVER_H_
