//
// Copyright (c) 2012-2014 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software
// in a product, an acknowledgment in the product documentation would be
// appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
// misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "GameCommonHeader.h"

#include "Camera2D.h"

#if MYFW_WP8
#include "../../SourceWP8/WP8Main.h"
#endif

Camera2D::Camera2D()
{
    m_OffsetX = 0;
    m_OffsetY = 0;
    m_Width = 0;
    m_Height = 0;
    m_Zoom = 1;

    m_matView.SetIdentity();
    m_matProj.SetIdentity();
    m_matViewProj.SetIdentity();
}

Camera2D::~Camera2D()
{
}

void Camera2D::SetViewSize(float width, float height)
{
    m_Width = width;
    m_Height = height;

    ResetMatrix();
}

void Camera2D::SetPos(float newx, float newy)
{
    m_OffsetX = newx;
    m_OffsetY = newy;

    ResetMatrix();
}

void Camera2D::SetZoom(float newzoom)
{
    m_Zoom = newzoom;

    ResetMatrix();
}

void Camera2D::MoveBy(float offx, float offy, float zoom)
{
    m_OffsetX += offx;
    m_OffsetY += offy;
    m_Zoom += zoom;

    float width;
    float height;

    float offsetx;
    float offsety;

    QueryOffsetAndSize( &offsetx, &offsety, &width, &height );

    ResetMatrix();
}

void Camera2D::ResetMatrix()
{
    float width;
    float height;

    float offx;
    float offy;

    QueryOffsetAndSize( &offx, &offy, &width, &height );

    float scrw = g_pGame->m_GameWidth;
    float scrh = g_pGame->m_GameHeight;

    //float zoomoffsetx = 0;//(g_pGame->m_ScreenWidth - scrw/m_Zoom)/2;
    //float zoomoffsety = 0;//(g_pGame->m_ScreenWidth - scrh/m_Zoom)/2;

    m_matView.SetIdentity();
    //m_matView.Scale( m_Zoom, m_Zoom, 1 );
    m_matView.SetTranslation( - ( m_OffsetX - (m_Width/m_Zoom)/2 ) ,//- (m_Width/m_Zoom)/2 ),
                              - ( m_OffsetY - (m_Height/m_Zoom)/2 ),//- m_Height/2 ),
                              0 );

    MyMatrix scale;
    scale.SetIdentity();
    scale.Scale( m_Zoom, m_Zoom, 1 );
    m_matView = scale * m_matView;
    //m_matView.SetPosition( (-m_OffsetX + m_Width/2)*m_Zoom - zoomoffsetx,
    //                       (-m_OffsetY + m_Height/2)*m_Zoom - zoomoffsety, 0 );

    {
        float ortholeft = - g_pGame->m_ScreenOffsetX/g_pGame->m_ScreenWidth * scrw;
        float orthoright = scrw + g_pGame->m_ScreenOffsetX/g_pGame->m_ScreenWidth * scrw;
        float orthobottom = - g_pGame->m_ScreenOffsetY/g_pGame->m_ScreenHeight * scrh;
        float orthotop = scrh + g_pGame->m_ScreenOffsetY/g_pGame->m_ScreenHeight * scrh;

        //float zoomoffsetx = (g_pGame->m_GameWidth - scrw);
        //float zoomoffsety = (g_pGame->m_GameHeight - scrh);

        //m_matProj.SetOrtho( ortholeft+zoomoffsetx, orthoright+zoomoffsetx,
        //                    orthobottom+zoomoffsety, orthotop+zoomoffsety, 1, -1 );

        m_matProj.CreateOrtho( ortholeft, orthoright, orthobottom, orthotop, 1, -1 );

#if MYFW_WP8
        // orientation hack for WP8
        if( g_pWP8App->GetOrientation() == WP8Orientation_Landscape90 )
        {
            MyMatrix rot;
            rot.SetIdentity();
            rot.Rotate( 90, 0, 0, 1 );
            m_matProj.Multiply( &rot );
        }
        else if( g_pWP8App->GetOrientation() == WP8Orientation_Landscape270 )
        {
            MyMatrix rot;
            rot.SetIdentity();
            rot.Rotate( 270, 0, 0, 1 );
            m_matProj.Multiply( &rot );
        }
#endif

        m_matViewProj = m_matProj * m_matView;
    }
}

void Camera2D::QueryOffsetAndSize( float* offx, float* offy, float* width, float* height )
{
    *width = m_Width;
    *height = m_Height;

    *offx = m_OffsetX - *width/2;
    *offy = m_OffsetY - *height/2;
}
