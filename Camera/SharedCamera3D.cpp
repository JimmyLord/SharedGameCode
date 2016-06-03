//
// Copyright (c) 2012-2016 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

//#include "GameCommonHeader.h"
#include "SharedCamera3D.h"

SharedCamera3D::SharedCamera3D()
: m_Eye(0,0,10)
, m_Up(0,1,0)
, m_LookAtPos(0,0,0)
, m_Width(0)
, m_Height(0)
{
    m_matView.SetIdentity();
    m_matProj.SetIdentity();
    m_matViewProj.SetIdentity();
}

SharedCamera3D::~SharedCamera3D()
{
}

void SharedCamera3D::LookAt(Vector3& eye, Vector3& up, Vector3& lookatpos)
{
    m_Eye = eye;
    m_Up = up;
    m_LookAtPos = lookatpos;
}

void SharedCamera3D::ResetMatrix(bool calculateinverse)
{
#if MYFW_RIGHTHANDED
    m_matView.CreateLookAtView( m_Eye, m_Up, m_LookAtPos );
#else
    m_matView.CreateLookAtViewLeftHanded( m_Eye, m_Up, m_LookAtPos );
#endif

    m_matViewProj = m_matProj * m_matView;

    // Calculate the inverse of the camera matrix
    if( calculateinverse )
    {
        m_matViewProjInverse = m_matViewProj;
        m_matViewProjInverse.Inverse();

        // Calculate the x and y values of the frustum at Z=0
        Vector3 sspos( 1, 1, 0 );
        Vector3 pos3d = m_matViewProjInverse * sspos;

        m_FrustumRightEdgeZ0 = pos3d.x;
        m_FrustumTopEdgeZ0 = pos3d.y;
    }
}
