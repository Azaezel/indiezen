//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Community Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// This is generated by the Zen Protocol Code Generator.  Do not modify!
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_COMMUNITY_PROTOCOL_JOINCHANNEL_REQUEST_HPP_INCLUDED
#define ZEN_COMMUNITY_PROTOCOL_JOINCHANNEL_REQUEST_HPP_INCLUDED

#include "../I_JoinChannelRequest.hpp"

#include "Request.hpp"

#include <Zen/Enterprise/AppServer/I_MessageFactory.hpp>
#include <Zen/Enterprise/AppServer/I_MessageType.hpp>

#include <boost/serialization/string.hpp>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {


    namespace Enterprise {
        namespace AppServer {
            class I_ApplicationServer;
            class I_MessageRegistry;
            class I_MessageType;
        }   // namespace AppServer
    }   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Protocol {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class JoinChannelRequest
:   public I_JoinChannelRequest
,   public Request
{
    /// @name Types
    /// @{
public:
    enum { type = 313 };  // TODO Should we be hardcoding this?
    
    typedef Zen::Memory::managed_weak_ptr<Zen::Enterprise::AppServer::I_Request>    wpRequest_type;
    /// @}

    /// @name Zen::Enterprise::AppServer::I_Message implementation
    /// @{
public:
    virtual pEndpoint_type getSourceEndpoint() { return Message::getSourceEndpoint(); }
    virtual pEndpoint_type getDestinationEndpoint() { return Message::getDestinationEndpoint(); }
    virtual pMessageHeader_type getMessageHeader() const { return Message::getMessageHeader(); }

    virtual pResourceLocation_type getSourceLocation() { return Message::getSourceLocation(); }
    virtual pResourceLocation_type getDestinationLocation();

    virtual void serialize(pMessageHeader_type _pMessageHeader, boost::archive::polymorphic_iarchive& _archive, const int _version);
    virtual void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version);
    /// @}

    /// @name I_Message implementation
    /// @{
public:
    virtual boost::uint64_t getMessageId() const { return Message::getMessageId(); } 
    virtual pMessageType_type getMessageType() const { return getStaticMessageType(); }
    /// @}

    /// @name Getter / Setter methods
    /// @{
public:
    /// Get the sessionId element.
    virtual const boost::uint64_t& getSessionId() const;
    
    /// Set the sessionId element value.
    virtual void setSessionId(const boost::uint64_t& _sessionId);

    /// Get the channelId element.
    virtual const boost::uint64_t& getChannelId() const;
    
    /// Set the channelId element value.
    virtual void setChannelId(const boost::uint64_t& _channelId);
    /// @}

    /// @name Static methods
    /// @{
public:
    static void registerMessage(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer);

    static pMessageHeader_type createMessageHeader(boost::uint64_t _messageId);

    static void destroy(wpRequest_type _wpRequest);
    
    static pMessageType_type getStaticMessageType();    
    /// @}
    
    /// @name 'Structors
    /// @{
protected:
    friend class I_JoinChannelRequest;
    friend class MessageFactory;
             /// This constructor is used by the static create
             /// methods for creating outbound messages.
             JoinChannelRequest(pEndpoint_type _pSourceEndpoint,
                           pEndpoint_type _pDestinationEndpoint);
             /// This constructor is used by the message factory
             /// for creating inbound messages.
             JoinChannelRequest(pMessageHeader_type _pMessageHeader,
                           pEndpoint_type _pSourceEndpoint,
                           pEndpoint_type _pDestinationEndpoint);
    virtual ~JoinChannelRequest();
    /// @}

    /// @name Member Variables
    /// @{
private:
    unsigned int                    m_id;

    static pMessageType_type                                sm_pType;
    static Zen::Enterprise::AppServer::I_MessageRegistry*   sm_pMessageRegistry;
   
    boost::uint64_t m_sessionId;
    boost::uint64_t m_channelId;
    
    /// @}

};  // class JoinChannelRequest

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zen
}   // namespace Community
}   // namespace Protocol
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_PROTOCOL_JOINCHANNEL_REQUEST_HPP_INCLUDED
