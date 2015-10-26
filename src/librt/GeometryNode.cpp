//------------------------------------------------------------------------------
// Copyright 2015 Corey Toler-Franklin, University of Florida
// GeometryNode.cpp
// Geometry Node.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// TO DO: Proj3_4 OpenGL
// This is a new file added for assignment Proj3_4 OpenGL
//------------------------------------------------------------------------------------
#include "GeometryNode.h"
#include <assert.h>
#include <stdio.h>
#include <string>





GeometryNode::GeometryNode(void) 
{ 
}


GeometryNode::~GeometryNode(void) 
{ 
}


void GeometryNode::Initialize(Surface *pSurface)
{
    m_pSurface = pSurface;
}



void GeometryNode::Update(void)
{
    //------------------------------------------------------------------------------------
    // TO DO: Proj3_4 OpenGL
    // Draw the surface or your triangle mesh here
    //------------------------------------------------------------------------------------


  // Draw  geometry 

  SceneNode::Update();
}


