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
#ifndef ZEN_COMMUNITY_CHAT_I_CHATCOMMAND_DATA_MAP_HPP_INCLUDED
#define ZEN_COMMUNITY_CHAT_I_CHATCOMMAND_DATA_MAP_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Enterprise/Database/I_DatabaseConnection.hpp>
#include <Zen/Enterprise/Database/I_DatabaseService.hpp>
#include <Zen/Enterprise/Database/I_DatabaseTransaction.hpp>

#include <boost/cstdint.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Chat {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_ChatCommandDomainObject;
class I_ChatCommandDataCollection;

class CHATMODEL_DLL_LINK I_ChatCommandDataMap
:	public boost::noncopyable
{

    /// @name Types
    /// @{
public:
//    typedef Zen::Memory::managed_ptr<I_ChatCommandDomainObject>   pChatCommandDomainObject_type;
//    typedef Zen::Memory::managed_ptr<I_ChatCommandDataCollection>     pChatCommandDataCollection_type;
    typedef Zen::Memory::managed_ptr<I_ChatCommandDataMap>        pChatCommandDataMap_type;

    typedef Zen::Database::I_DatabaseService::pDatabaseConnection_type      pDatabaseConnection_type;

    typedef Zen::Memory::managed_ptr<I_ChatCommandDataCollection>  pChatCommandDataCollection_type;
    typedef Zen::Event::future_return_value<pChatCommandDataCollection_type>  FutureChatCommandDataCollection_type;
	typedef Zen::Memory::managed_ptr<FutureChatCommandDataCollection_type>    pFutureChatCommandDataCollection_type;
    
    typedef Zen::Memory::managed_ptr<I_ChatCommandDomainObject>  pChatCommandDomainObject_type;
    typedef Zen::Event::future_return_value<pChatCommandDomainObject_type>  FutureChatCommandDomainObject_type;
	typedef Zen::Memory::managed_ptr<FutureChatCommandDomainObject_type>    pFutureChatCommandDomainObject_type;
    /// @}

    /// @name I_ChatCommandDataMap interface
    /// @{
public:
    /// Create a new ChatCommandDomainObject.
    virtual pChatCommandDomainObject_type createNew() = 0;
    
    /// Get an object by primary key.
    virtual pFutureChatCommandDomainObject_type getByKey(boost::uint64_t _primaryKeyValue) = 0;

    /// Delete an object by primary key.
    virtual void deleteByKey(boost::uint64_t _primaryKeyValue) = 0;

    /// Get all of the records.
    virtual pFutureChatCommandDataCollection_type getAll() = 0;

    /// Update a ChatCommandDomainObject.
    /// If the ChatCommandDomainObject hasn't been inserted yet, it will be inserted.
    virtual Zen::Database::I_DatabaseTransaction::pFutureKey_type update(pChatCommandDomainObject_type _pChatCommandDomainObject) = 0;
    /// @}

    /// @name Static class factory
    /// @{
public:
    static pChatCommandDataMap_type create(pDatabaseConnection_type _pDatabaseConnection);
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ChatCommandDataMap();
    virtual ~I_ChatCommandDataMap();
    /// @}

};  // interface I_ChatCommandDataMap

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zen
}   // namespace Community
}   // namespace Chat
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

namespace Zen {
namespace Memory {
    /// I_ChatCommandDataMap is a managed by factory
    template<>
    struct is_managed_by_factory<Zen::Community::Chat::I_ChatCommandDataMap> : public boost::true_type{};
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_CHAT_I_CHATCOMMAND_DATA_MAP_HPP_INCLUDED
