//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
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
//  Tony Richards trichards@indiezen.org
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ZCRAZYEDDIE_WIDGET_RENDERER_HPP_INCLUDED
#define ZEN_ZCRAZYEDDIE_WIDGET_RENDERER_HPP_INCLUDED

#include <Zen/Engine/Widgets/I_WidgetRenderer.hpp>

#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZCrazyEddie {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class WidgetRenderer
:   public Zen::Engine::Widgets::I_WidgetRenderer
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_WidgetRenderer implementation
    /// @{
public:
    virtual dimension_type getScreenHeight();
    virtual dimension_type getScreenWidth();
    /// @}

    /// @name WidgetRenderer implementation
    /// @{
public:
    CEGUI::OgreRenderer& getCEGUIRenderer();
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class WidgetService;
             WidgetRenderer(Zen::Engine::Rendering::I_View& _view);
public:
    virtual ~WidgetRenderer();
    /// @}

    /// @name Member variables
    /// @{
private:
    CEGUI::OgreRenderer*                    m_pWidgetRenderer;
    /// @}

};  // class WidgetRenderer

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZCrazyEddie
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZCRAZYEDDIE_WIDGET_RENDERER_HPP_INCLUDED
