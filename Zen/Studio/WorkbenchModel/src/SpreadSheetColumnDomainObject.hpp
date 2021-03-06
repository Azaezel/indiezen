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
#ifndef ZEN_STUDIO_WORKBENCH_SPREADSHEETCOLUMN_DOMAIN_OBJECT_HPP_INCLUDED
#define ZEN_STUDIO_WORKBENCH_SPREADSHEETCOLUMN_DOMAIN_OBJECT_HPP_INCLUDED

#include "../I_SpreadSheetColumnDomainObject.hpp"

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


class SpreadSheetColumnDomainObject
:	public I_SpreadSheetColumnDomainObject
{
    /// @name Types
    /// @{
public:
    typedef std::map<std::string, pElement_type>    FieldElementMap_type;
    /// @}

    /// @name Getter / Setter methods
    /// @{
public:
    /// Get the SpreadSheetColumnId element.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setSpreadSheetColumnId().
    virtual ElementReference_type getSpreadSheetColumnId() const;
    
    /// Set the SpreadSheetColumnId element value.
    /// This method does not directly update the database.  
    /// You must use the I_SpreadSheetColumnDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setSpreadSheetColumnId(const boost::any& _value);

    /// Get the SpreadSheetDocumentId element.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setSpreadSheetDocumentId().
    virtual ElementReference_type getSpreadSheetDocumentId() const;
    
    /// Set the SpreadSheetDocumentId element value.
    /// This method does not directly update the database.  
    /// You must use the I_SpreadSheetColumnDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setSpreadSheetDocumentId(const boost::any& _value);

    /// Get the ColumnNumber element.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setColumnNumber().
    virtual ElementReference_type getColumnNumber() const;
    
    /// Set the ColumnNumber element value.
    /// This method does not directly update the database.  
    /// You must use the I_SpreadSheetColumnDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setColumnNumber(const boost::any& _value);

    /// Get the ColumnText element.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setColumnText().
    virtual ElementReference_type getColumnText() const;
    
    /// Set the ColumnText element value.
    /// This method does not directly update the database.  
    /// You must use the I_SpreadSheetColumnDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setColumnText(const boost::any& _value);
    
    virtual Zen::Enterprise::DataModel::I_DataElement& getElement(const std::string& _fieldName);
    /// @}

    /// @name Member Variables
    /// @{
private:
    pElement_type               m_pSpreadSheetColumnId;
    pElement_type               m_pSpreadSheetDocumentId;
    pElement_type               m_pColumnNumber;
    pElement_type               m_pColumnText;

    FieldElementMap_type            m_elements;
    /// @}

    /// @name 'Structors
    /// @{
public:
             SpreadSheetColumnDomainObject();
    virtual ~SpreadSheetColumnDomainObject();
    /// @}

};  // class SpreadSheetColumnDomainObject

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zen
}   // namespace Studio
}   // namespace Workbench
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


#endif // ZEN_STUDIO_WORKBENCH_SPREADSHEETCOLUMN_DOMAIN_OBJECT_HPP_INCLUDED
