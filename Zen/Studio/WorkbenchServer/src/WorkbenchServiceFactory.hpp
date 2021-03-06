//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio Workbench
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
#ifndef ZEN_STUDIO_WORKBENCH_WORKBENCH_SERVICE_FACTORY_HPP_INCLUDED
#define ZEN_STUDIO_WORKBENCH_WORKBENCH_SERVICE_FACTORY_HPP_INCLUDED

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Enterprise/AppServer/I_ApplicationServiceFactory.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class WorkbenchServiceFactory
:   public Zen::Enterprise::AppServer::I_ApplicationServiceFactory
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_weak_ptr<Zen::Enterprise::AppServer::I_ApplicationService>  wpApplicationService_type;
    /// @}

    /// @name I_ApplicationServiceFactory implementation
    /// @{
public:
    virtual pApplicationService_type create(Zen::Enterprise::AppServer::I_ApplicationServer& _server, const std::string& _name);
    /// @}

    /// @name WorkbenchServiceFactory implementation
    /// @{
private:
    void destroy(wpApplicationService_type _pApplicationService);
    /// @}

    /// @name Static Instance
    /// @{
public:
    /// Get the singleton
    /// @return Reference to the singleton instance.
    static WorkbenchServiceFactory& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             WorkbenchServiceFactory();
    virtual ~WorkbenchServiceFactory();
    /// @}

};  // class WorkbenchServiceFactory

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_STUDIO_WORKBENCH_WORKBENCH_SERVICE_FACTORY_HPP_INCLUDED
