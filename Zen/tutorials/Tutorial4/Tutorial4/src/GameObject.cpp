//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Game Tutorial
//
// Copyright (C) 2001 - 2010 Tony Richards
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
// This project is part of the Zen Engine Tutorials
//
// For more details, click on the link below for the IndieZen.org documentation:
//
// http://www.indiezen.org/wiki/wiki/zoss/Engine/Tutorials
//
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#include "GameObject.hpp"
#include "GameClient.hpp"

#include <Zen/Core/Scripting.hpp>

#include <Zen/Engine/Core/I_GameGroup.hpp>

#include <Zen/Engine/Rendering/I_SceneService.hpp>

#include <Zen/Engine/Resource/I_ResourceManager.hpp>
#include <Zen/Engine/Resource/I_ResourceService.hpp>

#include <Zen/Starter/Base/BaseClient/I_BaseGameClient.hpp>

#include <sstream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Game {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObject::GameObject(Zen::Engine::Core::I_GameGroup& _group, const std::string& _name)
:   m_name(_name)
,   m_isVisible(true)
,   m_pScriptObject(NULL)
{
    m_pBaseObject = dynamic_cast<Zen::Engine::Base::I_BaseGameObject*>(&_group.createObject(_name));

    base().setGameObject(this);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObject::~GameObject()
{
    clearRenderables();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static GameObject::pScriptType_type sm_pScriptType;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Scripting::I_ObjectReference*
GameObject::getScriptObject()
{
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptObjectReference_type
            (
                m_pBaseObject->getGame().getScriptModule(),
                m_pBaseObject->getGame().getScriptModule()->getScriptType(getScriptTypeName()),
                this
            );
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static std::string sm_scriptTypeName("GameObject");
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
GameObject::getScriptTypeName()
{
    return sm_scriptTypeName;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObject::initScriptType(GameClient& _gameClient)
{
    sm_pScriptType = _gameClient.game().getScriptModule()->createScriptType(sm_scriptTypeName, "Game Object", 0);

    Zen::Scripting::script_type<GameObject>(sm_pScriptType)
        .addMethod("base", &GameObject::baseBase)
        .addMethod("loadRenderable", &GameObject::loadRenderable)
        .activate()
    ;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObject::clearRenderables()
{
    pSceneNode_type pSceneNode = baseBase().getSceneNode();

    if( pSceneNode.isValid() )
    {
        pSceneNode->detachAllObjects();
    }

    m_renderables.clear();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObject::pRenderableResource_type
GameObject::loadRenderable(const std::string& _meshName, const std::string& _materialName)
{
    Zen::Engine::Resource::I_ResourceManager::config_type config;
    config["fileName"] = _meshName;

    // TODO Come up with a better naming scheme.  What if this object has
    // two meshes of the same name?  This scheme won't work :(
    std::stringstream name;
    name << m_name << ":" << _meshName;
    config["label"] = name.str();

    /// TODO Should we be hardcoding this?
    config["scene"] = "default";

    config["type"] = "entity";

    pRenderableResource_type pRenderable =
        Zen::Engine::Base::I_BaseGameClient::getSingleton().getRenderingResourceService().loadResource(config).as<pRenderableResource_type>();


    if (!_materialName.empty())
    {
        pRenderable->setMaterialName(_materialName);
    }

    pSceneNode_type pSceneNode = baseBase().getSceneNode();

    // Attach to scene node if not already attached
    bool wasAttachedToSceneNode = true;
    if( !pSceneNode.isValid() )
    {
        wasAttachedToSceneNode = false;
        pSceneNode = Zen::Engine::Base::I_BaseGameClient::getSingleton().getSceneService()->createChildNode(name.str());
    }

    pSceneNode->attachObject(*pRenderable.get());

    if( !wasAttachedToSceneNode )
    {
        base().attachToSceneNode(pSceneNode);
    }

    m_renderables.push_back(pRenderable);

    pRenderable->setVisible(m_isVisible);
    return pRenderable;

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObject::setMaterial(const std::string& _materialName, int _index)
{
    m_renderables[_index]->setMaterialName(_materialName);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObject::setVisible(bool _visible)
{
    m_isVisible = _visible;
    for(RenderableResources_type::iterator iter = m_renderables.begin(); iter != m_renderables.end(); iter++)
    {
        (*iter)->setVisible(_visible);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
GameObject::isVisible() const
{
    return m_isVisible;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObject::setData(Zen::Engine::Core::I_GameObjectData* _pData)
{
    m_pData = _pData;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::Core::I_GameObjectData*
GameObject::getData()
{
    return m_pData;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObject::setScale(Zen::Engine::Core::I_BaseGameObject& _object, float _x, float _y, float _z)
{
    _object.setScale(_x, _y, _z);

    // TODO Should physics actor support scale?
    //_object.getPhysicsActor()->setScale(_x * 100.0f, _y * 100.0f, _z * 100.0f);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Game
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
