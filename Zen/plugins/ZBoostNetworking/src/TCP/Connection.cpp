//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source distribution.
//
//  Tony Richards trichards@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include <boost/asio.hpp>

#include "Connection.hpp"
#include "../Endpoint.hpp"

#include "../TransmissionControlProtocolService.hpp"

#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Enterprise/Networking/I_Endpoint.hpp>
#include <Zen/Enterprise/AppServer/I_Response.hpp>

#include <boost/bind.hpp>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
namespace TCP {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Connection::Connection(boost::asio::io_service& _ioService, TransmissionControlProtocolService& _protocolService)
:   m_socket(_ioService)
,   m_strand(_ioService)
,   m_protocolService(_protocolService)
,   m_pPeerEndpoint()
,   m_connected(false)
,   m_pWriteQueueGuard(Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Connection::~Connection()
{
    if (m_connected)
    {
        disconnect();
    }

    {
        Threading::CriticalSection lock(m_pWriteQueueGuard);

        while (!m_writeMessages.empty())
        {
            delete m_writeMessages.front();
            m_writeMessages.pop_front();
        }
    }

    Threading::MutexFactory::destroy(m_pWriteQueueGuard);
    // m_pHandler is destroyed in Connection::write()
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::asio::ip::tcp::socket&
Connection::getSocketReference()
{
    return m_socket;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Connection::connect(pEndpoint_type _pEndpoint)
{
    // Connect to a server.
    m_pPeerEndpoint = _pEndpoint;

    typedef Memory::managed_ptr<Endpoint>   pConcreteEndpoint_type;
    pConcreteEndpoint_type pEndpoint(_pEndpoint.as<pConcreteEndpoint_type>());
    m_socket.async_connect(pEndpoint->getEndpoint(),
        boost::bind(&Connection::handleConnect, shared_from_this(),
        boost::asio::placeholders::error));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Connection::disconnect()
{
    m_connected = false;
    m_socket.close();
    m_protocolService.onDisconnected(shared_from_this());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Connection::handleConnect(const boost::system::error_code& _errorCode)
{
    if (!_errorCode)
    {
        Threading::CriticalSection lock(m_pWriteQueueGuard);

        m_connected = true;

        m_protocolService.onConnected(shared_from_this());

        // Now read some
        readMore();

        // Now check to see if there are any pending write messages
        if (!m_writeMessages.empty())
        {
            writeMore();
        }

    }
    else
    {
        disconnect();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Connection::start(pEndpoint_type _pEndpoint)
{
    m_connected = true;
    m_pPeerEndpoint = _pEndpoint;

    m_protocolService.onConnected(shared_from_this());

    /// Now read some
    readMore();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
std::size_t
Connection::headerBytesRead(const boost::system::error_code& _errorCode, std::size_t _bytesRead)
{
    return MessageBuffer::HEADER_LENGTH - _bytesRead;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Connection::readMore()
{
    if (m_connected)
    {
        // Asynchronously read some data from the connection into the buffer.
        // Using shared_from_this() will prevent this Connection object from
        // being destroyed while data is being read.
        boost::asio::async_read(m_socket, 
            boost::asio::buffer(m_readMessage.getData(), MessageBuffer::HEADER_LENGTH),
            boost::bind(&Connection::headerBytesRead, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred),
                boost::bind(&Connection::handleReadHeader, shared_from_this(),
                boost::asio::placeholders::error)
        );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Connection::write(const char* _buffer, boost::uint32_t _size)
{
    if (_size > MessageBuffer::MAX_BODY_LENGTH)
    {
        // TODO Error
        return;
    }

    MessageBuffer* pMsg = new MessageBuffer;
    pMsg->setBodyLength(_size);
    memcpy(pMsg->getBody(), _buffer, _size);
    pMsg->encodeHeader();

    Threading::CriticalSection lock(m_pWriteQueueGuard);

    bool writeInProgress = !m_writeMessages.empty();
    m_writeMessages.push_back(pMsg);

    // Write the message on the front of the queue if a write wasn't
    // already in progress.
    if (!writeInProgress)
    {
        writeMore();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Connection::pEndpoint_type
Connection::getPeer() const
{
    return m_pPeerEndpoint;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TransmissionControlProtocolService&
Connection::getProtocolService()
{
    return m_protocolService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
std::size_t
Connection::bodyBytesRead(const boost::system::error_code& _errorCode, std::size_t _bytesRead)
{
    return m_readMessage.getBodyLength() - _bytesRead;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Connection::handleReadHeader(const boost::system::error_code& _errorCode)
{
    if (!_errorCode && m_readMessage.decodeHeader())
    {
        boost::asio::async_read(m_socket, 
            boost::asio::buffer(m_readMessage.getBody(), m_readMessage.getBodyLength()),
                boost::bind(&Connection::bodyBytesRead, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred),
                boost::bind(&Connection::handleReadBody, shared_from_this(),
                boost::asio::placeholders::error)
        );
    }
    else
    {
        disconnect();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Connection::handleReadBody(const boost::system::error_code& _errorCode)
{
    if (!_errorCode)
    {
        m_protocolService.onHandleMessage(shared_from_this(), m_readMessage);
        readMore();
    }
    else
    {
        disconnect();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Connection::handleWrite(const boost::system::error_code& _errorCode)
{
    if (!_errorCode)
    {
        // The write succeeded, so delete the message
        Threading::CriticalSection lock(m_pWriteQueueGuard);
        delete m_writeMessages.front();
        m_writeMessages.pop_front();

        // Now write some more if necessary
        writeMore();
    }
    else
    {
        // The write failed, so don't delete the message
        std::cout << _errorCode.message() << std::endl;
        disconnect();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
std::size_t
Connection::messageBytesRemaining(const boost::system::error_code& _errorCode, std::size_t _bytesTransferred)
{
    return m_writeMessages.front()->getLength() - _bytesTransferred;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Connection::writeMore()
{
    // If there's more to write then write the next message in line
    if (!m_writeMessages.empty() && m_connected)
    {
        boost::asio::async_write(m_socket, 
            boost::asio::buffer(m_writeMessages.front()->getData(), m_writeMessages.front()->getLength()),
            boost::bind(&Connection::messageBytesRemaining, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred),
            boost::bind(&Connection::handleWrite, shared_from_this(),
            boost::asio::placeholders::error));
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TCP
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
