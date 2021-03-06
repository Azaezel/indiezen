//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2007 Tony Richards
// Copyright (C)        2009 Brian Roberts
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
#include "PhysicsJoint.hpp"
#include "PhysicsActor.hpp"
#include "PhysicsZone.hpp"

#include <Zen/Engine/Physics/I_PhysicsZone.hpp>
#include <Zen/Engine/Physics/I_PhysicsActor.hpp>
#include <Zen/Core/Math/Vector3.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZBullet {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsJoint::PhysicsJoint(wpPhysicsZone_type _zone)
:   m_pZone(_zone)
{
    m_pConstraint = NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsJoint::~PhysicsJoint()
{
    if (m_pConstraint != NULL) delete m_pConstraint;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsJoint::attachActor(PhysicsJoint::pPhysicsActor_type _shape)
{
    m_pShape = _shape;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsJoint::initUpVectorJoint(const Math::Vector3& _upVector)
{
    //reference: http://www.bulletphysics.org/Bullet/phpBB3/viewtopic.php?f=9&t=4457&hilit=keep+body+upright 
    // and http://www.bulletphysics.org/Bullet/phpBB3/viewtopic.php?f=9&t=2027&hilit=+new+btGeneric6DofConstraint+
    // see btRotationalLimitMotor::testLimitValue specificly

    btTransform trans = btTransform(btQuaternion(_upVector.m_x,_upVector.m_y,_upVector.m_z));
    m_pConstraint = new btGeneric6DofConstraint(*(static_cast<PhysicsActor*>(m_pShape.get())->getActorPtr()),
        *(static_cast<PhysicsActor*>(m_pShape.get())->getActorPtr()), trans, trans.inverse(), false); 

    m_pConstraint->setAngularLowerLimit(btVector3(FLT_MAX,0,0));
    m_pConstraint->setAngularUpperLimit(btVector3(-FLT_MAX,0,0));

    //NewtonConstraintCreateUpVector(dynamic_cast<PhysicsZone*>(m_pShape->getPhysicsZone().get())->getZonePtr(), _upVector.m_array, dynamic_cast<PhysicsActor*>(m_pShape.get())->getActorPtr());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZBullet
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
