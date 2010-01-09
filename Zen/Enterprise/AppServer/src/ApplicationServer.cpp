//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
// Copyright (C)        2009 Joshua Cassity
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
//  Joshua Cassity jcassity@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "ApplicationServer.hpp"

#include "ResourceLocation.hpp"
#include "NumericTypeMessageRegistry.hpp"

#include "../I_ApplicationServerManager.hpp"
#include "../I_ProtocolService.hpp"
#include "../I_ApplicationService.hpp"
#include "../I_Message.hpp"
#include "../I_MessageFactory.hpp"
#include "../I_Request.hpp"
#include "../I_Response.hpp"
#include "../I_ResponseHandler.hpp"
#include "../I_SessionEvent.hpp"

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Threading/MutexFactory.hpp>

#include <Zen/Core/Scripting/I_ScriptableService.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void destroy(Memory::managed_weak_ptr<I_MessageRegistry> _pMessageRegistry)
{
    NumericTypeMessageRegistry* pMessageRegistry = dynamic_cast<NumericTypeMessageRegistry*>(_pMessageRegistry.get());

    if (pMessageRegistry)
    {
        delete pMessageRegistry;
    }
    else
    {
        // TODO Error?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

ApplicationServer::ApplicationServer()
:   m_sharedThreadPool(16, NULL, true, true)
,   m_installQueue(1, NULL, true, false)
,   m_shutdownQueue(1, NULL, true, false)
,   m_pProtocolGuard(Threading::MutexFactory::create())
,   m_pApplicationGuard(Threading::MutexFactory::create())
,   m_pMessageRegistry_type(new NumericTypeMessageRegistry(), &destroy)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServer::~ApplicationServer()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
ApplicationServer::start()
{
    class InstallationCompleteTask
    :   public Threading::ThreadPool::Task
    {
    private:
        class PrepareToStartCompleteTask
        :   public Threading::ThreadPool::Task
        {
        private:
            class StartCompleteTask
            :   public Threading::ThreadPool::Task
            {
            public:
                virtual void call()
                {
                    m_condition.assertCondition();
                }

                StartCompleteTask(Zen::Threading::I_Condition& _condition)
                :   m_condition(_condition)
                {
                }
            private:
                Zen::Threading::I_Condition&    m_condition;
            };
            
        public:
            virtual void call()
            {
                StartCompleteTask* pTask = new StartCompleteTask(m_condition);
                m_installQueue.pushRequest(pTask);
            }

            PrepareToStartCompleteTask(Zen::Threading::I_Condition& _condition, Zen::Threading::ThreadPool& _installQueue)
            :   m_condition(_condition)
            ,   m_installQueue(_installQueue)
            {
            }
        private:
            Zen::Threading::I_Condition&    m_condition;
            Zen::Threading::ThreadPool&     m_installQueue;
        };
    public:
        virtual void call()
        {
            PrepareToStartCompleteTask* pTask = new PrepareToStartCompleteTask(m_condition, m_installQueue);
            m_installQueue.pushRequest(pTask);
        }

        InstallationCompleteTask(Zen::Threading::I_Condition& _condition, Zen::Threading::ThreadPool& _installQueue)
        :   m_condition(_condition)
        ,   m_installQueue(_installQueue)
        {
        }
    private:
        Zen::Threading::I_Condition&    m_condition;
        Zen::Threading::ThreadPool&     m_installQueue;
    };

    // Create an I_Condition to all the caller to know when all the applications have been installed
    Zen::Threading::I_Condition* pCondition = Zen::Threading::ConditionFactory::create(false);

    // Create the installation complete task and pass the condition into it.
    InstallationCompleteTask* pTask = new InstallationCompleteTask(*pCondition, m_installQueue);

    // Push the task onto the end of the queue
    m_installQueue.pushRequest(pTask);

    // Start the installation queue so that protocols and services will be
    // installed.
    m_installQueue.start();

    return pCondition;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::stop()
{
    class InstallationQueueEmptyTask
    :   public Threading::ThreadPool::Task
    {
    public:
        virtual void call()
        {
            m_condition.assertCondition();
        }

        InstallationQueueEmptyTask(Zen::Threading::I_Condition& _condition)
        :   m_condition(_condition)
        {
        }
    private:
        Zen::Threading::I_Condition&    m_condition;
    };

    class ShutdownCompleteTask
    :   public Threading::ThreadPool::Task
    {
    public:
        virtual void call()
        {
            m_condition.assertCondition();
        }

        ShutdownCompleteTask(Zen::Threading::I_Condition& _condition)
        :   m_condition(_condition)
        {
        }
    private:
        Zen::Threading::I_Condition&    m_condition;
    };

    // Create an I_Condition to know when the installQueue is empty...
    Zen::Threading::I_Condition* pInstallerCondition = Zen::Threading::ConditionFactory::create(false);

    // Create the installation complete task and pass the condition into it.
    InstallationQueueEmptyTask* pInstallerTask = new InstallationQueueEmptyTask(*pInstallerCondition);

    // Push the task onto the end of the queue
    m_installQueue.pushRequest(pInstallerTask);

    // Prepare to stop the installation queue so that no more protocols or services will
    // be installed (at least not until the app server is restarted, if ever).
    m_installQueue.prepareToStop();

    // Wait for the install queue to be emptied
    pInstallerCondition->requireCondition();

    // Destroy the condition
    Zen::Threading::ConditionFactory::destroy(pInstallerCondition);

    // Stop the install queue
    m_installQueue.stop();

    // Create an I_Condition to know when the shutdownQueue is finished.
    Zen::Threading::I_Condition* pShutdownCondition = Zen::Threading::ConditionFactory::create(false);

    // Create the shutdown complete task and pass the condition into it.
    ShutdownCompleteTask* pShutdownTask = new ShutdownCompleteTask(*pShutdownCondition);

    // Push the task onto the end of the queue
    m_shutdownQueue.pushRequest(pShutdownTask);

    // Start the shutdown queue
    m_shutdownQueue.start();

    // Wait on the shutdown queue to completely start
    m_shutdownQueue.requireStarted();

    // Prepare to stop the shutdown queue so that no more tasks will be
    // pushed onto it.
    m_shutdownQueue.prepareToStop();

    // Wait for the shutdown queue to be emptied.
    pShutdownCondition->requireCondition();

    // Destroy the condition
    Zen::Threading::ConditionFactory::destroy(pShutdownCondition);

    // Stop the shutdown queue
    m_shutdownQueue.stop();

    // Empty service collections
    {
        Threading::CriticalSection lock(m_pProtocolGuard);
        m_protocolServices.empty();
    }

    {
        Threading::CriticalSection lock(m_pApplicationGuard);
        m_applicationServices.empty();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::registerDefaultScriptEngine(pScriptEngine_type _pEngine)
{
    // TODO Register with all of the already-installed app services.

    m_pScriptEngine = _pEngine;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::installProtocols(pConfig_type _pProtocolsConfig)
{
    class ConfigVisitor
    : public Zen::Plugins::I_ConfigurationElement::I_ConfigurationElementVisitor
    {
    public:
        virtual void begin()
        {
        }

        virtual void visit(const Zen::Plugins::I_ConfigurationElement& _element)
        {
            Zen::Enterprise::AppServer::I_ApplicationServerManager& manager = 
                Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton();

            Zen::Enterprise::AppServer::I_ApplicationServerManager::pProtocolService_type
                pProtocolService = manager.createProtocolService(*m_pAppServer, _element.getAttribute("type"));

            if (pProtocolService.isValid())
            {
                pProtocolService->setConfiguration(_element);
                m_pAppServer->installProtocol(pProtocolService, _element.getAttribute("name"));
            }
        }

        virtual void end()
        {
        }

        ConfigVisitor(I_ApplicationServer* _pAppServer)
        :   m_pAppServer(_pAppServer)
        {
        }
    private:
        I_ApplicationServer*        m_pAppServer;
    };

    ConfigVisitor visitor(this);
    _pProtocolsConfig->getChildren("protocol", visitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::installProtocol(pProtocolService_type _pProtocolService, const std::string& _protocolName)
{
    class InstallProtocolServiceTask
    :   public Threading::ThreadPool::Task
    {
    private:
        class PrepareToStartProtocolServiceTask
        :   public Threading::ThreadPool::Task
        {
        private:
            class StartProtocolServiceTask
            :   public Threading::ThreadPool::Task
            {
            public:
                virtual void call()
                {
                    m_pProtocolService->start();
                }

                StartProtocolServiceTask(pProtocolService_type _pProtocolService)
                :   m_pProtocolService(_pProtocolService)
                {
                }
            private:
                pProtocolService_type   m_pProtocolService;
            };
            class StopProtocolServiceTask
            :   public Threading::ThreadPool::Task
            {
            public:
                virtual void call()
                {
                    m_pProtocolService->stop();
                }

                StopProtocolServiceTask(pProtocolService_type _pProtocolService)
                :   m_pProtocolService(_pProtocolService)
                {
                }
            private:
                pProtocolService_type   m_pProtocolService;
            };
        public:
            virtual void call()
            {
                Threading::I_Condition* pCondition = m_pProtocolService->prepareToStart(m_server.getSharedThreadPool());
                if( pCondition != NULL )
                {
                    pCondition->requireCondition();
                }

                StartProtocolServiceTask* pStartTask = new StartProtocolServiceTask(m_pProtocolService);
                StopProtocolServiceTask* pStopTask = new StopProtocolServiceTask(m_pProtocolService);
                m_installQueue.pushRequest(pStartTask);
                m_shutdownQueue.pushRequest(pStopTask);
            }

            PrepareToStartProtocolServiceTask(ApplicationServer& _server, pProtocolService_type _pProtocolService, Threading::ThreadPool& _installQueue, Threading::ThreadPool& _shutdownQueue)
            :   m_server(_server)
            ,   m_pProtocolService(_pProtocolService)
            ,   m_installQueue(_installQueue)
            ,   m_shutdownQueue(_shutdownQueue)
            {
            }
        private:
            ApplicationServer&          m_server;
            pProtocolService_type       m_pProtocolService;
            Threading::ThreadPool&      m_installQueue;
            Threading::ThreadPool&      m_shutdownQueue;
        };
        class PrepareToStopProtocolServiceTask
        :   public Threading::ThreadPool::Task
        {
        public:
            virtual void call()
            {
                Threading::I_Condition* pCondition = m_pProtocolService->prepareToStop();
                if( pCondition != NULL )
                {
                    pCondition->requireCondition();
                }
            }

            PrepareToStopProtocolServiceTask(pProtocolService_type _pProtocolService)
            :   m_pProtocolService(_pProtocolService)
            {
            }
        private:
            pProtocolService_type       m_pProtocolService;
        };
    public:
        virtual void call()
        {
            m_server.handleInstallProtocol(m_pProtocolService, m_protocolName);
            PrepareToStartProtocolServiceTask* pStartTask = new PrepareToStartProtocolServiceTask(m_server,m_pProtocolService, m_installQueue, m_shutdownQueue);
            PrepareToStopProtocolServiceTask* pStopTask = new PrepareToStopProtocolServiceTask(m_pProtocolService);
            m_installQueue.pushRequest(pStartTask);
            m_shutdownQueue.pushRequest(pStopTask);
        }

        InstallProtocolServiceTask(ApplicationServer& _server, pProtocolService_type _pProtocolService, const std::string& _protocolName, Threading::ThreadPool& _installQueue, Threading::ThreadPool& _shutdownQueue)
        :   m_server(_server)
        ,   m_pProtocolService(_pProtocolService)
        ,   m_protocolName(_protocolName)
        ,   m_installQueue(_installQueue)
        ,   m_shutdownQueue(_shutdownQueue)
        {
        }
    private:
        ApplicationServer&      m_server;
        pProtocolService_type   m_pProtocolService;
        std::string             m_protocolName;
        Threading::ThreadPool&  m_installQueue;
        Threading::ThreadPool&  m_shutdownQueue;
    };

    // Schedule the installation of the protocol.
    InstallProtocolServiceTask* pTask = new InstallProtocolServiceTask(*this, _pProtocolService, _protocolName,m_installQueue, m_shutdownQueue);
    m_installQueue.pushRequest(pTask);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServer::pProtocolService_type
ApplicationServer::getProtocol(const std::string& _protocolName)
{
    Threading::CriticalSection lock(m_pProtocolGuard);

    ProtocolServices_type::iterator iter = m_protocolServices.find(_protocolName);

    if (iter != m_protocolServices.end())
    {
        return iter->second;
    }
    else
    {
        // TODO Throw an exception?
        return pProtocolService_type();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::installApplications(pConfig_type _pAppServicesConfig)
{
	class ConfigVisitor
	:	public Zen::Plugins::I_ConfigurationElement::I_ConfigurationElementVisitor
	{
	public:
		virtual void begin() {}
		
		virtual void visit(const Zen::Plugins::I_ConfigurationElement& _element)
		{
			Zen::Enterprise::AppServer::I_ApplicationServerManager& manager =
				Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton();

			Zen::Enterprise::AppServer::I_ApplicationServerManager::pApplicationService_type
				pApplicationService = manager.createApplicationService(*m_pAppServer, _element.getAttribute("type"));

			if( pApplicationService.isValid() )
			{
				pApplicationService->setConfiguration(_element);
				m_pAppServer->installApplication(pApplicationService, manager.createLocation(_element.getAttribute("location")));
			}
		}

		virtual void end() {}

        ConfigVisitor(I_ApplicationServer* _pAppServer)
        :   m_pAppServer(_pAppServer)
        {
        }

    private:
        I_ApplicationServer*        m_pAppServer;
    };

    ConfigVisitor visitor(this);
	_pAppServicesConfig->getChildren("application", visitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::installApplication(pApplicationService_type _pApplicationService, pResourceLocation_type _pRootLocation)
{
    class InstallApplicationServiceTask
    :   public Threading::ThreadPool::Task
    {
        class PrepareToStartApplicationServiceTask
        :   public Threading::ThreadPool::Task
        {
        private:
            class StartApplicationServiceTask
            :   public Threading::ThreadPool::Task
            {
            public:
                virtual void call()
                {
                    m_pApplicationService->start();
                }

                StartApplicationServiceTask(pApplicationService_type _pApplicationService)
                :   m_pApplicationService(_pApplicationService)
                {
                }
            private:
                pApplicationService_type   m_pApplicationService;
            };
            class StopApplicationServiceTask
            :   public Threading::ThreadPool::Task
            {
            public:
                virtual void call()
                {
                    m_pApplicationService->stop();
                }

                StopApplicationServiceTask(pApplicationService_type _pApplicationService)
                :   m_pApplicationService(_pApplicationService)
                {
                }
            private:
                pApplicationService_type    m_pApplicationService;
            };
        public:
            virtual void call()
            {
                Threading::I_Condition* pCondition = m_pApplicationService->prepareToStart(m_server.getSharedThreadPool());
                if( pCondition != NULL )
                {
                    pCondition->requireCondition();
                }

                StartApplicationServiceTask* pStartTask = new StartApplicationServiceTask(m_pApplicationService);
                StopApplicationServiceTask* pStopTask = new StopApplicationServiceTask(m_pApplicationService);
                m_installQueue.pushRequest(pStartTask);
                m_shutdownQueue.pushRequest(pStopTask);
            }

            PrepareToStartApplicationServiceTask(ApplicationServer& _server, pApplicationService_type _pApplicationService, Threading::ThreadPool& _installQueue, Threading::ThreadPool& _shutdownQueue)
            :   m_server(_server)
            ,   m_pApplicationService(_pApplicationService)
            ,   m_installQueue(_installQueue)
            ,   m_shutdownQueue(_shutdownQueue)
            {
            }
        private:
            ApplicationServer&          m_server;
            pApplicationService_type    m_pApplicationService;
            Threading::ThreadPool&      m_installQueue;
            Threading::ThreadPool&      m_shutdownQueue;
        };

        class PrepareToStopApplicationServiceTask
        :   public Threading::ThreadPool::Task
        {
        public:
            virtual void call()
            {
                Threading::I_Condition* pCondition = m_pApplicationService->prepareToStop();
                if( pCondition != NULL )
                {
                    pCondition->requireCondition();
                }
            }

            PrepareToStopApplicationServiceTask(pApplicationService_type _pApplicationService)
            :   m_pApplicationService(_pApplicationService)
            {
            }
        private:
            pApplicationService_type    m_pApplicationService;
        };
    public:
        virtual void call()
        {
            // TODO What about the resource location?
            m_server.handleInstallApplication(m_pApplicationService, m_pRootLocation);
            PrepareToStartApplicationServiceTask* pStartTask = new PrepareToStartApplicationServiceTask(m_server, m_pApplicationService, m_installQueue, m_shutdownQueue);
            PrepareToStopApplicationServiceTask* pStopTask = new PrepareToStopApplicationServiceTask(m_pApplicationService);
            m_installQueue.pushRequest(pStartTask);
            m_shutdownQueue.pushRequest(pStopTask);
        }

        InstallApplicationServiceTask(ApplicationServer& _server, pApplicationService_type _pApplicationService, pResourceLocation_type _pRootLocation, Threading::ThreadPool& _installQueue, Threading::ThreadPool& _shutdownQueue)
        :   m_server(_server)
        ,   m_pApplicationService(_pApplicationService)
        ,   m_pRootLocation(_pRootLocation)
        ,   m_installQueue(_installQueue)
        ,   m_shutdownQueue(_shutdownQueue)
        {
        }
    private:
        ApplicationServer&          m_server;
        pApplicationService_type    m_pApplicationService;
        pResourceLocation_type      m_pRootLocation;
        Threading::ThreadPool&      m_installQueue;
        Threading::ThreadPool&      m_shutdownQueue;
    };

    // Schedule the installation of the application.
    InstallApplicationServiceTask* pTask = new InstallApplicationServiceTask(*this, _pApplicationService, _pRootLocation, m_installQueue, m_shutdownQueue);
    m_installQueue.pushRequest(pTask);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::configureApplication(pApplicationService_type _pApplicationService, pConfig_type _pConfig)
{
    class ConfigureApplicationServiceTask
    :   public Threading::ThreadPool::Task
    {
    public:
        virtual void call()
        {
            m_server.handleConfigureApplication(m_pApplicationService, m_pConfig);
        }

        ConfigureApplicationServiceTask(ApplicationServer& _server, pApplicationService_type _pApplicationService, pConfig_type _pConfig)
        :   m_server(_server)
        ,   m_pApplicationService(_pApplicationService)
        ,   m_pConfig(_pConfig)
        {
        }

    private:
        ApplicationServer&          m_server;
        pApplicationService_type    m_pApplicationService;
        pConfig_type                m_pConfig;
    };

    // Schedule the configuration of the application.
    ConfigureApplicationServiceTask* pTask = new ConfigureApplicationServiceTask(*this, _pApplicationService, _pConfig);
    m_installQueue.pushRequest(pTask);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServer::pApplicationService_type
ApplicationServer::getApplication(pResourceLocation_type _pServiceLocation) const
{
    Threading::CriticalSection lock(m_pApplicationGuard);

    ApplicationServices_type::const_iterator iter = m_applicationServices.find(_pServiceLocation);

    if( iter != m_applicationServices.end() )
    {
        return iter->second;
    }
    else
    {
        // TODO Throw an exception?
        return pApplicationService_type();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::getResourceLocations(I_ResourceLocationVisitor& _visitor) const
{
    _visitor.begin();

    ApplicationServices_type::const_iterator iter = m_applicationServices.begin();
    while( iter != m_applicationServices.end() )
    {
        _visitor.visit(iter->first, iter->second);
        iter++;
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ApplicationServer::pMessageRegistry_type
ApplicationServer::getMessageRegistry()
{
    return m_pMessageRegistry_type;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::handleMessage(pMessage_type _pMessage)
{
    ResourceLocation* pDestination = dynamic_cast<ResourceLocation*>(_pMessage->getDestinationLocation().get());

    if (pDestination != NULL)
    {
        class HandleMessageTask
        :   public Threading::ThreadPool::Task
        {
        public:
            virtual void call()
            {
                if( m_pApplicationService.isValid() )
                {
                    m_pApplicationService->handleMessage(m_pMessage);
                }
            }

            HandleMessageTask(pApplicationService_type _pApplicationService, pMessage_type _pMessage)
            :   m_pApplicationService(_pApplicationService)
            ,   m_pMessage(_pMessage)
            {
            }

            virtual ~HandleMessageTask() {}
        private:
            pApplicationService_type    m_pApplicationService;
            pMessage_type               m_pMessage;
        };

        // TODO Use a TaskPool
        HandleMessageTask* pTask = new HandleMessageTask(pDestination->getApplicationService(), _pMessage);

        m_sharedThreadPool.pushRequest(pTask);
    }
    else
    {
        throw Utility::runtime_exception("ApplicationServer::handleMessage(): Error, invalid destination.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::handleRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
{
    ResourceLocation* pDestination = dynamic_cast<ResourceLocation*>(_pRequest->getDestinationLocation().get());

    if (pDestination != NULL)
    {
        class HandleRequestTask
        :   public Threading::ThreadPool::Task
        {
        public:
            virtual void call()
            {
                if( m_pApplicationService.isValid() )
                {
                    m_pApplicationService->handleRequest(m_pRequest, m_pResponseHandler);
                }
            }

            HandleRequestTask(pApplicationService_type _pApplicationService, pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
            :   m_pApplicationService(_pApplicationService)
            ,   m_pRequest(_pRequest)
            ,   m_pResponseHandler(_pResponseHandler)
            {
            }

            virtual ~HandleRequestTask() {}
        private:
            pApplicationService_type    m_pApplicationService;
            pRequest_type               m_pRequest;
            pResponseHandler_type       m_pResponseHandler;
        };

        // TODO Use a TaskPool
        HandleRequestTask* pTask = new HandleRequestTask(pDestination->getApplicationService(), _pRequest, _pResponseHandler);

        m_sharedThreadPool.pushRequest(pTask);
    }
    else
    {
        throw Utility::runtime_exception("ApplicationServer::handleRequest(): Error, invalid destination.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::handleSessionEvent(pSessionEvent_type _pSessionEvent)
{
    throw Utility::runtime_exception("ApplicationServer::handleSessionEvent(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::handleInstallProtocol(pProtocolService_type _pProtocolService, const std::string& _protocolName)
{
    Threading::CriticalSection lock(m_pProtocolGuard);

    m_protocolServices[_protocolName] = _pProtocolService;

    // TODO Handle the startup sequence better.  All prepareToStart() must be called for both the protocols
    // and the services, then all of the start() methods must be called.
    // start() must be called

#if 0   // deprecated
    _pProtocolService->prepareToStart(m_sharedThreadPool);
    _pProtocolService->start();
#endif  // deprecated
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::handleInstallApplication(pApplicationService_type _pApplicationService, pResourceLocation_type _pRootLocation)
{
    Threading::CriticalSection lock(m_pApplicationGuard);

    m_applicationServices[_pRootLocation] = _pApplicationService;

    // TODO Get the root resource location and set the application service
    ResourceLocation* pRoot = dynamic_cast<ResourceLocation*>(_pRootLocation.get());
    pRoot->setApplicationService(_pApplicationService);

    if (m_pScriptEngine.isValid())
    {
        Scripting::I_ScriptableService* const pScriptable = dynamic_cast<Scripting::I_ScriptableService*>(_pApplicationService.get());

        if (pScriptable)
        {
            pScriptable->registerScriptEngine(m_pScriptEngine);
        }
    }

#if 0   // deprecated
    // TODO Handle startup sequence better.  
    _pApplicationService->prepareToStart(m_sharedThreadPool);
    _pApplicationService->start();
#endif  // deprecated
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ApplicationServer::handleConfigureApplication(pApplicationService_type _pApplicationService, pConfig_type _pConfig)
{
    _pApplicationService->setConfiguration(*_pConfig);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

