//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio
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
#ifndef ZEN_COMMUNITY_COMMON_SUBSCRIPTION_HPP_INCLUDED
#define ZEN_COMMUNITY_COMMON_SUBSCRIPTION_HPP_INCLUDED

#include <Zen/Spaces/ObjectModel/I_Subscription.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Common {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Session;

/// subscription template.
/// This helper template is used for implementing lightweight subscription 
/// classes.
template<class pModel_type, class pView_type>
class subscription
:   public Zen::Spaces::ObjectModel::I_Subscription
{
    typedef Zen::Memory::managed_ptr<Zen::Networking::I_Endpoint>   pEndpoint_type;

public:
    inline
    subscription(pEndpoint_type _pEndpoint, pModel_type _pModel, pView_type _pView)
    :   m_pModel(_pModel)
    ,   m_pView(_pView)
    ,   m_pEndpoint(_pEndpoint)
    {
    }

    inline
    pEndpoint_type
    getEndpoint()
    {
        return m_pEndpoint;
    }

    inline
    pView_type
    getView()
    {
        return m_pView;
    }

    inline
    pModel_type
    getModel()
    {
        return m_pModel;
    }

    inline
    virtual ~subscription()
    {
        m_pModel->unSubscribe(this);
    }
private:
    pEndpoint_type  m_pEndpoint;
    pModel_type     m_pModel;
    pView_type      m_pView;

};  // template subscription

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Common
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_COMMON_SUBSCRIPTION_HPP_INCLUDED
