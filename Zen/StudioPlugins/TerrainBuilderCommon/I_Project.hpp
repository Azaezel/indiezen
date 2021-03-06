//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Terrain Builder
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
// Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef TERRAIN_BUILDER_I_PROJECT_HPP_INCLUDED
#define TERRAIN_BUILDER_I_PROJECT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Spaces/ObjectModel/I_Filter.hpp>
#include <Zen/Spaces/ObjectModel/I_Subscription.hpp>

#include <Zen/Studio/WorkbenchCommon/I_Project.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectExplorerController.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Studio {
        namespace Workbench {
            class I_ProjectExplorerController;
        }   // namespace Workbench
    }   // namespace Studio
}   // namespace Zen
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_ProjectListener;

/// A project
class TERRAINBUILDERCOMMON_DLL_LINK I_Project
:   public virtual Zen::Studio::Workbench::I_Project
{
    /// @name Types
    /// @{
public:
    typedef I_ProjectListener*                                                  pProjectListener_type;
    typedef Zen::Memory::managed_ptr<Zen::Spaces::ObjectModel::I_Filter>        pFilter_type;
    typedef Zen::Memory::managed_ptr<Zen::Spaces::ObjectModel::I_Subscription>  pSubscription_type;
    /// @}

    /// @name TerrainBuilder::I_Project interface
    /// @{
public:
    /// Subscribe a listener to this project.
    /// The subscription should be stored in the listener so that when
    /// the listener goes out of scope, the subscription also goes out
    /// of scope. The listener should also retain a strong reference
    /// to the project.
    /// @param _pListener - Unmanaged pointer to the listener. The listener
    ///     MUST remain alive for at least as long as the document (which it
    ///     will if it keeps a strong reference tothe project).
    ///     The listener must dereference the subscription before the listener
    ///     is destroyed.
    /// @todo Should this be promoted to Workbench::I_Project?
    virtual pSubscription_type subscribe(pProjectListener_type _pListener, pFilter_type pFilter = pFilter_type()) = 0;
    /// @}

    /// @name Static methods
    /// @{
public:
    static Zen::Studio::Workbench::I_ExplorerNodeType& getNodeType();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Project();
    virtual ~I_Project();
    /// @}

};  // interface I_Project

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


#endif // TERRAIN_BUILDER_I_PROJECT_HPP_INCLUDED
