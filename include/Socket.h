/*
 * Socket.h
 *
 *  Created on: May 4, 2014
 *      Author: dubyoo
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdexcept>
#include <iostream>

class Socket {
public:
	Socket(const std::string &ip, const std::string &port);
	virtual ~Socket();
	int send_message(char *buf, int len);
	int recv_message(char *buf, int len);
	const struct sockaddr_in get_client_addr() const;

private:
	int m_server_fd;
	socklen_t m_len;
	struct sockaddr_in m_server_addr;
	struct sockaddr_in m_client_addr;
};

#endif /* SOCKET_H_ */
