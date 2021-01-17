

#include "../include/client.h"


Client::~Client() {
    if (m_threadHandler != nullptr) {
        m_threadHandler->detach();
        delete m_threadHandler;
        m_threadHandler = nullptr;
    }
}

bool Client::operator==(const Client &rhs) const {
    return m_sockfd == rhs.m_sockfd &&
           m_ip == rhs.m_ip;
}

bool Client::operator!=(const Client &rhs) const {
    return !(rhs == *this);
}