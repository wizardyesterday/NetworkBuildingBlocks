//**********************************************************************
// file name: UdpClient.h
//**********************************************************************

#ifndef _UDPCLIENT_H_
#define _UDPCLIENT_H_

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

class UdpClient
{
  public:

  UdpClient(char *ipAddressPtr,int port,int maxPayloadLength);
  ~UdpClient(void);

  bool connectionIsEstablished(void);
  bool sendData(void *bufferPtr,int bufferLength);

  private:

  // Attributes
  int socketDescriptor;
  struct sockaddr_in peerAddress;

  size_t maxPayloadLength;
};

#endif // _UDPCLIENT_H_
