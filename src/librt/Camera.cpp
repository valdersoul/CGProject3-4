//------------------------------------------------------------------------------
// Copyright 2015 Corey Toler-Franklin, University of Florida
// Camera.cpp
// Camera Object - position, up and lookat
//------------------------------------------------------------------------------

#include "Camera.h"
#include <assert.h>
#include <stdio.h>
#include <string>
#include "STMatrix4.h"
#include "utilities.h"
#include "defs.h"

// TO DO: Proj3_4 OpenGL
//-----------------------------------
// Adjust this camera implementation
// to support the camera rotation and translation modes
// Make sure that the camera viewing and projection
// matrices are updated in the scene graph
//-----------------------------------


// construct the camera
Camera::Camera(void)
    : m_trackballsize   (0.8f)
{
    Reset();
    m_rotationMode = ORBIT;
}


Camera::~Camera()
{

}

// reset the camera basis so camera is up right
void Camera::SetUpAndRight(void)
{
    m_Right = STVector3::Cross(m_LookAt - m_Position, m_Up);
    m_Right.Normalize();
    m_Up = STVector3::Cross(m_Right, m_LookAt - m_Position);
    m_Up.Normalize();
}

// reset the camera position to (0,0,0) looking down negative z
void Camera::Reset(void) 
{
    m_LookAt=STVector3(0.f,0.f,0.f);
    m_Position=STVector3(0.f,0.f,0.f);
    m_Up=STVector3(0.f,1.f,-1.f);

    SetUpAndRight();
}

// resets the camera up vector
void Camera::ResetUp(void)
{
    m_Up = STVector3(0.f,1.f,0.f);
    m_Right = STVector3::Cross(m_LookAt - m_Position, m_Up);
    m_Right.Normalize();
    m_Up = STVector3::Cross(m_Right, m_LookAt - m_Position);
    m_Up.Normalize();
}


// TO DO: Proj3_4 OpenGL 
// Complete this function
// rotates about the center of an object
// axis - axis of rotation, 
// p1x, p1y - coordinates of mouse START position in screen coords
// p2x, p2y - coordinates of mouse END position in screen coords
void Camera::Orbit(float axis[4], float p1x, float p1y, float p2x, float p2y)
{

    // TO DO: Proj3_4 OpenGL 
    // Orbit - rotates about an object
    // This is a analogous to a virtual trackball 
    // your job is to update camera view matrix and projection matrix
    // this will then effect the camer node in the scene
    //-----------------------------------------------------

    //-----------------------------------------------------
}

void Camera::Rotate(float delta_x, float delta_y)
{

    // TO DO: Proj3_4 OpenGL
    // 1. Alter this code so that there are two camera rotation options
    // 2. Fly - rotates around a viewpoint
    // Orbit - rotates about an object (see Camera::Orbit)
    // 3. All rotations are about the origin in world space
    // 4. Compute the coordinates of the world origin in camera space
    // 5. Contruct your rotation about this computed point
    //-------------------------------------------------------------------------------


    //-------------------------------------------------------------------------------
}


void Camera::Zoom(float delta_y)
{
    STVector3 direction = m_LookAt - m_Position;
    float magnitude = direction.Length();
    direction.Normalize();
    float zoom_rate = 0.1f*magnitude < 0.5f ? .1f*magnitude : .5f;
    if(delta_y * zoom_rate + magnitude > 0)
    {
        m_Position += (delta_y * zoom_rate) * direction;
    }
}


void Camera::Strafe(float delta_x, float delta_y)
{
    float strafe_rate = 0.05f;

    m_Position -= strafe_rate * delta_x * m_Right;
    m_LookAt   -= strafe_rate * delta_x * m_Right;
    m_Position += strafe_rate * delta_y * m_Up;
    m_LookAt   += strafe_rate * delta_y * m_Up;
}


STVector3 Camera::Position (void)
{
    return(m_Position);
}


STVector3 Camera::LookAt (void)
{
    return(m_LookAt);
}


STVector3 Camera::Up (void)
{
    return(m_Up);
}

//------------------------------------------------------------------------------------
// TO DO: Proj3_4 OpenGL
// These are all of the new members added to the Camera class for this assignment
//------------------------------------------------------------------------------------

STVector3 Camera::Right (void)
{
    return(m_Right);
}


STMatrix4 Camera::GetProjectionMatrix (void)
{
    return(m_MP);
}


STMatrix4 Camera::GetViewMatrix (void)
{
    return(m_MV);
}


void Camera::SetProjectionMatrix (STMatrix4 m)
{
    m_MP = m;
}


void Camera::SetViewMatrix (STMatrix4 m)
{
    m_MV = m;
}


void Camera::ToggleRotationMode (void)
{
    if(m_rotationMode == ORBIT)
        m_rotationMode = FLY;
    else if(m_rotationMode == FLY)
        m_rotationMode = ORBIT;
}


void Camera::SetLookAt(STVector3 lookat)
{
    m_LookAt = lookat;
}

void Camera::SetPosition(STVector3 position)
{
    m_Position = position;
}

void Camera::SetUp(STVector3 up)
{
    m_Up = up;
}


//-----------------------------------------------------------------------------------------
