#include <windows.h>
#include "SDL.h"
#include <GL/glut.h> 
#include <stdio.h>  
#include <stdlib.h> 
#include <iomanip>
#include <CMATH> 
#include <iostream>
#include <Eigen/Dense>

using namespace Eigen;
using namespace Eigen::internal;
using namespace Eigen::Architecture;
using namespace std;
SDL_Surface *screen;  

GLfloat angle = 0.0;
//diffuse light color variables
GLfloat dlr = 1.0;
GLfloat dlg = 1.0;
GLfloat dlb = 1.0;
//ambient light color variables
GLfloat alr = 1.0;
GLfloat alg = 1.0;
GLfloat alb = 1.0;
//light position variables
GLfloat lx = 0.0;
GLfloat ly = 0.0;
GLfloat lz = 1.0;
GLfloat lw = 0.0;

GLfloat vpx_buff[24][3];
GLfloat vpz[3] = {0,0,1.732};
GLfloat vpo[3] = {0,0,0};
GLfloat vpx[3] = {1,0,0};
float R = 0.12;
float r = 0.8*R;
float H = 1.3;
void quit( int code )  
{  
    SDL_Quit( );  
    /* Exit program. */  
    exit( code );  
}  
void handleKeyEvent( SDL_keysym* keysym )  
{  
    switch( keysym->sym )  
    {  
    case SDLK_ESCAPE:  
        quit( 0 );  
        break;  
    case SDLK_SPACE:  
        break;
    case SDLK_a:  
        lx -= 10.0;
        break; 
    case SDLK_d:  
        lx += 10.0;
        break; 
    case SDLK_w:  
        ly += 10.0;
        break; 
    case SDLK_s:  
        ly -= 10.0;
        break;   
    default:  
        break;  
    }  
}  
void resizeGL(int width,int height)  
{  
    if ( height == 0 )    
    {    
        height = 1;    
    }    
    //Reset View   
    glViewport( 0, 0, (GLint)width, (GLint)height );    
    //Choose the Matrix mode   
    glMatrixMode( GL_PROJECTION );    
    //reset projection  
    glLoadIdentity();    
    //set perspection   
    gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );    
    //choose Matrix mode  
    glMatrixMode( GL_MODELVIEW );    
    glLoadIdentity();    
}  
void handleEvents()  
{  
    // Our SDL event placeholder.   
    SDL_Event event;  
    //Grab all the events off the queue.   
    while( SDL_PollEvent( &event ) ) {  
        switch( event.type ) {  
        case SDL_KEYDOWN:  
            // Handle key Event  
            handleKeyEvent( &event.key.keysym );  
            break;  
        case SDL_QUIT:  
            // Handle quit requests (like Ctrl-c).   
            quit( 0 );  
            break;  
        case SDL_VIDEORESIZE:  
            //Handle resize event  
            screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 16,  
                SDL_OPENGL|SDL_RESIZABLE);  
            if ( screen )   
            {  
                resizeGL(screen->w, screen->h);  
            }  
            break;  
        }  
    }  
}   
void initSDL(int width,int height,int bpp,int flags)  
{  
    // First, initialize SDL's video subsystem.   
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )   
    {  
        fprintf( stderr, "Video initialization failed: %s\n",  
            SDL_GetError( ) );  
        quit( 1 );  
    }  
    atexit(SDL_Quit);  
    //Set some Attribute of OpenGL in SDL  
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );  
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );  
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );  
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );  
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );  

    //Set the video mode    
    screen= SDL_SetVideoMode( width, height, bpp,flags);  
    if(!screen )   
    {  
        fprintf( stderr, "Video mode set failed: %s\n",SDL_GetError( ) );  
        quit( 1 );  
    }  
    resizeGL(screen->w, screen->h);  
    //Set caption  
    SDL_WM_SetCaption( "Sierpinski 3D", NULL );   
}  
void transform(GLfloat* vp1, GLfloat* vp2,float  theta)// shun shi zhen
{   
    // v1 rotate axis, rotating v2 vector, theta hudu, v3 temp
    Vector3f v1, v2, v3;
    for (int i = 0; i < 3; ++i)
    {
        v1[i] = vp1[i];
    }
    for (int j = 0; j < 3; ++j)
    {
        v2[j] = vp2[j];
    }           
    v1 = v1.normalized();//orith
    Matrix3f A;
    Matrix3f B;
    Matrix3f I;
    Matrix3f M;
    I.setIdentity(3,3);
    A<< v1[0]*v1[0],v1[0]*v1[1],v1[0]*v1[2],
        v1[1]*v1[0],v1[1]*v1[1],v1[1]*v1[2],
        v1[2]*v1[0],v1[2]*v1[1],v1[2]*v1[2];
    B<< 0, -v1[2], v1[1],
        v1[2], 0, -v1[0],
        -v1[1], v1[0], 0;
    M = A + cos(theta)*(I-A) + sin(theta)*B;
    v3 = M.adjoint()*v2 ;
    for (int k = 0; k < 3; ++k)
    {
        vp2[k] = v3[k];
    }
    //return v3;
}
void my_cone(GLfloat *vpz, GLfloat *vpo, GLfloat *vpx)
{
    int N = 24;  // 决定圆锥的平滑程度，自己试验决定合适的值
    //float R = 1.0f;   // 半径
    //float H = 1.0f;   // 高
    float PI = 3.14159f;
    float theta = 0.0f;
    //glColor3f(0.0, 0.0,1.0);  
    GLfloat vp1[3] = {vpz[0] - vpo[0], vpz[1] - vpo[1], vpz[2] - vpo[2]};
    GLfloat vp2[3] = {vpx[0] - vpo[0], vpx[1] - vpo[1], vpx[2] - vpo[2]};
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(vpz[0], vpz[1], vpz[2]);
    for (int i = 0; i < N; i++)
    {   
        theta =  2 * PI / N;
        glVertex3f(vpx[0],vpx[1],vpx[2]);
        transform(vp1, vp2,theta);
        for (int i = 0; i < 3; ++i)
        {
            vpx[i] = vp2[i] + vpo[i];
        }
        glVertex3f(vpx[0],vpx[1],vpx[2]);
    }
    glEnd(); 

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(vpo[0], vpo[1], vpo[2]);
    for (int i = 0; i < N; i++)
    {   
        theta =  2 * PI / N;
        glVertex3f(vpx[0],vpx[1],vpx[2]);
        transform(vp1, vp2,theta);
        for (int i = 0; i < 3; ++i)
        {
            vpx[i] = vp2[i] + vpo[i];
        }
        glVertex3f(vpx[0],vpx[1],vpx[2]);
    }
    glEnd(); 
    glFlush();
}
void calc_vx_buff(GLfloat *vpz, GLfloat *vpo, GLfloat *vpx, GLfloat vpx_buff[][3])
{
    int N= 24;
    float PI = 3.14159f;
    float theta = 0.0f;
    GLfloat vp1[3] = {vpz[0]-vpo[0], vpz[1]-vpo[1], vpz[2]-vpo[2]};
    GLfloat vp2[3] = {vpx[0]-vpo[0], vpx[1]-vpo[1], vpx[2]-vpo[2]};
    for (int i = 0; i < N; i++)
    {   
        theta =  2 * PI / N;
        for (int j = 0; j < 3; ++j)
        {
            vpx_buff[i][j] = vp2[j] + vpo[j];
        }   
        transform(vp1, vp2, theta);
    }
}
float calc_position(float alpha, float r, float R, int times)
{
    //GLfloat;
    float delta;
    //float distance = sqrt((vpz[0] - vpx[0])*(vpz[0] - vpx[0]) + (vpz[1] - vpx[1])*(vpz[1] - vpx[1]) + (vpz[2] - vpx[2])*(vpz[2] - vpx[2]));
    float a = 1.0f;
    float b = -2.0*r*cos(97.5/180*3.1416);
    float c = -(R*R + 2.0*r*R);
    float temp;
    delta = sqrt(b*b - 4.0*a*c);
    temp = (-b + delta)/2.0*a;
    alpha = alpha -(-b + delta)/2.0*a ;

    return alpha;
}
void cone_attachment(GLfloat *vpz, GLfloat *vpo, float r, float R,float H, GLfloat vpx[][3], int m)
{
    GLfloat vpo_attachment_buff0[24][3];
    GLfloat vpz_attachment_buff0[24][3];
    GLfloat vpx_attachment_buff0[24][3];
    GLfloat vpo_attachment_buff1[24][3];
    GLfloat vpz_attachment_buff1[24][3];
    GLfloat vpx_attachment_buff1[24][3];
    GLfloat vx_temp[24][3];
    GLfloat vpo_tmp[3];
    float alpha = 0.9;

    for (int times = 0; times < 3; ++times)
    {
        for (int i = 0; i < 24; i+=2)
        {
            for (int j = 0; j < 3; ++j)
            {
                vpo_attachment_buff0[i][j] = alpha*vpx[i][j] + (1-alpha)*vpz[j];// xian xing cha zhi for vpo_attachment
                vpx_attachment_buff0[i][j] = (alpha+R)*vpx[i][j] + (1-alpha-R)*vpz[j];// xian xing cha zhi for vpx_attachment
                vpo_tmp[j] = (1.0 - (1.0-alpha)*4.0)*(vpx[i][j] - vpo[j]);
                vpz_attachment_buff0[i][j] = (H)*(vpo_attachment_buff0[i][j] - vpo[j] - vpo_tmp[j]) + vpo_attachment_buff0[i][j];
            }
        }
        for (int i = 0; i <24; i+=2)
        {
            glColor3f(0.0,1.0,0.0);
            my_cone(vpz_attachment_buff0[i], vpo_attachment_buff0[i], vpx_attachment_buff0[i]);
            if (m>0)
            {
                calc_vx_buff(vpz_attachment_buff0[i], vpo_attachment_buff0[i],vpx_attachment_buff0[i], vx_temp);
                cone_attachment(vpz_attachment_buff0[i], vpo_attachment_buff0[i], 0.096, 0.12, 1.2, vx_temp, m-1);
                //calc_vx_buff(vpz_attachment_buff0[0], vpo_attachment_buff0[0],vpx_attachment_buff0[0], vx_temp);
                //cone_attachment(vpz_attachment_buff0[0], vpo_attachment_buff0[0], 0.096, 0.12, 0.8, vx_temp, m-1);
            }

        }

        alpha = calc_position(alpha, r, R,times);
        H = H/2.0;

        for (int i = 1; i < 24; i+=2)
        {
            for (int j = 0; j < 3; ++j)
            {
                vpo_attachment_buff1[i][j] = alpha*vpx[i][j] + (1-alpha)*vpz[j];// xian xing cha zhi for vpo_attachment
                vpx_attachment_buff1[i][j] = (alpha+r)*vpx[i][j] + (1-alpha-r)*vpz[j];// xian xing cha zhi for vpx_attachment
                vpo_tmp[j] = (1.0 - (1.0-alpha)*4.0)*(vpx[i][j] - vpo[j]);
                vpz_attachment_buff1[i][j] = (H)*(vpo_attachment_buff1[i][j]- vpo[j]  -  vpo_tmp[j]) + vpo_attachment_buff1[i][j];
            }
        }
        for (int i = 1; i < 24; i+=2)
        {
            glColor3f(1.0,0.0,0.0);
            my_cone(vpz_attachment_buff1[i], vpo_attachment_buff1[i], vpx_attachment_buff1[i]);
            if (m>0)
            {
                calc_vx_buff(vpz_attachment_buff1[i], vpo_attachment_buff1[i],vpx_attachment_buff1[i], vx_temp);
                cone_attachment(vpz_attachment_buff1[i], vpo_attachment_buff1[i], 0.096, 0.12, 1.2, vx_temp, m-1);
            }

        }


        //update state
        R = r;
        r = 0.8*r;
        H = H/2.0;
        alpha = calc_position(alpha, r, R,times);
    }

}
void renderGL()  
{  
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  
    glMatrixMode( GL_MODELVIEW );  
    glLoadIdentity( );  
    glTranslatef( -1.0, -0.5, -5.0 );  
    GLfloat DiffuseLight[] = {dlr, dlg, dlb}; //set DiffuseLight[] to the specified values
    GLfloat AmbientLight[] = {alr, alg, alb}; //set AmbientLight[] to the specified values
    glLightfv (GL_LIGHT0, GL_DIFFUSE, DiffuseLight); //change the light accordingly
    glLightfv (GL_LIGHT1, GL_AMBIENT, AmbientLight); //changethe light accordingly
    GLfloat LightPosition[] = {lx, ly, lz, lw}; //set the LightPosition to the specified values
    glLightfv (GL_LIGHT0, GL_POSITION, LightPosition); //change the light accordingly
    glRotatef(-70, 1.0, 0.0, 0.0); //rotate on the x axis
    glRotatef(15, 0.0, 1.0, 0.0); //rotate on the y axis
    glRotatef(15, 0.0, 0.0, 1.0); //rotate on the z axis
    //glColor3f(0.0,1.0,0.0);
    //my_cone(vpz,vpo,vpx); 
    //angle++;
    cone_attachment(vpz, vpo, r, R, H, vpx_buff,1);
    SDL_GL_SwapBuffers( );  
}  
void initGL( int width, int height )  
{  
    float ratio = (float) width / (float) height;  
    // Our shading model--Gouraud (smooth).  
    glEnable (GL_LIGHTING); //enable the lighting
    glEnable (GL_LIGHT0); //enable LIGHT0, our Diffuse Light
    glEnable (GL_LIGHT1); //enable LIGHT1, our Ambient Light4

    glShadeModel( GL_SMOOTH );  
    // Set the clear color.   
    glClearColor( 0, 0, 0, 0 );  
    // Setup our viewport.   
    glViewport( 0, 0, width, height );  
    //Change to the projection matrix and set our viewing volume.  
    glMatrixMode( GL_PROJECTION );  
    glLoadIdentity();  
    gluPerspective( 60.0, ratio, 1.0, 100.0 );  
}  
int main( int argc, char* argv[] )  
{  

    // Dimensions of our window.      
    int width = 640;  
    int height = 480;  
    // Color depth in bits of our window.   
    int bpp = 32;  
    int flags= SDL_OPENGL|SDL_RESIZABLE;  
    //Set the SDL  
    initSDL(width, height, bpp,flags);  
    //Set the OpenGL  
    initGL( width, height );  
    //main loop  */
    while(true)   
    {  
        /* Process incoming events. */  
        handleEvents( );  
        calc_vx_buff(vpz,vpo,vpx,vpx_buff);
        my_cone(vpz,vpo,vpx); 
        /* Draw the screen. */  
        renderGL( );  
    }  
    return 0;  
}  
