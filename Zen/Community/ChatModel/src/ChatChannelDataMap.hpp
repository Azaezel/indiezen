//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Spaces
//
// Copyright (C) 2001 - 2009 Tony Richards
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// This is generated by the Zen Spaces Code Generator.  Do not modify!
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_COMMUNITY_CHAT_CHATCHANNEL_DATA_MAP_HPP_INCLUDED
#define ZEN_COMMUNITY_CHAT_CHATCHANNEL_DATA_MAP_HPP_INCLUDED

#include "../I_ChatChannelDataMap.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Chat {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


class ChatChannelDataMap
:	public I_ChatChannelDataMap
{
    /// @name I_ChatChannelDataMap implementation
    /// @{
public:
    virtual pChatChannelDomainObject_type createNew();
    virtual pFutureChatChannelDomainObject_type getByKey(boost::uint64_t _primaryKeyValue);
    virtual void deleteByKey(boost::uint64_t _primaryKeyValue);
    virtual pFutureChatChannelDataCollection_type getAll();
    virtual Zen::Database::I_DatabaseTransaction::pFutureKey_type update(pChatChannelDomainObject_type _pChatChannelDomainObject);
    /// @}

    /// @name ChatChannelDataMap implementation
    /// @{
public:
    /// Escape the string to prevent SQL injection
    std::string escapeString(const std::string& _value) const;

    /// Get a comma separated list of the fields in this table.
    static const std::string& getFieldNames();

    /// Get a comma separated list of the fields in this table
    /// sans the primary key (chatChannelId).
    static const std::string& getNonPKFieldNames();
    
    /// Get the name of the table this class maps.
    static const std::string& getTableName();

	pDatabaseConnection_type getDatabaseConnection() const;
    /// @}

	/// @name Member Variables
	/// @{
public:
	pDatabaseConnection_type			m_pDatabaseConnection;
	/// @}

    /// @name 'Structors
    /// @{
public:
             ChatChannelDataMap(pDatabaseConnection_type _pDatabaseConnection);
    virtual ~ChatChannelDataMap();
    /// @}

};  // class ChatChannelDataMap

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zen
}   // namespace Community
}   // namespace Chat
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


#endif // ZEN_COMMUNITY_CHAT_CHATCHANNEL_DATA_MAP_HPP_INCLUDED
