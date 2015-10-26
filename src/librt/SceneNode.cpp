//------------------------------------------------------------------------------
// Copyright 2015 Corey Toler-Franklin, University of Florida
// SceneNode.cpp
// Base class for scene nodes.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// TO DO: Proj3_4 OpenGL
// This is a new file added for assignment Proj3_4 OpenGL
//------------------------------------------------------------------------------------
#include "SceneNode.h"
#include <assert.h>
#include <stdio.h>
#include <string>




SceneNode::SceneNode(void) 
{ 
}


// destructor - calls destroy
SceneNode::~SceneNode(void) 
{ 
    Destroy();
}


// release this object from memory
void SceneNode::Release(void) 
{ 
    delete this; 
}


// update our scene node
void SceneNode::Update(void)
{
    // loop through the list and update the children
    for( std::list<SceneNode*>::iterator i = m_lstChildren.begin(); i != m_lstChildren.end(); i++ )
    {
      (*i)->Update();
    }
}


// destroy all the children
void SceneNode::Destroy(void)
{
    for( std::list<SceneNode*>::iterator i = m_lstChildren.begin();
        i != m_lstChildren.end(); i++ )
        (*i)->Release();

    m_lstChildren.clear();
}


// add a child to our custody
 void SceneNode::AddChild(SceneNode* pNode )
{
    m_lstChildren.push_back(pNode);
}


// get Children
std::list<SceneNode*> SceneNode::GetChildren(void)
{
    return(m_lstChildren);
}


