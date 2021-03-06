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
#ifndef ZEN_STUDIO_WORKBENCH_I_SPREADSHEETROW_DATA_MAP_HPP_INCLUDED
#define ZEN_STUDIO_WORKBENCH_I_SPREADSHEETROW_DATA_MAP_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Enterprise/Database/I_DatabaseConnection.hpp>
#include <Zen/Enterprise/Database/I_DatabaseService.hpp>
#include <Zen/Enterprise/Database/I_DatabaseTransaction.hpp>

#include <boost/cstdint.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_SpreadSheetRowDomainObject;
class I_SpreadSheetRowDataCollection;

class WORKBENCHMODEL_DLL_LINK I_SpreadSheetRowDataMap
:	public boost::noncopyable
{

    /// @name Types
    /// @{
public:
//    typedef Zen::Memory::managed_ptr<I_SpreadSheetRowDomainObject>   pSpreadSheetRowDomainObject_type;
//    typedef Zen::Memory::managed_ptr<I_SpreadSheetRowDataCollection>     pSpreadSheetRowDataCollection_type;
    typedef Zen::Memory::managed_ptr<I_SpreadSheetRowDataMap>        pSpreadSheetRowDataMap_type;

    typedef Zen::Database::I_DatabaseService::pDatabaseConnection_type      pDatabaseConnection_type;

    typedef Zen::Memory::managed_ptr<I_SpreadSheetRowDataCollection>  pSpreadSheetRowDataCollection_type;
    typedef Zen::Event::future_return_value<pSpreadSheetRowDataCollection_type>  FutureSpreadSheetRowDataCollection_type;
	typedef Zen::Memory::managed_ptr<FutureSpreadSheetRowDataCollection_type>    pFutureSpreadSheetRowDataCollection_type;
    
    typedef Zen::Memory::managed_ptr<I_SpreadSheetRowDomainObject>  pSpreadSheetRowDomainObject_type;
    typedef Zen::Event::future_return_value<pSpreadSheetRowDomainObject_type>  FutureSpreadSheetRowDomainObject_type;
	typedef Zen::Memory::managed_ptr<FutureSpreadSheetRowDomainObject_type>    pFutureSpreadSheetRowDomainObject_type;
    /// @}

    /// @name I_SpreadSheetRowDataMap interface
    /// @{
public:
    /// Create a new SpreadSheetRowDomainObject.
    virtual pSpreadSheetRowDomainObject_type createNew() = 0;
    
    /// Get an object by primary key.
    virtual pFutureSpreadSheetRowDomainObject_type getByKey(boost::uint64_t _primaryKeyValue) = 0;

    /// Delete an object by primary key.
    virtual void deleteByKey(boost::uint64_t _primaryKeyValue) = 0;

    /// Get SpreadSheet rows by spreadSheetDocumentId.    
    virtual pFutureSpreadSheetRowDataCollection_type getByDocumentId(const boost::uint64_t _spreadSheetDocumentId) = 0;

    /// Get all of the records.
    virtual pFutureSpreadSheetRowDataCollection_type getAll() = 0;

    /// Update a SpreadSheetRowDomainObject.
    /// If the SpreadSheetRowDomainObject hasn't been inserted yet, it will be inserted.
    virtual Zen::Database::I_DatabaseTransaction::pFutureKey_type update(pSpreadSheetRowDomainObject_type _pSpreadSheetRowDomainObject) = 0;
    /// @}

    /// @name Static class factory
    /// @{
public:
    static pSpreadSheetRowDataMap_type create(pDatabaseConnection_type _pDatabaseConnection);
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_SpreadSheetRowDataMap();
    virtual ~I_SpreadSheetRowDataMap();
    /// @}

};  // interface I_SpreadSheetRowDataMap

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zen
}   // namespace Studio
}   // namespace Workbench
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

namespace Zen {
namespace Memory {
    /// I_SpreadSheetRowDataMap is a managed by factory
    template<>
    struct is_managed_by_factory<Zen::Studio::Workbench::I_SpreadSheetRowDataMap> : public boost::true_type{};
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_STUDIO_WORKBENCH_I_SPREADSHEETROW_DATA_MAP_HPP_INCLUDED
