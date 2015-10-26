//------------------------------------------------------------------------------
// Copyright 2015 Corey Toler-Franklin, University of Florida
// Scene.cpp
// Stores all the objects - lights, camera and surfaces for setting up the scene.
//------------------------------------------------------------------------------



// GLEW
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "Scene.h"
#include <assert.h>
#include <stdio.h>
#include <string>
#include "Intersection.h"
#include "Surface.h"
#include "Light.h"
#include "Sphere.h"
#include "STMatrix4.h"
#include "STVector2.h"
#include "CameraNode.h"
#include "TransformNode.h"
#include "LightNode.h"
#include "GeometryNode.h"
#include "utilities.h"
#include "STVector3.h"



Scene::Scene(void)
    : m_background(RGBR_f(0, 0, 0, 1)),
      m_pCamera(NULL),
      m_renderMode(LAMBERTIAN),
      m_bHasManipulator (false),
      m_manipMotionType(MANIP_NONE), 
      m_activeManipGeom(AXIS_NONE)
 {
    // init camera
    m_pCamera = new Camera();


//------------------------------------------------------------------------------------
// TO DO: Proj3_4 OpenGL
// Add initial node to the scene graph
// 1. Create a new CameraNode
// 2. Initialize the node with the camera
// 3. Create a TranfromNode for the camera based on the light position
// 4. Add the TransfromNode and the camera to the scene tree
//------------------------------------------------------------------------------------
   


//-------------------------------------------------------------------------------------

    InitManipMode();
}


Scene::~Scene()
{
}


void Scene::SetBackgroundColor(RGBR_f color)
{
    m_background = color;
}


RGBR_f Scene::GetBackgroundColor(void)
{
    return(m_background);
}


void Scene::AddLight(Light lightin)
{
    m_lights.push_back(lightin);

//------------------------------------------------------------------------------------
// TO DO: Proj3_4 OpenGL
// Add the light to the scene graph
// 1. Create a new LightNode
// 2. Initialize the node with the light
// 3. Create a TranfromNode for the light based on the light position
// 4. Add the TransfromNode and the light to the scene
//------------------------------------------------------------------------------------
    

//------------------------------------------------------------------------------------

}


void Scene::AddSurface(Surface *pSurfacein)
{

    m_surfaceList.push_back(pSurfacein);

//------------------------------------------------------------------------------------
// TO DO: Proj3_4 OpenGL
// Add the geometry to the scene graph
// 1. Create a new GeometryNode
// 2. Initialize the node with the surface
// 3. Create a TranfromNode for the surface based on the surface position
// 4. Add the TransfromNode and the surface to the scene
//------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------
}


Camera *Scene::GetCamera(void)
{
    return(m_pCamera);
}


// computes the direction of the light in the scene
// and returns it
STVector3 Scene::GetLightDirection(void)
{
    STVector3 lightDirection;

    // TO DO: Proj2 raytracer
    // CAP5705 - Compute light direction.
    // 1. Return the direction of the light in the scene
    //---------------------------------------------------------
    //---------------------------------------------------------

    return(lightDirection);
}


// Select the closest intersection and return the number of points
// very close to this one
int Scene::SelectClosest(IntersectionList *pIntersectionList, Intersection *pIntersection) 
{

    int numPoints = 0;

    // TO DO: Proj2 raytracer
    // CAP5705 - Find the closest intersection.
    // 1. Find the closest (over all objects) intersection and update pIntersection
    // 2. return the number of really close points if there is a cluster
    //---------------------------------------------------------

    //---------------------------------------------------------

    return(numPoints);
}


//-----------------------------------------------------
// Find the intersection of the ray with objects in the scene
// Return the closest intersection
//-----------------------------------------------------
int Scene::FindClosestIntersection(Ray ray, Intersection *pIntersection)
{
    int numPoints = 0;
    numPoints = FindIntersection(ray, pIntersection, false);
    return(numPoints);
}


//-----------------------------------------------------
// Find the intersection of the ray with objects in the scene
// Checks for the closest intersections and retuns the number
// of close intersections found
// Updates the pIntersection 
//   -if bAny is true, return the first intersection found
//   - if bAny is false, return the closest intersection
// 
//-----------------------------------------------------
int Scene::FindIntersection(Ray ray, Intersection *pIntersection, bool bAny)
{
    bool bFound = false;
    int numPoints = 0;

    IntersectionList intersectionList;

    SurfaceList::const_iterator iter = m_surfaceList.begin();
    SurfaceList::const_iterator end  = m_surfaceList.end();

    for (; iter != end; ++iter) {

        // TO DO: Proj2 raytracer
        // CAP5705 - Find Intersections.
        // 1. Find intersections with objects in the scene
        // 2. If bAny is true, return as soon as you find one
        //    Do not forget to update the pIntersection before returning
        // 3. Othersize just add to the list of intersections
        //---------------------------------------------------------

    }

    // TO DO: Proj2 raytracer
    // CAP5705 - Find Intersections.
    // 1. Find the closest intersection along the ray in the list
    //---------------------------------------------------------
    //---------------------------------------------------------

    return(numPoints);
}


//-----------------------------------------------------
// clear the scene by removing surfaces
//-----------------------------------------------------
void Scene::Clear(void)
{
    SurfaceList::iterator iter = m_surfaceList.begin();
    SurfaceList::iterator end  = m_surfaceList.end();

    // delete objects
    for (; iter != end; ++iter) {
        delete *iter;
        *iter = NULL;
    }

    // clean up
    m_surfaceList.clear();
}


//------------------------------------------------------------------------------------
// TO DO: Proj3_4 OpenGL
// These are all of the new functions added to the scene class for this assignment
//------------------------------------------------------------------------------------


STVector3 Scene::GetLightPosition(void)
{
    if(m_lights.size())
        return(m_lights[0].GetPosition());

    STVector3 p(10.0f, 15.0f, 10.0f);
    return(p);
}


// toggles between modes
void Scene::ToggleRenderMode(void)
{
    if(m_renderMode == NAVIGATION) {
        RemoveManip();
        InitManipMode();
        m_renderMode = ENVIRONMENTMAP;
    }
    else if(m_renderMode == ENVIRONMENTMAP) {
        m_renderMode = NAVIGATION;
        InitManipMode();
    }
}

// toggles between modes
void Scene::ToggleManipMode(void)
{
    if(m_renderMode == NAVIGATION) {
        m_modeQueue.push(m_modeQueue.front());
        m_modeQueue.pop();
    }
}


// toggles between modes
// make different parts of the geometry visible
void Scene::ToggleManipGeometry(void)
{
    if((m_renderMode == NAVIGATION) && (m_bHasManipulator)){
        m_manipGeomQueue.push(m_manipGeomQueue.front());
        m_manipGeomQueue.pop();
    }
}


// Inits the current mode
void Scene::InitManipMode(void)
{
    std::queue<ManipMode> empty;
    std::swap(m_modeQueue, empty);
    m_modeQueue.push(LOCAL);
    m_modeQueue.push(PARENT);
    m_modeQueue.push(NONE);

    InitManipGeometryMode();
    RemoveManip();
}


// Inits the current manip geometry mode
// helps with selection to show individual parts
void Scene::InitManipGeometryMode(void)
{
    std::queue<ManipGeom> empty;
    std::swap(m_manipGeomQueue, empty);
    m_manipGeomQueue.push(AXIS_ALL);
    m_manipGeomQueue.push(AXIS_TRANSXYZ);
    m_manipGeomQueue.push(AXIS_ROTATIONALL);
}


//
// axis - manipulator axis
// matrix4 - view projection
// Object - selected object
// prevMousePos - previous mouse position
// curMousePos - current mouse position
void Scene::Translate(int axis, STMatrix4 viewProjection, SceneNode object, STVector2 prevMousePos, STVector2 curMousePos)
{

    // TO DO: Proj3_4 OpenGL
    // Consider M - The object's transformation, T the adjustment
    // 1. Check which manipulation mode the program is in - the mode in the front of m_modeQueue
    // 2. For local mode - transform in the objects coordinate system, M = M * T
    // 3. For parent mode - transform in relation to the object's parent, M = T * M
    // 4. Formulate the transform along the axis by delta related to 
    //    the product of the difference in mouse positions and a constant term
    // 5. Make your manipulator move with the mouse position. 
    //    The point you press on should remain under the mouse as it moves
    //    Form a ray corresponding to the mouse click and 
    //    the line corresponding to the manipulation axis in world space
    //    Compute the t intervals along the axis that correspond to the mouse movement.
    //-----------------------------------------------------------------------------------------
    

    //--------------------------------------------------------------------------------------------
}


//
// axis - manipulator axis
// matrix4 - view projection
// Object - selected object
// prevMousePos - previous mouse position
// curMousePos - current mouse position
void Scene::Rotate(int axis, STMatrix4 viewProjection, SceneNode object, STVector2 prevMousePos, STVector2 curMousePos)
{

    // TO DO: Proj3_4 OpenGL
    // Consider M - The object's transformation, T the adjustment
    // 1. Check which manipulation mode the program is in - the mode in the front of m_modeQueue
    // 2. For local mode - transform in the objects coordinate system, M = M * T
    // 3. For parent mode - transform in relation to the object's parent, M = T * M
    // 4. Formulate the transform along the axis by delta related to 
    //    the product of the difference in mouse positions and a constant term
    // 5. Make your manipulator move with the mouse position. 
    //    The point you press on should remain under the mouse as it moves
    //    Form a ray corresponding to the mouse click and 
    //    the line corresponding to the manipulation axis in world space
    //    Compute the t intervals along the axis that correspond to the mouse movement.
    //---------------------------------------------------------------------------------

    //--------------------------------------------------------------------------------------
}


// spins the scene
void Scene::Spin(float QMat1[4], float QMat2[4], float QMat3[4])
{
    combineQ(QMat1, QMat2, QMat3);
    glutPostRedisplay();
}


// Propogates group transformations to child nodes
void Scene::PropogateTransforms(SceneNode *pNode)
{
    // TO DO: Proj3_4 OpenGL
    // Consider A - the Object's parent's world transform
    //          B - the Object's local transform
    // 1. Implement a recursive function that:
    // 2. for an Object that is not at the root
    //             sets m_worldT and m_worldInvT with the Object
    //             m_worldT should be set to AB
    //             m_worldIT should be set to transform the normal vectors
    // 3. Finally traverse the tree making a recursive call o each of the Object's children
    //----------------------------------------------------------------------------------


    //---------------------------------------------------------------------------------
}


void Scene::SetRenderMode (RenderMode mode)
{
    m_renderMode = mode;
}


RenderMode Scene::GetRenderMode (void)
{
    return(m_renderMode);
}


void Scene::AddManipulator(void)
{
    // use the provided bopunding box to add a manipulator
    // to the scene
    // if this is true, the manipulator is drawn

    m_bHasManipulator = true;
}


// True if you are rotating the manipulator
bool Scene::RotatingManip(void)
{
    if   ( (RenderMode() == NAVIGATION)
        && (HasManipulator())
        && ((CurrentManipMode() == LOCAL) || (CurrentManipMode() == PARENT))
        && ((CurrentManipGeometryState() == AXIS_ALL) || 
            (CurrentManipGeometryState() == AXIS_ROTATIONALL))
        && ((CurrentManipMotion() == ROTATE_X) || 
            (CurrentManipMotion() == ROTATE_Y) || 
            (CurrentManipMotion() == ROTATE_Z) || 
            (CurrentManipMotion() == ROTATE_SCREEN) || 
            (CurrentManipMotion() == ROTATE_DUPLICATE)))
        return(true);

    return(false);
}


// True if you are rotating the manipulator
bool Scene::TranslatingManip(void)
{
    if   ( (RenderMode() == NAVIGATION)
        && (HasManipulator())
        && ((CurrentManipMode() == LOCAL) || (CurrentManipMode() == PARENT))
        && ((CurrentManipGeometryState() == AXIS_ALL) || 
            (CurrentManipGeometryState() == AXIS_TRANSXYZ))
        && ((CurrentManipMotion() == TRANS_YZ) || 
            (CurrentManipMotion() == TRANS_XZ) || 
            (CurrentManipMotion() == TRANS_XY) || 
            (CurrentManipMotion() == TRANS_X)  || 
            (CurrentManipMotion() == TRANS_Y)  ||
            (CurrentManipMotion() == TRANS_Z)))
        return(true);

    return(false);
}


bool Scene::HasManipulator(void)
{
    return(m_bHasManipulator);
}


bool Scene::SelectManipulator(int mouseX, int mouseY, int width, int height)
{

    double matModelView[16], matProjection[16]; 
    int viewport[4]; 
    double t = 0.0;
    STVector3 closestPt;
    bool bHit = false;

    glGetDoublev(GL_MODELVIEW_MATRIX, matModelView ); 
    glGetDoublev(GL_PROJECTION_MATRIX, matProjection ); 
    glGetIntegerv(GL_VIEWPORT, viewport ); 
    double winX = (double)mouseX; 
    double winY = viewport[3] - (double)mouseY; 

    GLdouble   m_startX, m_startY, m_startZ;
    GLdouble   m_endX,   m_endY,   m_endZ;

    gluUnProject(winX, winY, 0.0, matModelView, matProjection, 
                 viewport, &m_startX, &m_startY, &m_startZ); 
    gluUnProject(winX, winY, 1.0, matModelView, matProjection, 
                 viewport, &m_endX, &m_endY, &m_endZ); 


    // TO DO: Proj3_4 OpenGL
    // Complete this code block
    // Check if ray hits manipulator and update the
    // active geometry and manip motion type
    // This sis similar to SelectObject, but this time
    // checks for manipulator hits
    //--------------------------------------------------------

    // see if the ray hits the manipulator
    ManipGeom selectedGeom;
    bHit = RayTestManip(STVector3(m_startX, m_startY, m_startZ), STVector3(m_endX, m_endY, m_endZ), &selectedGeom);

    if(bHit) {
       SetActiveManipGeom(selectedGeom);
       ManipMotionType mtype = MapGeomToMotion(selectedGeom);
       SetCurrentManipMotion(mtype);
    }

    //--------------------------------------------------------

    return(bHit);
}



void Scene::RemoveManip(void)
{
    m_bHasManipulator = false;
}


// Returns true if an object was selected
// Otherwise returns false
// Uses bounding spheres of groups for intersection tests
bool Scene::SelectObject(int mouseX, int mouseY, int width, int height, STPoint3 bspherecenter, float bsphereradius)
{

    double matModelView[16], matProjection[16]; 
    int viewport[4]; 
    double t = 0.0;
    STVector3 closestPt;
    bool bHit = false;

    glGetDoublev(GL_MODELVIEW_MATRIX, matModelView ); 
    glGetDoublev(GL_PROJECTION_MATRIX, matProjection ); 
    glGetIntegerv(GL_VIEWPORT, viewport ); 
    double winX = (double)mouseX; 
    double winY = viewport[3] - (double)mouseY; 

    GLdouble   m_startX, m_startY, m_startZ;
    GLdouble   m_endX,   m_endY,   m_endZ;

    gluUnProject(winX, winY, 0.0, matModelView, matProjection, 
                 viewport, &m_startX, &m_startY, &m_startZ); 
    gluUnProject(winX, winY, 1.0, matModelView, matProjection, 
                 viewport, &m_endX, &m_endY, &m_endZ); 

    STVector3 center(bspherecenter.x, bspherecenter.y, bspherecenter.z);
    bHit = RayTest(center, bsphereradius, center,
                   STVector3(m_startX, m_startY, m_startZ), 
                   STVector3(m_endX, m_endY, m_endZ),
                   &closestPt, &t);

    return(bHit);
}

// TO DO: Proj3_4 OpenGL
// map the geometry part to the motion
//    AXIS_ROTX    - ROTATE_X                
//    AXIS_ROTY    - ROTATE_Y               
//    AXIS_ROTZ    - ROTATE_Z              
//
//   AXIS_TRANSX   - TRANS_X                
//   AXIS_TRANSY   - TRANS_Y                 
//   AXIS_TRANSX    - TRANS_Z                 
//
//   AXIS_ROTSCREEN - ROTATE_SCREEN
//   AXIS_DUPLICATE - ROTATE_DUPLICATE
ManipMotionType  Scene::MapGeomToMotion(ManipGeom selectedGeom)
{
    ManipMotionType mType;

    switch(selectedGeom) {
        case AXIS_TRANSX: mType = TRANS_X; break;
        default:mType = MANIP_NONE; break;
    }

    return(mType);
}


// TO DO: Proj3_4 OpenGL
// Complete this function
// Check if ray hits manipulator and update the
// active geometry and manip motion type
// This is similar to SelectObject, but this time
// checks for manipulator hits
//--------------------------------------------------------
bool Scene::RayTestManip(STVector3 raystart, STVector3 rayend, ManipGeom *selectedGeom)
{
    return(false);
}

// returns identifier for the current manipulation mode LOCAL or PARENT or NONE
ManipMode Scene::CurrentManipMode(void)
{
    ManipMode mmanipmode = m_modeQueue.front();
    return (mmanipmode);
}

// returns identifier for the current visible manipulator geometry
ManipGeom Scene::CurrentManipGeometryState(void)
{
    ManipGeom mmanipGeomState = m_manipGeomQueue.front();
    return (mmanipGeomState);
}


// returns identifier for the current manipulation
ManipMotionType Scene::CurrentManipMotion(void)
{
    return (m_manipMotionType);
}

// gets identifier for the current manipulation
void Scene::SetCurrentManipMotion(ManipMotionType mmotiontype)
{
    m_manipMotionType = mmotiontype;
}


// returns identifier for the selected manip geometry part
ManipGeom Scene::GetActiveManipGeom(void)
{
    return (m_activeManipGeom);
}

// gets identifier for the selected manip geometry part
void Scene::SetActiveManipGeom(ManipGeom mgeom)
{
    m_activeManipGeom = mgeom;
}



//-----------------------------------------------------------------------------------------