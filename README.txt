This repository contains a collection of various network building blocks that
I have created.  Since the netcat (or nc) program doesn't seem to support
UDP, I decided to write the UdpClient and UdpServer software to circumvent
this problem.

We have the following directories:

1. UdpClient - This directory contains code for the UdpClient object and a
test program that reads its input from stdin and sents a datagram that
contains the data just read.

2. UdpServer - This directory contains code for the UdpServer object and
a program that accepts UDP datagrams and sends the payload data to
stdout. 

