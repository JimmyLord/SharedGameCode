//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __SharedCamera3D_H__
#define __SharedCamera3D_H__

class SharedCamera3D
{
public:
    Vector3 m_Eye;
    Vector3 m_Up;
    Vector3 m_LookAtPos;

    float m_Width;
    float m_Height;

    MyMatrix m_matView;
    MyMatrix m_matProj;
    MyMatrix m_matViewProj;

    MyMatrix m_matViewProjInverse;
    float m_FrustumRightEdgeZ0;
    float m_FrustumTopEdgeZ0;

public:
    SharedCamera3D();
    ~SharedCamera3D();

    void LookAt(Vector3& eye, Vector3& up, Vector3& lookatpos);

    void ResetMatrix(bool calculateinverse = false);
};

#endif //__SharedCamera3D_H__
