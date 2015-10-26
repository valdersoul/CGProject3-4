//--------------------------------------------------------
// CAP5705 Computer Graphics
// University of Florida
// Copyright 2015 Corey Toler-Franklin
//--------------------------------------------------------



// System
#include <string>
#include <algorithm>
#include <stdio.h>
#include <stdexcept>
#include<map>
#include <stdlib.h>
#include <assert.h>
using namespace std;

// GLEW
#define GLEW_STATIC
#include "stglew.h"
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

// stlib and rtlib
#include "STColor4ub.h"
#include "STImage.h"
#include "defs.h"
#include "Scene.h"
#include "utilities.h"
#include "STVector4.h"


// forward declarations
void MouseMotionCallback(int x, int y);
void MouseCallback(int button, int state, int x, int y);
void ManipulationMode(int button, int state, int x, int y);
void KeyCallback(unsigned char key, int x, int y);
void SpecialKeyCallback(int key, int x, int y);
void ReshapeCallback(int w, int h);
void Setup(void);
void menu(int item);
void CleanUp (void);
void GenerateCubeMap(void);
void LoadCubeFace(GLenum target, char *filename);
void GenerateTextureCorrdinates(void);
void SetUpEnvironmentMap(void);
void Spin (void);
void BoundingSphere(STPoint3 *center, float *radius);
void DrawQuadrant(STVector3 origin, float size, bool bSelected, STVector3 axisU, STVector3 axisV);
void DrawCircle(STVector3 origin, RGBR_f color, STVector3 Xvert, STVector3 Yvert);
void DrawCircleHalf(STVector3 origin, RGBR_f color, STVector3 Xvert, STVector3 Yvert, float camPlane[4]);
void DrawAxis(STVector3 origin, RGBR_f color, STVector3 axis, STVector3 Xvert, STVector3 Yvert, float fct, float fct2, float col[4]);
void DrawCam(STVector3 origin, STVector3 Xvert, STVector3 Yvert, float ng);
void DrawManipulator(void);
float VProjScreenFactor (void);
void ViewProjectionScreenSpace(STMatrix4 M);
void ProcessRotation(int mouseX, int mouseY);
void ProcessTranslation(void);



// globals
// a neccessary evil for most glut programs
//---------------------------------

// scene
Scene       *pScene = NULL;         // scene geometry and lights
RenderMode startMode = NAVIGATION;

int globalCount;

typedef struct {
  int i1;
  int i2; 
  int i3;
} TriangleIndices;


// Window size, kept for screenshots
static int gWindowSizeX = 800;
static int gWindowSizeY = 800;

// shader files
std::string envmapFragmentShader;
std::string envmapVertexShader;
std::string reflectionFragmentShader;
std::string reflectionVertexShader;
std::string vertexShader;
std::string fragmentShader;
std::string normalMap;
std::string displacementMap;

// shaders
STShaderProgram *shader;
STShaderProgram *envmapshader;
STShaderProgram *reflectanceshader;

//mesh files
std::string meshOBJ;

//textures
STImage   *surfaceNormImg;
STTexture *surfaceNormTex;
STImage   *surfaceDisplaceImg;
STTexture *surfaceDisplaceTex;


// Stored mouse position for camera rotation, panning, and zoom.
int gPreviousMouseX = -1;
int gPreviousMouseY = -1;
int gMouseButton = -1;
bool bSpin = false;
bool bMove = false;
int beginx;
int beginy;
float curQMat[4];
float lastQMat[4];


// mesh
bool mesh = false; // draw mesh
bool smooth = true; // smooth/flat shading for mesh
bool normalMapping = true; // true=normalMapping, false=displacementMapping
bool proxyType=false; // false: use cylinder; true: use sphere
std::vector<STTriangleMesh*> gTriangleMeshes;
STPoint3 gMassCenter;
std::pair<STPoint3,STPoint3> gBoundingBox;
STTriangleMesh* gManualTriangleMesh = 0;
int TesselationDepth = 100;

// view
float vpscreenfactor = 1.0;
float screenScale = 1.0;
float Ng2 = 0;
STVector3 gblXVert;
STVector3 gblYVert;


// lights
static float ambientLight[]  = {0.20, 0.20, 0.20, 1.0};
static float diffuseLight[]  = {1.00, 1.00, 1.00, 1.0};
static float specularLight[] = {1.00, 1.00, 1.00, 1.0};
float lightPosition[] = {10.0f, 15.0f, 10.0f, 1.0f};

// Cube map textures
static GLenum CubeFaceTarget[6] = {
  GL_TEXTURE_CUBE_MAP_POSITIVE_X,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
  GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
  GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};


//------------------------------------------------------------------------------------
// TO DO: Proj3_4 OpenGL
// Here is where you specify the path to the cube map files
// You have been given files in ../../data/images/stpeters
// To load other cubmaps, replace these mages with your cubemap files in the correct order
// It is optional and not required to test other cube maps
//------------------------------------------------------------------------------------
char *cmapFiles[6] = {
  "../../data/images/stpeters/negx.jpg", 
  "../../data/images/stpeters/posx.jpg", 
  "../../data/images/stpeters/posy.jpg", 
  "../../data/images/stpeters/negy.jpg", 
  "../../data/images/stpeters/negz.jpg", 
  "../../data/images/stpeters/posz.jpg", 
};


// Load the cube faces
void LoadCubeFace(GLenum target, char *filename)
{
    FILE *file;

    // load image and get pixels
    STImage *image = new STImage(filename);
    const STColor4ub* pixels = image->GetPixels();
 
    // build mipmaps
    gluBuild2DMipmaps(target, GL_RGBA,
    image->GetWidth(), image->GetHeight(),
    GL_RGBA, GL_UNSIGNED_BYTE, pixels);

}

// Generate textures
void GenerateTextureCoordinates(void)
{

    //------------------------------------------------------------------------------------
    // TO DO: Proj3_4 OpenGL
    // Generate texture coordinates (S, T, R), glTexGeni function
    //------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------
}


// set texture qrap flags
void SetTextureWrapFlags(void)
{
    //------------------------------------------------------------------------------------
    // TO DO: Proj3_4 OpenGL
    // Set the texture warp parameters and anyother parameters you might need here.
    //------------------------------------------------------------------------------------
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
    //--------------------------------------------------------------------------------------
}


// generates the cube map
void GenerateCubeMap(void)
{

    //------------------------------------------------------------------------------------
    // TO DO: Proj3_4 OpenGL
    // Generate the cube map
    // 1. LoadCubeFace()
    // 2. Set texture parameters for interpolation using mipmaps
    // 3. Enable the texture
    // 4. GenerateTextureCoordinates()
    // 5. SetTextureWrapFlags()
    // 6. Set glMatrixMode to  GL_TEXTURE
    // 7. Set glMatrixMode to  GL_MODELVIEW
    //------------------------------------------------------------------------------------
    for (int i=0; i<6; i++) {
        LoadCubeFace(CubeFaceTarget[i], cmapFiles[i]);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
      GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
    glEnable(GL_TEXTURE_CUBE_MAP);

    GenerateTextureCoordinates();
    SetTextureWrapFlags();

    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);

    glMatrixMode(GL_TEXTURE);
    glScalef(-1,-1,-1);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
    
    //---------------------------------------------
}

float ScreenFactor(void)
{
    return(vpscreenfactor);
}

void ViewProjectionScreenSpace(STMatrix4 M)
{
    // get matrices
    float modelMat_f[16];
    float projMat_f[16];    
    glGetFloatv (GL_MODELVIEW_MATRIX,  modelMat_f);  
    glGetFloatv (GL_PROJECTION_MATRIX, projMat_f); 

    STMatrix4 modelMat(modelMat_f);
    STMatrix4 projMat(projMat_f);

    float x, y, z;
    M.GetT(&x, &y, &z);

    // compute view matrix
    STMatrix4 viewProjMat = modelMat * projMat;
    STVector4 trans = vector4(x, y, z, 1.f);
    trans.Transform( viewProjMat );
    vpscreenfactor = screenScale * 0.15f * trans.w;
}




// Draw Geometry for manipulators
//----------------------------------------------------------------------

void DrawQuadrant(STVector3 origin, float size, bool bSelected, STVector3 axisU, STVector3 axisV)
{

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_CULL_FACE);

    STVector3 pts[4];
    pts[0] = origin;
    pts[1] = origin + (axisU * size);
    pts[2] = origin + (axisU + axisV)*size;
    pts[3] = origin + (axisV * size);

    if (!bSelected)
        glColor4f(1,1,0,0.5f);
    else
        glColor4f(1,1,1,0.6f);

    glBegin(GL_QUADS);
    glVertex3fv(&pts[0].x);
    glVertex3fv(&pts[1].x);
    glVertex3fv(&pts[2].x);
    glVertex3fv(&pts[3].x);
    glEnd();

    if (!bSelected)
        glColor4f(1,1,0.2f,1);
    else
        glColor4f(1,1,1,0.6f);

    glBegin(GL_LINE_STRIP);
    glVertex3fv(&pts[0].x);
    glVertex3fv(&pts[1].x);
    glVertex3fv(&pts[2].x);
    glVertex3fv(&pts[3].x);
    glEnd();

    glDisable(GL_BLEND);
}


void DrawCircle(STVector3 origin, RGBR_f color, STVector3 Xvert, STVector3 Yvert)
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glColor4f(color.r, color.g, color.b, 1);

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 50 ; i++)
    {
        STVector3 vt;
        vt = Xvert * cos((2*PI/50)*i);
        vt += Yvert * sin((2*PI/50)*i);
        vt += origin;
        glVertex3f(vt.x,vt.y,vt.z);
    }
    glEnd();
}

void DrawCircleHalf(STVector3 origin, RGBR_f color, STVector3 Xvert, STVector3 Yvert, float camPlane[4])
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glColor4f(color.r, color.g, color.b, 1);

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 30 ; i++)
    {
        STVector3 vt;
        vt = Xvert  * cos((PI/30)*i);
        vt += Yvert * sin((PI/30)*i);
        vt +=origin;
        STVector3 camvec(camPlane[0],camPlane[1],camPlane[2]);
        if (camvec.Dot(camvec, vt))
            glVertex3f(vt.x,vt.y,vt.z);
    }
    glEnd();
}

void DrawAxis(STVector3 origin, RGBR_f color, STVector3 axis, STVector3 Xvert, STVector3 Yvert, float fct, float fct2, float col[4])
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glColor4fv(col);
    glBegin(GL_LINES);
    glVertex3fv(&origin.x);
    glVertex3f(origin.x + axis.x, origin.y + axis.y, origin.z + axis.z);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    for (int i=0;i<=30;i++)
    {
        STVector3 pt;
        pt = Xvert * cos(((2*PI)/30.0f)*i)*fct;
        pt += Yvert * sin(((2*PI)/30.0f)*i)*fct;
        pt += axis*fct2;
        pt += origin;
        glVertex3fv(&pt.x);
        pt = Xvert * cos(((2*PI)/30.0f)*(i+1))*fct;
        pt+= Yvert * sin(((2*PI)/30.0f)*(i+1))*fct;
        pt+=axis*fct2;
        pt+=origin;
        glVertex3fv(&pt.x);
        glVertex3f(origin.x + axis.x, origin.y + axis.y, origin.z + axis.z);

    }
    glEnd();

}


void DrawCam(STVector3 origin, STVector3 Xvert, STVector3 Yvert, float ng)
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    int i = 0 ;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);

    glColor4f(1,1,0,0.5f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3fv(&origin.x);
    for (i = 0 ; i <= 50 ; i++)
    {
        STVector3 vt;
        vt  = Xvert * cos(((ng)/50)*i);
        vt += Yvert * sin(((ng)/50)*i);
        vt += origin;
        glVertex3f(vt.x,vt.y,vt.z);
    }
    glEnd();

    glDisable(GL_BLEND);


    glColor4f(1,1,0.2f,1);
    STVector3 vt;
    glBegin(GL_LINE_LOOP);

    glVertex3fv(&origin.x);
    for ( i = 0 ; i <= 50 ; i++)
    {
        STVector3 vt;
        vt  = Xvert * cos(((ng)/50)*i);
        vt += Yvert * sin(((ng)/50)*i);
        vt += origin;
        glVertex3f(vt.x,vt.y,vt.z);
    }

    glEnd();
}




void ProcessRotation(int mouseX, int mouseY)
{

    if(!pScene->HasManipulator())
        return;

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
    
    STVector3 raystart(m_startX, m_startY, m_startZ);
    STVector3 raysend(m_endX, m_endY, m_endZ);
    
    //------------------------------------------------------------------------------
    // TO DO: Proj3_4 OpenGL
    // Update the matrix transformation for the manipulator geometry
    // Update current model matrix
    // Use this information to call  pScene->Rotate();
    // 
    //------------------------------------------------------------------------------

    //---------------------------------------------------------------------------------
}



void ProcessTranslation(int mouseX, int mouseY)
{
    if(!pScene->HasManipulator())
        return;

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
    
    STVector3 raystart(m_startX, m_startY, m_startZ);
    STVector3 raysend(m_endX, m_endY, m_endZ);

    //------------------------------------------------------------------------------
    // TO DO: Proj3_4 OpenGL
    // Update the matrix transformation for the manipulator geometry
    // Update current model matrix
    // Use this information to call pScene->Translate()
    // 
    //------------------------------------------------------------------------------

    //---------------------------------------------------------------------------------


}


void DrawManipulator(void)
{

    // color
    RGBR_f colorRed(1, 0, 0, 1);
    RGBR_f colorGreen(0, 1, 0, 1);
    RGBR_f colorBlue(0, 0, 1, 1);
    RGBR_f colorWhite(1,1,1,1);
    RGBR_f colorMagenta(1.0f,0.3f,1.0f, 1.0f);
    RGBR_f colorGrey(0.2f,0.2f,0.2f, 1.0f);

    // a, y and z axis
    STVector3 xaxis = STVector3::eX;
    STVector3 yaxis = STVector3::eY;
    STVector3 zaxis = STVector3::eZ;

    // camera basis
    STVector3 up = pScene->GetCamera()->Up();
    STVector3 right = pScene->GetCamera()->Right();
    STVector3 dir = pScene->GetCamera()->LookAt();
    STVector3 front;
    front = front.Cross(right, xaxis);
    front.Normalize();

    // camera plane
    float colx[4] = {1, 0, 0, 1};
    float coly[4] = {0, 1, 0, 1};
    float colz[4] = {0, 0, 1, 1};
    float colall[4] = {1, 1, 1, 1};

    float fct = 0.05f;
    float fct2 = 0.83f;

    STMatrix4 curModelMatrix;
    curModelMatrix.EncodeI();

    //------------------------------------------------------------------------------
    // TO DO: Proj3_4 OpenGL
    // Update the matrix transformation for the manipulator geometry
    // Update curModelMatrix
    //------------------------------------------------------------------------------
    curModelMatrix.EncodeS(0.1, 0.1, 0.1);
    //-------------------------------------------------------------------------------

    // screen projection
    ViewProjectionScreenSpace(curModelMatrix);

    STVector3 origin(0,0,0);
    curModelMatrix.GetT(&origin.x, &origin.y, &origin.z);

    if(pScene->CurrentManipMode() == LOCAL) {
        xaxis.Transform(curModelMatrix);
        yaxis.Transform(curModelMatrix);
        zaxis.Transform(curModelMatrix);
        xaxis.Normalize();
        yaxis.Normalize();
        zaxis.Normalize();
    }


    // Rotations
    if((pScene->CurrentManipGeometryState() == AXIS_ALL) || 
        (pScene->CurrentManipGeometryState() == AXIS_ROTATIONALL)) {


        STVector3 X_UP;
        STVector3 X_RIGHT;
        STVector3 X_FRONT;
        STVector3 X_UP_sc;
        STVector3 X_RIGHT_sc;
        STVector3 X_FRONT_sc;


        STVector3 planenorm(pScene->GetCamera()->Position() - origin);
        planenorm.Normalize();
        STVector4 camplane = vector4(planenorm,0);
        
        // duplicate
        X_RIGHT   =   right    * ScreenFactor();
        X_UP      =   up       * ScreenFactor();
        if(pScene->CurrentManipMotion() == ROTATE_DUPLICATE)
            DrawCircle(origin, colorWhite, X_RIGHT, X_UP);
        else
            DrawCircle(origin, colorGrey, X_RIGHT, X_UP);


        // screen rot
        X_UP_sc    =  up * 1.2f       * ScreenFactor();
        X_RIGHT_sc =  right * 1.2f    * ScreenFactor();
        if(pScene->CurrentManipMotion() == ROTATE_SCREEN) 
            DrawCircle(origin, colorWhite, X_UP_sc, X_RIGHT_sc);
        else 
            DrawCircle(origin, colorMagenta, X_UP_sc, X_RIGHT_sc);


        // x rot
        right.Cross(dir, xaxis);
        right.Normalize();
        front.Cross(right, xaxis);
        front.Normalize();
        X_RIGHT   =   right    * ScreenFactor();
        X_FRONT   =   front    * ScreenFactor();
        if(pScene->CurrentManipMotion() == ROTATE_Y)
            DrawCircleHalf(origin, colorWhite, X_RIGHT, X_FRONT, camplane);
        else
            DrawCircleHalf(origin, colorRed, X_RIGHT, X_FRONT, camplane);


        // y rot
        right.Cross(dir, yaxis);
        right.Normalize();
        front.Cross(right, yaxis);
        front.Normalize();
        X_RIGHT   =   right    * ScreenFactor();
        X_FRONT   =   front    * ScreenFactor();
        if(pScene->CurrentManipMotion() == ROTATE_Y)
            DrawCircleHalf(origin, colorWhite, X_RIGHT, X_FRONT, camplane);
        else
            DrawCircleHalf(origin, colorGreen, X_RIGHT, X_FRONT, camplane);

        // z rot
        right.Cross(dir, zaxis);
        right.Normalize();
        front.Cross(right, zaxis);
        front.Normalize();
        X_RIGHT   =   right    * ScreenFactor();
        X_FRONT   =   front    * ScreenFactor();
        if(pScene->CurrentManipMotion() == ROTATE_Z)
            DrawCircleHalf(origin, colorWhite, X_RIGHT, X_FRONT, camplane);
        else
            DrawCircleHalf(origin, colorBlue, X_RIGHT, X_FRONT, camplane);
    }


    // cam
    if ((pScene->CurrentManipMotion() !=  MANIP_NONE) && ((pScene->CurrentManipMotion() != ROTATE_DUPLICATE)))
        DrawCam(origin, gblXVert*ScreenFactor(), gblYVert *ScreenFactor(), -Ng2);



    // draw translation manupulator
    //-----------------------------------------------
 
    // Translations
    if((pScene->CurrentManipGeometryState() == AXIS_ALL) ||
        (pScene->CurrentManipGeometryState() == AXIS_TRANSXYZ)) {

        DrawQuadrant(origin, 0.5f*ScreenFactor(), (pScene->CurrentManipMotion() == TRANS_XZ), xaxis, zaxis);
        DrawQuadrant(origin, 0.5f*ScreenFactor(), (pScene->CurrentManipMotion() == TRANS_XY), xaxis, yaxis);
        DrawQuadrant(origin, 0.5f*ScreenFactor(), (pScene->CurrentManipMotion() == TRANS_YZ), yaxis, zaxis);

        xaxis *= ScreenFactor();
        yaxis *= ScreenFactor();
        zaxis *= ScreenFactor();


        if(pScene->CurrentManipMotion() == TRANS_X)
            DrawAxis(origin, colorRed, xaxis, yaxis, zaxis, fct, fct2, colall);
        else
            DrawAxis(origin, colorRed, xaxis, yaxis, zaxis, fct, fct2, colx);


        // y axis
        if(pScene->CurrentManipMotion() == TRANS_Y) 
            DrawAxis(origin, colorGreen, yaxis, xaxis, zaxis, fct, fct2, colall);
        else
            DrawAxis(origin, colorGreen, yaxis, xaxis, zaxis, fct, fct2, coly);

    
        // z axis
        if(pScene->CurrentManipMotion() == TRANS_Z) 
            DrawAxis(origin, colorBlue, zaxis, xaxis, yaxis, fct, fct2, colall);
        else
            DrawAxis(origin, colorBlue, zaxis, xaxis, yaxis, fct, fct2, colz);
    }


}



//------------------------------------------------------------
// Nvidia Teapot  model
//-------------------------------------------------------------
void GenerateTeaPotModel(GLint grid, GLdouble scale)
{
  float p[4][4][3], q[4][4][3], r[4][4][3], s[4][4][3];
  long i, j, k, l;

  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_MAP2_VERTEX_3);
  glRotatef(270.0, 1.0, 0.0, 0.0);
  glScalef(0.5 * scale, 0.5 * scale, 0.5 * scale);
  glTranslatef(0.0, 0.0, -1.5);
  for (i = 0; i < 10; i++) {
    for (j = 0; j < 4; j++) {
      for (k = 0; k < 4; k++) {
        for (l = 0; l < 3; l++) {
          p[j][k][l] = geomData_teapot[geom_teapot[i][j * 4 + k]][l];
          q[j][k][l] = geomData_teapot[geom_teapot[i][j * 4 + (3 - k)]][l];
          if (l == 1)
            q[j][k][l] *= -1.0;
          if (i < 6) {
            r[j][k][l] =
              geomData_teapot[geom_teapot[i][j * 4 + (3 - k)]][l];
            if (l == 0)
              r[j][k][l] *= -1.0;
            s[j][k][l] = geomData_teapot[geom_teapot[i][j * 4 + k]][l];
            if (l == 0)
              s[j][k][l] *= -1.0;
            if (l == 1)
              s[j][k][l] *= -1.0;
          }
        }
      }
    }
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,
      &p[0][0][0]);
    glMapGrid2f(grid, 0.0, 1.0, grid, 0.0, 1.0);
    glEvalMesh2(GL_FILL, 0, grid, 0, grid);
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,
      &q[0][0][0]);
    glEvalMesh2(GL_FILL, 0, grid, 0, grid);
    if (i < 6) {
      glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,
        &r[0][0][0]);
      glEvalMesh2(GL_FILL, 0, grid, 0, grid);
      glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,
        &s[0][0][0]);
      glEvalMesh2(GL_FILL, 0, grid, 0, grid);
    }
  }
}


void BoundingSphere(STPoint3 *center, float *radius)
{
    STPoint3 topleft = gBoundingBox.first;
    STPoint3 bottomright = gBoundingBox.second;

    float c =  (topleft.x - bottomright.x) * (topleft.x - bottomright.x);
          c += (topleft.y - bottomright.y) * (topleft.y - bottomright.y);
          c += (topleft.z - bottomright.z) * (topleft.z - bottomright.z);

    *radius = fabs(sqrtf(c));
    *radius = (*radius)/2.0;
    *center = gMassCenter;
}


void CreateYourOwnMesh()
{
    float leftX   = -2.0f;
    float rightX  = -leftX;
    float nearZ   = -2.0f;
    float farZ    = -nearZ;
    
    gManualTriangleMesh= new STTriangleMesh();
    for (int i = 0; i < TesselationDepth+1; i++){
        for (int j = 0; j < TesselationDepth+1; j++) {
            float s0 = (float) i / (float) TesselationDepth;
            float x0 =  s0 * (rightX - leftX) + leftX;
            float t0 = (float) j / (float) TesselationDepth;
            float z0 = t0 * (farZ - nearZ) + nearZ;

            gManualTriangleMesh->AddVertex(x0,(x0*x0+z0*z0)*0.0f,z0,s0,t0);
        }
    }
    for (int i = 0; i < TesselationDepth; i++){
        for (int j = 0; j < TesselationDepth; j++) {
            unsigned int id0=i*(TesselationDepth+1)+j;
            unsigned int id1=(i+1)*(TesselationDepth+1)+j;
            unsigned int id2=(i+1)*(TesselationDepth+1)+j+1;
            unsigned int id3=i*(TesselationDepth+1)+j+1;
            gManualTriangleMesh->AddFace(id0,id2,id1);
            gManualTriangleMesh->AddFace(id0,id3,id2);
        }
    }
    gManualTriangleMesh->Build();
    gManualTriangleMesh->mMaterialAmbient[0]=0.2f;
    gManualTriangleMesh->mMaterialAmbient[1]=0.2f;
    gManualTriangleMesh->mMaterialAmbient[2]=0.6f;
    gManualTriangleMesh->mMaterialDiffuse[0]=0.2f;
    gManualTriangleMesh->mMaterialDiffuse[1]=0.2f;
    gManualTriangleMesh->mMaterialDiffuse[2]=0.6f;
    gManualTriangleMesh->mMaterialSpecular[0]=0.6f;
    gManualTriangleMesh->mMaterialSpecular[1]=0.6f;
    gManualTriangleMesh->mMaterialSpecular[2]=0.6f;
    gManualTriangleMesh->mShininess=8.0f;
}




//-------------------------------------------------------
// TO DO: Proj3_4 OpenGL
// in ENVIRONMENTMAP mode,
// you will create your cubemap and
// code your reflectance and environment map shaders
//------------------------------------------------------
void SetUpEnvironmentMap(void)
{

    // load camera projection matrix and model matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //----------------------------------------------------
    // TO DO: Proj3_4 OpenGL
    // You may "optionally" change the parameters of gluPerspective
    // to see how the perspective projection matrix  works
    // This is not required for grading
    //----------------------------------------------------
    gluPerspective( 35.0,  1.0, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // set the camera position
    // eye is at (0,0,8)
    // center is at (0,0,0)
    // up is in positive Y direction 
    gluLookAt(0.0, 0.0, 8.0, 
              0.0, 0.0, 0.0,          
              0.0, 1.0, 0.);          

    // enable depth testing
    glEnable(GL_DEPTH_TEST);


    //----------------------------------------------------
    // TO DO: Proj3_4 OpenGL
    // Follow the instructions in the assignment 
    // Fill in implementation for the following
    // envmap.vert, envmap.frag, reflectance.vert, reflectance.frag
    // vertex and fragment shaders
    // Your implementation should go in the
    // 1. GenerateCubeMap()
    // 2. Allocate memory for the shaders (the shaders are of type STShaderProgram)
    // 3. Load the shader files (use LoadVertexShader)
    // NOTE: the variable shader is an example for the phong model
    //      it is used here in defaultMode()
    //      use it as an example to write your code
    //---------------------------------------------------


    // first your need to make your cubemap texture
    // GenerateCubeMap();

    // now initialize your shaders

    //----------------------------------------

}



//
// Initialize the application, loading all of the settings that
// we will be accessing later in our fragment shaders.
//
void Setup()
{


    // reset
    //-----------------------------------------------
    for(int i = 0; i < (int) gTriangleMeshes.size(); ++i) {
        if(gTriangleMeshes[0])
            delete(gTriangleMeshes[0]);
        gTriangleMeshes[0] = NULL;
    }
    gTriangleMeshes.clear();
    if(gManualTriangleMesh) {
        delete(gManualTriangleMesh);
        gManualTriangleMesh = NULL;
    }
    gMassCenter = STPoint3(0,0,0);
    glDisable (GL_TEXTURE_CUBE_MAP);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);

    if(shader)
        delete(shader);
    if(envmapshader)
        delete(envmapshader);
    if(reflectanceshader)
        delete(reflectanceshader);
    shader = NULL;
    envmapshader = NULL;
    reflectanceshader = NULL;
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    vpscreenfactor = 1.0;
    screenScale = 1.0;

    if(pScene) {
        delete(pScene);
        pScene = NULL;
    }
    //-------------------------------------------------


    // initialize the scene
    if(pScene == NULL) {
        pScene = new Scene();
        pScene->SetRenderMode(startMode);
    }


    if(pScene->GetRenderMode() == ENVIRONMENTMAP) {
        SetUpEnvironmentMap();
        return;
    }


    // init the scene
    //----------------------------------------------------
    // TO DO: Proj3_4 OpenGL
    // Here is where you add nodes to your scene graph
    // for part I 
    //-----------------------------------------------------
    //
    //
    //
    //------------------------------------------------------



    // Set up lighting variables in OpenGL
    // Once we do this, we will be able to access them as built-in
    // attributes in the shader (see examples of this in normalmap.frag)
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_SPECULAR,  specularLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT,   ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,   diffuseLight);

    surfaceNormImg = new STImage(normalMap);
    surfaceNormTex = new STTexture(surfaceNormImg);
    
    surfaceDisplaceImg = new STImage(displacementMap);
    surfaceDisplaceTex = new STTexture(surfaceDisplaceImg);

    //----------------------------------------------------
    // TO DO: Proj3_4 OpenGL
    // Here is an example of creating a phong shader
    // No additional implementation required
    //-----------------------------------------------------
    shader = new STShaderProgram();
    shader->LoadVertexShader(vertexShader);
    shader->LoadFragmentShader(fragmentShader);


    pScene->GetCamera()->Reset();

    if(pScene->GetRenderMode() != ENVIRONMENTMAP) {
        pScene->GetCamera()->SetLookAt(STVector3(0.f,0.f,0.f));
        pScene->GetCamera()->SetPosition(STVector3(0.f,5.f,15.f));
        pScene->GetCamera()->SetUp(STVector3(0.f,1.f,0.f));
    }
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);

    
    // load a mesh
    STTriangleMesh::LoadObj(gTriangleMeshes,meshOBJ);
    gMassCenter=STTriangleMesh::GetMassCenter(gTriangleMeshes);
    std::cout<<"Mass Center: "<<gMassCenter<<std::endl;
    gBoundingBox=STTriangleMesh::GetBoundingBox(gTriangleMeshes);
    std::cout<<"Bounding Box: "<<gBoundingBox.first<<" - "<<gBoundingBox.second<<std::endl;

    // add a grid
    //CreateYourOwnMesh();
}

// clean up 
void CleanUp()
{
    for(unsigned int id=0;id<gTriangleMeshes.size();id++)delete gTriangleMeshes[id];
    if(gManualTriangleMesh!=0)
        delete gManualTriangleMesh;
}


// default display mode
//----------------------------------------------------
// TO DO: Proj3_4 OpenGL
// This function shows examples of how to bind textures
// to the phong shader
// The ReflectanceMapping function is where you will
// set parameters to your shaders
// No additional implementation required here
//-----------------------------------------------------
void defaultMode(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    pScene->GetCamera()->SetUpAndRight();

    STVector3 mPosition = pScene->GetCamera()->Position();
    STVector3 mLookAt = pScene->GetCamera()->LookAt();
    STVector3 mUp = pScene->GetCamera()->Up();
    float lightPosition[] = {10.0f, 15.0f, 10.0f, 1.0f};

    gluLookAt(mPosition.x,mPosition.y,mPosition.z,
              mLookAt.x,mLookAt.y,mLookAt.z,
              mUp.x,mUp.y,mUp.z);

    //----------------------------------------------------
    // TO DO: Proj3_4 OpenGL
    // 1. Set the camera projection and view matrix
    // 2. HINT - use GL_MODELVIEW_MATRIX and GL_PROJECTION_MATRIX
    //           set your matrices into the scene camera
    //----------------------------------------------------
    float modelMat_f[16];
    float projMat_f[16];
    STMatrix4 projMat(projMat_f);
    STMatrix4 modelMat(modelMat_f);

    //----------------------------------------------------

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);


    // draw manipulators
    if((pScene->GetRenderMode() == NAVIGATION) 
        && ((pScene->CurrentManipMode() == LOCAL)
            || (pScene->CurrentManipMode() == PARENT))
        && (pScene->HasManipulator()))
            DrawManipulator();

    // Texture 0: surface normal map
    glActiveTexture(GL_TEXTURE0);
    surfaceNormTex->Bind();
    
    // Texture 1: surface normal map
    glActiveTexture(GL_TEXTURE1);
    surfaceDisplaceTex->Bind();
    
    // Bind the textures we've loaded into openGl to
    // the variable names we specify in the fragment
    // shader.
    shader->SetTexture("normalTex", 0);
    shader->SetTexture("displacementTex", 1);
    shader->SetTexture("colorTex", 2);
    
    // Invoke the shader.  Now OpenGL will call our
    // shader programs on anything we draw.
    shader->Bind();

    if(mesh)
    {
        shader->SetUniform("normalMapping", -1.0);
        shader->SetUniform("displacementMapping", -1.0);
        shader->SetUniform("colorMapping", 1.0);
        
        glPushMatrix();

        //----------------------------------------------------
        // TO DO: Proj3_4 OpenGL
        // Once you have added nodes to your scene, replace this 
        // code by calling your PropogateTransforms function
        // to recursively visit the nodes in your scene
        // graph and render them
        //----------------------------------------------------
        // Pay attention to scale 
        STVector3 size_vector=gBoundingBox.second-gBoundingBox.first;
        float maxSize=(std::max)((std::max)(size_vector.x,size_vector.y),size_vector.z);
        glScalef(3.0f/maxSize,3.0f/maxSize,3.0f/maxSize);
        glTranslatef(-gMassCenter.x,-gMassCenter.y,-gMassCenter.z);
        for(unsigned int id=0;id<gTriangleMeshes.size();id++) {
            gTriangleMeshes[id]->Draw(smooth);
        }
        //--------------------------------------------------------------------------------


        glPopMatrix();
    }
    else
    {
        if(normalMapping){
            shader->SetUniform("displacementMapping", -1.0);
            shader->SetUniform("normalMapping", 1.0);
            shader->SetUniform("colorMapping", -1.0);
        }
        else{
            shader->SetUniform("displacementMapping", 1.0);
            shader->SetUniform("normalMapping", -1.0);
            shader->SetUniform("colorMapping", -1.0);
            shader->SetUniform("TesselationDepth", TesselationDepth);
        }

        //----------------------------------------------------
        // TO DO: Proj3_4 OpenGL
        // This code draws the gManualTriangleMesh from createyourownmesh
        // You do not need this for the assignment unless you
        // would like to create test meshes
        //----------------------------------------------------
        if(gManualTriangleMesh)
            gManualTriangleMesh->Draw(smooth);
        if(gTriangleMeshes.size()) {
            for(unsigned int id=0;id<gTriangleMeshes.size();id++) {
                gTriangleMeshes[id]->Draw(smooth);
            }
        }
        //-------------------------------------------------------------


    }

        

    shader->UnBind();
    
    glActiveTexture(GL_TEXTURE0);
    surfaceNormTex->UnBind();
    
    glActiveTexture(GL_TEXTURE1);
    surfaceDisplaceTex->UnBind();
    
    glutSwapBuffers();
}


// Display mode for reflectance mapping
// in ENVIRONMENTMAP mode
void ReflectanceMapping(void)
{

    // rotation matrix
    float m[4][4];
    STMatrix4 mI;

    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // build rotation matrix and place on stack
    glPushMatrix();
    build_rotmatrix(m, curQMat);
    glMultMatrixf(&m[0][0]);

    // generate the teapot model
    GenerateTeaPotModel(5, 1.1);

    // manipulate matrix stack
    glPopMatrix();


    //----------------------------------------------------
    // TO DO: Proj3_4 OpenGL
    // Set up your shader variables here
    //-----------------------------------------------------



    //-----------------------------------------------

    // swap buffers
    glutSwapBuffers();
}


//
// Display the output image from our vertex and fragment shaders
//
void DisplayCallback()
{

    if(pScene->GetRenderMode() == ENVIRONMENTMAP) 
        ReflectanceMapping();
    else
        defaultMode();
}

//
// Reshape the window and record the size so
// that we can use it for screenshots.
//
void ReshapeCallback(int w, int h)
{


    gWindowSizeX = w;
    gWindowSizeY = h;

    glViewport(0, 0, gWindowSizeX, gWindowSizeY);

   if (pScene->GetRenderMode() == ENVIRONMENTMAP)
       return;

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    // Set up a perspective projection
    float aspectRatio = (float) gWindowSizeX / (float) gWindowSizeY;
    gluPerspective(30.0f, aspectRatio, .1f, 10000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void SpecialKeyCallback(int key, int x, int y)
{
    switch(key) {
        case GLUT_KEY_LEFT:
            pScene->GetCamera()->Strafe(10,0);
            break;
        case GLUT_KEY_RIGHT:
            pScene->GetCamera()->Strafe(-10,0);
            break;
        case GLUT_KEY_DOWN:
            pScene->GetCamera()->Strafe(0,-10);
            break;
        case GLUT_KEY_UP:
            pScene->GetCamera()->Strafe(0,10);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}





void KeyCallback(unsigned char key, int x, int y)
{


    // TO DO: Any new key press events must be added to this function

    switch(key) {

        case 'a': {
            for(unsigned int id=0;id<gTriangleMeshes.size();id++)
                gTriangleMeshes[id]->mDrawAxis=!gTriangleMeshes[id]->mDrawAxis;
            if(gManualTriangleMesh!=0)
                gManualTriangleMesh->mDrawAxis=!gManualTriangleMesh->mDrawAxis;
        }
        break;

        case 'c': {
            // TO DO: Proj3_4 OpenGL
            // 1. Add a key press event 'c' that toggles the camera
            // between Fly and Orbit mode
            // 2. Use Camera::ToggleRotationMode
            //------------------------------------------------
            pScene->GetCamera()->ToggleRotationMode();
            //------------------------------------------------
        }
        break;

        case 'd': {
            // TO DO: Proj3_4 OpenGL
            // 1. Add a key press event 'd' that toggles the scene
            // between ENVIRONMENTMAP and NAVIGATION mode
            // 2. Don't forget to change the RenderMode gloabl startMode
            // ToggleRenderMode() , set startMode, Setup()
            //------------------------------------------------

            //------------------------------------------------
        }
        break;

        case 'e': {
            // TO DO: Proj3_4 OpenGL
            // 1. Add a key press event 'e' that toggles the manipulator
            // geometry showing. This makes it easier to select
            // parts of the manipulator
            // ALL, TRANSONLY, ROTATEONLY
            // call ToggleManipGeometry() in the scene class
            //------------------------------------------------

            //------------------------------------------------
        }   
        break;

        case 'f': {// switch between smooth shading and flat shading
            smooth = !smooth;
        }
        break;
    

        case 'k': {
            // TO DO: Proj3_4 OpenGL
            // 1. Add a key press event 'k' that toggles between local and parent mode
            // 2. Use Scene::ToggleManipMode() in scene class 
            //------------------------------------------------
            //------------------------------------------------
        }   
        break;


        case 'r': {
            pScene->GetCamera()->Reset();
        }
        break;
    

        case 's': {
                //
                // Take a screenshot, and save as screenshot.jpg
                //
                STImage* screenshot = new STImage(gWindowSizeX, gWindowSizeY);
                screenshot->Read(0,0);
                screenshot->Save("../../data/images/screenshot.jpg");
                delete screenshot;
         }
        break;

        // TO:DO do loop subdivision, uncomment this code
        // modify it so that it works with your sphere
        // simply make sure you are calling the functions
        // on the correct mesh.
        //---------------------------------
        //case 'l': 
        //    if(mesh){
        //        gTriangleMesh->LoopSubdivide();
	    //   if(proxyType) gTriangleMesh->CalculateTextureCoordinatesViaSphericalProxy();
	    //		else gTriangleMesh->CalculateTextureCoordinatesViaCylindricalProxy(-

        case 'm': {// switch between the mesh you create and the mesh from file
            mesh = !mesh;
        }
        break;

        case 'n': { // switch between normalMapping and displacementMapping
            normalMapping = !normalMapping;
        }
        break;

        case 'u': {
            pScene->GetCamera()->ResetUp();
        }
        break;
    
        case 'w': {
            for(unsigned int id=0;id<gTriangleMeshes.size();id++)
                gTriangleMeshes[id]->Write("output.obj");
       }
       break;

        case 'q': {
            exit(0);
        }

        default:
            break;
    }

    glutPostRedisplay();
}

void ManipulationMode(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    bSpin = false;
    glutIdleFunc(NULL);
    bMove = true;
    beginx = x;
    beginy = y;
  }
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    bMove = false;
  }
}


/**
 * Mouse event handler
 */
void MouseCallback(int button, int state, int x, int y)
{

    if(pScene->GetRenderMode() == ENVIRONMENTMAP) {
        ManipulationMode(button, state, x, y);
        return;
    }


    if (button == GLUT_LEFT_BUTTON
        || button == GLUT_RIGHT_BUTTON
        || button == GLUT_MIDDLE_BUTTON){
        gMouseButton = button;
    } 
    else {
        gMouseButton = -1;
    }
    
    if (state == GLUT_UP)
    {
        gPreviousMouseX = -1;
        gPreviousMouseY = -1;
    }

    if(  (button == GLUT_LEFT_BUTTON)
      && (state == GLUT_DOWN)
      && (pScene->GetRenderMode() == NAVIGATION)
      && (pScene->CurrentManipMode() == LOCAL || pScene->CurrentManipMode() == PARENT)) {

        // TO DO: Proj3_4 OpenGL
        // if there is a manupulator in the scene
        // see if it is selected
        // if it is selected, make the selected the part active
        // set the manipulation motion mode and return
        if(pScene->HasManipulator()) {
            if(pScene->SelectManipulator(x, y, gWindowSizeX, gWindowSizeY))
                return;
        }


        STPoint3 center;
        float radius;
        BoundingSphere(&center, &radius);


        // TO DO: Proj3_4 OpenGL
        // No new implementation required
        // This code selects objects.
        // If you are in a manipulation mode LOCAL or PARENT,
        // a manipulator is added to the object.
        // Note: selections occur as long as you click in the bounding sphere
        // of the object group (so technically you can select an object group
        // if the mouse is not on the object but close to it)
        // By deselecting (pressing far away from an object) you remove the manipulator.
        if(pScene->SelectObject(x, y, gWindowSizeX, gWindowSizeY, center, radius)) {
            fprintf(stderr, "SELECTED!\n");
            
            if(!pScene->HasManipulator()) 
                pScene->AddManipulator();
        }
        else {
            fprintf(stderr, "No selection found!\n");
            pScene->RemoveManip();
        }
    }
}


// spin
void Spin (void)
{
    pScene->Spin(lastQMat, curQMat, curQMat);
}


void motion(int x, int y)
{
  if (bMove) {
    pScene->GetCamera()->Orbit(lastQMat,
      (2.0 * beginx - gWindowSizeX) / gWindowSizeX,
      (gWindowSizeY - 2.0 * beginy) / gWindowSizeY,
      (2.0 * x - gWindowSizeX) / gWindowSizeX,
      (gWindowSizeY - 2.0 * y) / gWindowSizeY
      );
    beginx = x;
    beginy = y;
    bSpin = true;
    glutIdleFunc(Spin);
  }
}

/**
 * Mouse active motion callback (when button is pressed)
 */
void MouseMotionCallback(int x, int y)
{

    if(pScene->GetRenderMode() == ENVIRONMENTMAP) {
        motion(x, y);
        return;
    }


    // rotatation  and translation callbacks
    ProcessRotation(x, y);
    ProcessTranslation(x, y);


    if (gPreviousMouseX >= 0 && gPreviousMouseY >= 0)
    {
        //compute delta
        float deltaX = x-gPreviousMouseX;
        float deltaY = y-gPreviousMouseY;
        gPreviousMouseX = x;
        gPreviousMouseY = y;
        

        //orbit, strafe, or zoom
        if (gMouseButton == GLUT_LEFT_BUTTON)
        {
            // TO DO: Proj3_4 OpenGL
            // Either translate or roate objects
            // ...or just rotate the camera in
            // orbit or fly mode
            // This depends on the manipulation mode LOCAL, PARENT or NONE
            // and the manipulator state
            //-----------------------------------------

            //-------------------------------------------
        }
        else if (gMouseButton == GLUT_MIDDLE_BUTTON)
        {
            pScene->GetCamera()->Strafe(deltaX, deltaY);
        }
        else if (gMouseButton == GLUT_RIGHT_BUTTON)
        {
           pScene->GetCamera()->Zoom(deltaY);
        }
        
    } 
    else {
        gPreviousMouseX = x;
        gPreviousMouseY = y;
    }
    
}



// context menu implementation
void menu(int item)
{
    //------------------------------------------------------------------------------------
    // TO DO: Proj3_4 OpenGL
    // "Optional" 
    // On third mouse button down, CreateContextMenu()
    // alows you to add a context menu
    // The menu toggles modes 
    // implement this menu() function to complete this
    // This is an optional assignment and not required for grading
    //-------------------------------------------------------------------------------------
    switch (item) {
    default:
        assert(0);
    }
    glutPostRedisplay();
}



// user feedback on input args
void usage()
{
}



//------------------------------------------------------------------------------------
// TO DO: Proj3_4 OpenGL
// "Optional" 
// On third mouse button down, this function 
// alows you to add a context menu
// The menu toggles modes 
// implement the menu() function to complete this
// This is an optional  and not required for grading
//-------------------------------------------------------------------------------------
void CreateContextMenu()
{
  glutCreateMenu(menu);
  glutAddMenuEntry("Local", LOCAL);
  glutAddMenuEntry("Parent", PARENT);
  glutAddMenuEntry("Selection", NONE);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
//--------------------------------------
}

//-------------------------------------------------
// main program loop
//-------------------------------------------------
int main(int argc, char** argv)
{
    //------------------------------------------------------------------------------------
    // TO DO: Proj3_4 OpenGL
    // Shader files are specified here
    // No additional implementation required
    //------------------------------------------------------------------------------------
    meshOBJ                         = std::string("../../data/meshes/cone.obj");
    envmapFragmentShader            = std::string("kernels/envmap.frag");
    envmapVertexShader              = std::string("kernels/envmap.vert");
    reflectionFragmentShader        = std::string("kernels/reflectance.frag");
    reflectionVertexShader          = std::string("kernels/reflectance.vert");
    vertexShader                    = std::string("kernels/default.vert");
    fragmentShader                  = std::string("kernels/phong.frag");
    normalMap                       = std::string("../../data/images/normalmap.png");
    displacementMap                 = std::string("../../data/images/displacementmap.jpeg");


    //
    // Initialize GLUT.
    //
    glutInitWindowSize(800, 800);
    glutInit(&argc, argv);


    // initialize display mode
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Program 3_4 OpenGL");



    // verify cube maps are supported
    if (!glutExtensionSupported("GL_EXT_texture_cube_map")) {
        fprintf(stderr, "GL_EXT_texture_cube_map not supported by this version of OpenGL\n");
        exit(1);
    }

    //
    // Initialize GLEW.
    //
#ifndef __APPLE__
    glewInit();
    if(!GLEW_VERSION_2_0) {
        printf("Your graphics card or graphics driver does\n"
            "\tnot support OpenGL 2.0, trying ARB extensions\n");

        if(!GLEW_ARB_vertex_shader || !GLEW_ARB_fragment_shader) {
            printf("ARB extensions don't work either.\n");
            printf("\tYou can try updating your graphics drivers.\n"
                "\tIf that does not work, you will have to find\n");
            printf("\ta machine with a newer graphics card.\n");
            exit(1);
        }
    }
#endif

    // callback functions
    glutDisplayFunc(DisplayCallback);
    glutReshapeFunc(ReshapeCallback);
    glutSpecialFunc(SpecialKeyCallback);
    glutKeyboardFunc(KeyCallback);
    glutMouseFunc(MouseCallback);
    glutMotionFunc(MouseMotionCallback);
    glutIdleFunc(DisplayCallback);


    // Be sure to initialize GLUT (and GLEW for this assignment) before
    // initializing your application.

    Setup();

    //------------------------------------------------------------------------------------
    // TO DO: Proj3_4 OpenGL
    // Set up the camera orbit mode
    // -----------------------------------------------------------------------------------
    if(pScene->GetRenderMode() == ENVIRONMENTMAP)
        pScene->GetCamera()->Orbit(curQMat, 0.0, 0.0, 0.0, 0.0);
    //------------------------------------------------------------------------------------

    //------------------------------------------------------------------------------------
    // TO DO: Proj3_4 OpenGL
    // "Optional" enable context menu, CreateContextMenu()
    // This is not required for grading
    //------------------------------------------------------------------------------------

    //------------------------------------------------------------------------------------


    // run the main glut loop
    glutMainLoop();
  
  
    // Cleanup code should be called here.
    CleanUp();

    // return
    return 0;
}


