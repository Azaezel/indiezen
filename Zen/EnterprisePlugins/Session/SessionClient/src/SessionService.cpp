//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
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

#include "SessionService.hpp"
#include "Session.hpp"
#include "../Protocol/I_SessionProtocolManager.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Event/I_Event.hpp>
#include <Zen/Core/Event/I_Action.hpp>
#include <Zen/Core/Event/I_ActionMap.hpp>
#include <Zen/Core/Event/I_Connection.hpp>

#include <Zen/Enterprise/AppServer/I_ApplicationServerManager.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Session {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SessionService::SessionService(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer)
:   Zen::Enterprise::AppServer::scriptable_generic_service <Zen::Enterprise::Session::Client::I_SessionService, SessionService>(_appServer)
,   m_pScriptObject(NULL)
,   m_pScriptModule(NULL)
,   m_pScriptEngine()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SessionService::~SessionService()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
SessionService::getScriptTypeName()
{
    static std::string sm_name("SessionClient");
    return sm_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
SessionService::getScriptObject()
{
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptWrapper_type(getScriptModule(),
            getScriptModule()->getScriptType(getScriptTypeName()),
            this
        );
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SessionService::pScriptModule_type
SessionService::getScriptModule()
{
    return m_pScriptModule->getScriptModule();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::registerScriptEngine(pScriptEngine_type _pScriptEngine)
{
    m_pScriptEngine = _pScriptEngine;

    // TODO change this so the Community module can be shared
    m_pScriptModule = new Zen::Scripting::script_module(_pScriptEngine, "Enterprise");

    m_pScriptModule->addType<SessionService>(getScriptTypeName(), "Session Client Service")
        .addMethod("requestLogin", &SessionService::scriptLogin)
        .addMethod("getSessionEvent", &SessionService::getSessionEvent)
        .createGlobalObject("sessionClient", this)
    ;

    Session::registerScriptModule(*m_pScriptModule);

    m_pScriptModule->activate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::setConfiguration(const Zen::Plugins::I_ConfigurationElement& _config)
{
    super::setConfiguration(_config);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
SessionService::prepareToStart(Zen::Threading::ThreadPool& _threadPool)
{
    Zen::Enterprise::Session::Protocol::Client::I_SessionProtocolManager::getSingleton().install(getApplicationServer());

    return super::prepareToStart(_threadPool);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::start()
{
    // Create some actions.
    getApplicationServer().getEventService()->getActionMap("SessionClient").createAction("onDisconnected", boost::bind(&SessionService::handleOnDisconnected, this, _1));

    // Connect this the sessionClient protocol onDisconnected event to this
    // services onDisconnected action.
    // TODO We need m_pOnDisconnectedConnection if we disconnect from this event.
    //m_pOnDisconnectedConnection = 
        getApplicationServer().getProtocol("sessionClient")->getDisconnectedEvent()
            .connect
            (
                (getApplicationServer().getEventService()->getActionMap("SessionClient"))["onDisconnected"].getSelfReference().lock(), 
                &getApplicationServer().getEventService()->getEventQueue("script")
            );

    // Allow the super class to start up.
    super::start();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
SessionService::prepareToStop()
{
    return super::prepareToStop();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::stop()
{
    super::stop();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::uint64_t
SessionService::requestLogin(pEndpoint_type _pDestinationEndpoint,
                             pResourceLocation_type _pDestLocation,
                             const std::string& _name,
                             const std::string& _password)
{
    Zen::Enterprise::AppServer::create_request<Zen::Enterprise::Session::Protocol::I_LoginRequest, void*>
        request(_pDestinationEndpoint, NULL, getServiceLocation(), _pDestLocation);

    request->setUserId(_name);
    request->setPassword(_password);

    send<Zen::Enterprise::Session::Protocol::I_LoginRequest, void*>
        (request, boost::bind(&SessionService::handleLoginResponse, this, _1, _2, _3));

    return request->getMessageId();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::requestLogout(Enterprise::Session::I_Session& _session)
{
    throw Zen::Utility::runtime_exception("SessionService::requestLogout() : Not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Event::I_Event&
SessionService::getSessionEvent()
{
    return  getApplicationServer().getEventService()->createEvent("SessionService::Client::SessionEvent");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::scriptLogin(const std::string& _server, const std::string& _port, const std::string& _location, const std::string& _name, const std::string& _password)
{
    // TODO Don't hard-code this protocol name.
    pEndpoint_type pEndpoint = getApplicationServer().getProtocol("sessionClient")
        ->resolveEndpoint(_server, _port);
    
    // TODO make sure this endpoint doesn't already exist in m_endpointIndex
    // TODO Guard for m_endpointIndex
    AddressIndex_type::iterator iter = m_addressIndex.find(pEndpoint->toString());
    if (iter != m_addressIndex.end())
    {
        // This is an address that has already been used, so re-use
        // the original one.
        pEndpoint = iter->second;
    }
    else
    {
        // This is a new address, save the endpoint.
        m_addressIndex[pEndpoint->toString()] = pEndpoint;
    }

    // TODO Save the endpoint and associate it with this server and port?
    
    // For now lets just assume we only have one "scriptLogin" request.

    requestLogin(
        pEndpoint, 
        Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton()
            .createLocation(_location),
        _name, 
        _password
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::handleLoginResponse(pResponse_type _pResponse, Zen::Enterprise::Session::Protocol::I_LoginRequest& _request, void* _nullPtr)
{
    // _pSession is the payload associated with the original _request.
    // Handle this response.

    Zen::Enterprise::Session::Protocol::I_LoginResponse* pResponse =
        dynamic_cast<Zen::Enterprise::Session::Protocol::I_LoginResponse*>(_pResponse.get());

    // TODO Create an index of sessionId to Session*
    // Hack?
    pSession_type pSession = new Session(pResponse->getSession());

    m_sessionIdIndex[pSession->getSessionId()] = pSession;

    // Notify that the session status has changed.
    boost::any anySession(pSession);
    getSessionEvent().fireEvent(anySession);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::handleOnDisconnected(boost::any _anyEndpoint)
{
    // When an endpoint is disconnected, see if there's a session associated with it.
    // TODO Handle the case of multiple sessions connected to the same endpoint.

    pEndpoint_type pEndpoint = boost::any_cast<pEndpoint_type>(_anyEndpoint);

    EndpointIndex_type::iterator iter = m_endpointIndex.find(pEndpoint);

    if (iter != m_endpointIndex.end())
    {
        // If there is a session associated with this endpoint, it's not valid anymore, 
        // so mark it as DISCONNECTED and fire the session change event.
        Enterprise::Session::I_Session* const pSession = iter->second;
        if (pSession != NULL)
        {
            Session* const pSessionImpl = dynamic_cast<Session*>(pSession);
            //pSessionImpl->setSessionState(Enterprise::Session::I_Session::DISCONNECTED);
            boost::any anySession(pSessionImpl->getScriptObject());
            getSessionEvent().fireEvent(anySession);
        }
        else
        {
            // Error
        }
    }
    else
    {
        // Error?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Session
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
