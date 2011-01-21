//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2011 Tony Richards
// Copyright (C)        2011 Matthew Alan Gray
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
#ifndef ZEN_ZPOSTGRES_DATABASE_TRANSACTION_HPP_INCLUDED
#define ZEN_ZPOSTGRES_DATABASE_TRANSACTION_HPP_INCLUDED

#include "DatabaseConnection.hpp"

#include "PostgresTypes.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Enterprise/Database/I_DatabaseTransaction.hpp>
#include <Zen/Enterprise/Database/I_DatabaseResult.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZPostgres {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class DatabaseQuery;

class DatabaseTransaction
:   public Zen::Database::I_DatabaseTransaction
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Database::I_DatabaseResult>      pDatabaseResult_type;
    typedef Zen::Memory::managed_weak_ptr<Database::I_DatabaseResult> wpDatabaseResult_type;
    /// @}

    /// @name I_DatabaseTransaction implementation
    /// @{
public:
    virtual pStaticQuery_type createStaticQuery();
    virtual pDynamicQuery_type createDynamicQuery();
    virtual void executeQuery(pQuery_type _pQuery, pQueryResultHandler_type _pQueryResultHandler);
    virtual void executeQuery(pQuery_type _pQuery);
    virtual pFutureKey_type executeInsert(pQuery_type _pQuery);
    virtual pDatabaseCommand_type createCommand();
    virtual void executeCommand(pDatabaseCommand_type _pCommand, pCommandResultHandler_type _pCommandResultHandler);
    virtual bool isFinalized() const;
    virtual bool isActive() const;
    virtual void commit();
    virtual void rollback();
    /// @}

    /// @name DatabaseTransaction implementation
    /// @{
public:
    void reset();
    std::string generateStatementName(DatabaseQuery* _pQuery);
    /// @}

    /// @name Event handlers
    /// @{
public:
    void onDestroyStaticQuery(wpStaticQuery_type _wpStaticQuery);
    void onDestroyDynamicQuery(wpDynamicQuery_type _wpDynamicQuery);
    void onDestroyDatabaseCommand(wpDatabaseCommand_type _wpDatabaseCommand);
    void onDestroyDatabaseResult(wpDatabaseResult_type _wpDatabaseResult);
    /// @}

    /// @name 'Structors
    /// @{
public:
             DatabaseTransaction(PGconn* _pConnection);
    virtual ~DatabaseTransaction();
    /// @}

    /// @name Member variables
    /// @{
private:
    bool                m_isActive;
    
    bool                m_isCommitted;

    PGconn*             m_pConnection;
    /// @}

};  // class DatabaseTransaction

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZPostgres
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZPOSTGRES_DATABASE_TRANSACTION_HPP_INCLUDED
