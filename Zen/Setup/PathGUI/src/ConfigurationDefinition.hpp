//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
// Copyright (C) 2008        Matthew Alan Gray
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
#ifndef ZEN_PATH_GUI_CONFIGURATION_DEFINITION_HPP_INCLUDED
#define ZEN_PATH_GUI_CONFIGURATION_DEFINITION_HPP_INCLUDED

#include <boost/serialization/serialization.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace PathGUI {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ConfigurationDefinition
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name ConfigurationDefinition implementation
    /// @{
public:
    /// @}

    /// @name ConfigurationDefinition serialization
    /// @{
private:
    friend boost::serialization::access;
    template<class Archive>
    void serialize(Archive& _ar, const unsigned int _version)
    {
        _ar & boost::serialization::make_nvp("plugin-paths", m_pPluginPathDefinitions);
        _ar & boost::serialization::make_nvp("module-paths", m_pModulePathDefinitions);
        _ar & boost::serialization::make_nvp("vsprops-paths", m_pUserMacroPathDefinitions);
    }
    /// @}

    /// @name 'Structors
    /// @{
public:
             ConfigurationDefinition();
    virtual ~ConfigurationDefinition();
    /// @}

    /// @name Member variables
    /// @{
private:
    std::vector<PathDefinition*>    m_pPluginPathDefinitions;
    std::vector<PathDefinition*>    m_pModulePathDefinitions;
    std::vector<PathDefinition*>    m_pUserMacroPathDefinitions;
    /// @}

};  // class ConfigurationDefinition

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace PathGUI
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PATH_GUI_CONFIGURATION_DEFINITION_HPP_INCLUDED
