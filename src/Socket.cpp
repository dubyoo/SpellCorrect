/*
 * Socket.cpp
 *
 *  Created on: May 4, 2014
 *      Author: dubyoo
 */

#include "Socket.h"

Socket::Socket(const std::string &ip, const std::string &port) {
	m_server_fd = socket(AF_INET, SOCK_DGRAM, 0);
	m_server_addr.sin_family = AF_INET;
	m_server_addr.sin_port = ntohs(atoi(port.c_str()));
	m_server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
	m_len = sizeof(m_server_addr);

	//bind
	bind(m_server_fd, (const struct sockaddr*)&m_server_addr, m_len);
#ifndef NDEBUG
	std::cout << "UDP Server ready!" << std::endl;
#endif
}

Socket::~Socket() {
	//close
	close(m_server_fd); 
}

int Socket::send_message(char *buf, int len){
	//sendto
	return sendto(m_server_fd, buf, len, 0, (const struct sockaddr*)&m_client_addr, m_len);
}

int Socket::recv_message(char *buf, int len){
	//recvfrom
	int iret = recvfrom(m_server_fd, buf, len, 0, (struct sockaddr*)&m_client_addr, &m_len);
	return iret;
}

const struct sockaddr_in Socket::get_client_addr() const{
	return m_client_addr;
}
