#include <windows.h>
#include <SDL/SDL.h>  
#include <GL/gl.h>  
#include <GL/glu.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <iomanip>
#include <fstream>
#include <iostream>
using namespace std;

SDL_Surface *screen;  
GLfloat colors[6][3]={{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,0.5},{0.0,0.5,0.5},{0.7,0.5,0.6},{0.1,0.2,0.3}};  
int m = 1;
int num = 0;
ofstream outfile("b.txt");
ofstream outfile2("a.txt");
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
    //case SDLK_a:  
    //    m = m+1;
    //    break;  
    //case SDLK_s:  
    //    m = m-1;
    //    break;  
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
            // Handle key Event  keydown is differ with keyup
            handleKeyEvent( &event.key.keysym );  
            break;  
        case SDL_QUIT:  
            // Handle quit requests (like Ctrl-c).   
            quit( 0 );  
            break;  
        case SDL_VIDEORESIZE:  
            //Handle resize event  ,for current windows
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
  
void triangle( GLfloat *va, GLfloat *vb, GLfloat *vc)  
{  
       glVertex3fv(va);  
       glVertex3fv(vb);  
       glVertex3fv(vc);  
       outfile<<"v "<<va[0] <<" "<<va[1]<<" "<<va[2]<<endl;
       outfile<<"v "<<vb[0] <<" "<<vb[1]<<" "<<vb[2]<<endl;  
       outfile<<"v "<<vc[0] <<" "<<vc[1]<<" "<<vc[2]<<endl;  
       va[3] = ++num;
       vb[3] = ++num;
       vc[3] = ++num;
       outfile2<<"f "<<va[3] <<" "<<vb[3]<<" "<<vc[3]<<endl;  

}  
void tetra(GLfloat *a, GLfloat *b, GLfloat *c, GLfloat *d, GLfloat *e, GLfloat *f, GLfloat *g, GLfloat *h)  
{  
    triangle(a,b,c);
    triangle(a,c,d);
    glColor3fv(colors[0]);  
    triangle(a,b,f);
    triangle(a,e,f);
    glColor3fv(colors[1]);  
    triangle(b,c,g);
    triangle(b,f,g);
    glColor3fv(colors[2]);  

    triangle(d,c,g);
    triangle(d,h,g);
    glColor3fv(colors[3]); 
    triangle(a,d,h); 
    triangle(a,e,h); 
    glColor3fv(colors[4]); 
    triangle(e,f,g);
    triangle(e,h,g);    
    glColor3fv(colors[5]); 
}  

void trans(float x1, float y1, float z1, float x2, float y2, float z2)
{
    GLfloat v0[4], v1[4], v2[4], v3[4], v4[4], v5[4], v6[4], v7[4];
    float L = x2 - x1;
    float W = y2 - y1;
    float H = z2 - z1;
    v0[0] = x1 + L;
    v0[1] = y1 + W;
    v0[2] = z1 + H;

    v1[0] = x1 + L;
    v1[1] = y2 - W;
    v1[2] = z1 + H;

    v2[0] = x2 - L;
    v2[1] = y2 - W;
    v2[2] = z1 + H;

    v3[0] = x2 - L;
    v3[1] = y1 + W;
    v3[2] = z1 + H;

    v4[0] = x1 + L;
    v4[1] = y1 + W;
    v4[2] = z2 - H;

    v5[0] = x1 + L;
    v5[1] = y2 - W;
    v5[2] = z2 - H;

    v6[0] = x2 - L;
    v6[1] = y2 - W;
    v6[2] = z2 - H;

    v7[0] = x2 - L;
    v7[1] = y1 + W;
    v7[2] = z2 - H;

    tetra(v0, v1, v2, v3, v4, v5, v6, v7);
}
  
void divide_tetra(float x1, float y1, float z1, float x2, float y2, float z2, int m)
{  
    float L = x2 - x1;
    float W = y2 - y1;
    float H = z2 - z1;
    if(m<1)  
       {  
        L = L/3;
        W = W/3;
        H = H/3;
        //first loop
        trans(x2-L,y1,z2-H,x2,y1+W,z2);
        trans(x2-L,y1,z1+H,x2,y1+W,z2-H);
        trans(x2-L,y1,z1,x2,y1+W,z1+H);

        trans(x2-L,y1+W,z2-H,x2,y2-W,z2);
        trans(x2-L,y1+W,z1,x2,y2-W,z1+H);

        trans(x2-L,y2-W,z2-H,x2,y2,z2);
        trans(x2-L,y2-W,z1+H,x2,y2,z2-H);
        trans(x2-L,y2-W,z1,x2,y2,z1+H);


        //zhongjian
        trans(x1+L,y1,z2-H,x2-L,y1+W,z2);
        trans(x1+L,y1,z1,x2-L,y1+W,z1+H);
        trans(x1+L,y2-W,z2-H,x2-L,y2,z2);
        trans(x1+L,y2-W,z1,x2-L,y2,z1+H);
        //last loop
        trans(x1,y1,z2-H,x1+L,y1+W,z2);
        trans(x1,y1,z1+H,x1+L,y1+W,z2-H);
        trans(x1,y1,z1,x1+L,y1+W,z1+H);

        trans(x1,y1+W,z2-H,x1+L,y2-W,z2);
        trans(x1,y1+W,z1,x1+L,y2-W,z1+H);

        trans(x1,y2-W,z2-H,x1+L,y2,z2);
        trans(x1,y2-W,z1+H,x1+L,y2,z2-H);
        trans(x1,y2-W,z1,x1+L,y2,z1+H);
  
    }  
    else  
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        L = L/3;
        W = W/3;
        H = H/3;
         //first loop
        divide_tetra(x2-L,y1,z2-H,x2,y1+W,z2, m-1);
        divide_tetra(x2-L,y1,z1+H,x2,y1+W,z2-H, m-1);
        divide_tetra(x2-L,y1,z1,x2,y1+W,z1+H, m-1);

        divide_tetra(x2-L,y1+W,z2-H,x2,y2-W,z2, m-1);
        divide_tetra(x2-L,y1+W,z1,x2,y2-W,z1+H, m-1);

        divide_tetra(x2-L,y2-W,z2-H,x2,y2,z2, m-1);
        divide_tetra(x2-L,y2-W,z1+H,x2,y2,z2-H, m-1);
        divide_tetra(x2-L,y2-W,z1,x2,y2,z1+H, m-1);


        //zhongjian
        divide_tetra(x1+L,y1,z2-H,x2-L,y1+W,z2, m-1);
        divide_tetra(x1+L,y1,z1,x2-L,y1+W,z1+H, m-1);
        divide_tetra(x1+L,y2-W,z2-H,x2-L,y2,z2, m-1);
        divide_tetra(x1+L,y2-W,z1,x2-L,y2,z1+H, m-1);
        //last loop
        divide_tetra(x1,y1,z2-H,x1+L,y1+W,z2, m-1);
        divide_tetra(x1,y1,z1+H,x1+L,y1+W,z2-H, m-1);
        divide_tetra(x1,y1,z1,x1+L,y1+W,z1+H, m-1);

        divide_tetra(x1,y1+W,z2-H,x1+L,y2-W,z2, m-1);
        divide_tetra(x1,y1+W,z1,x1+L,y2-W,z1+H, m-1);

        divide_tetra(x1,y2-W,z2-H,x1+L,y2,z2, m-1);
        divide_tetra(x1,y2-W,z1+H,x1+L,y2,z2-H, m-1);
        divide_tetra(x1,y2-W,z1,x1+L,y2,z1+H, m-1);
    }
}  
  
void renderGL()  
{  
    //Define a triangle in space  
    float x1 = -0.5;
    float y1 = -0.5;
    float z1 = 0.5;
    float x2 = 0.5;
    float y2 = 0.5;
    float z2 = -0.5;
    // Clear the color and depth buffers.   
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  
    // We don't want to modify the projection matrix. */  
    glMatrixMode( GL_MODELVIEW );  
    glLoadIdentity( );  
    // Move down the z-axis.   
    glTranslatef( 0.0, 0.0, -2.0 );  
    glRotatef(50,0,1,0);  
    glRotatef(30,1,0,0);  
  
    glClear(GL_COLOR_BUFFER_BIT);  /* clear the window */    
    glBegin(GL_TRIANGLES);    
    divide_tetra(x1,y1,z1,x2,y2,z2,m);
    glEnd();  
    SDL_GL_SwapBuffers( );  
}  

void initGL( int width, int height )   
{  
    float ratio = (float) width / (float) height;  
    // Our shading model--Gouraud (smooth).   
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
    initGL(width, height);  
    // init output

    if(!outfile){
    cout << "Unable to open otfile";
        exit(1); // terminate with error

	}
    if(!outfile2){
    cout << "Unable to open otfile2";
        exit(1); // terminate with error

    }
    //main loop  
    // while(true)   
    // {  
        /* Process incoming events. */  
        handleEvents( );  
        /* Draw the screen. */  
        renderGL( );  
    // }  
    return 0;  
}  


