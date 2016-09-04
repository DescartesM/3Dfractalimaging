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

GLfloat colors[4][3]={{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},{0.0,1.0,1.0}};  
double alpha = 0.55;

int num = 0;
ofstream outfile("b.txt");
ofstream outfile2("a.txt");

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
  
void triangle( GLfloat *va, GLfloat *vb, GLfloat *vc)  
{  

       glVertex3fv(va);  
       glVertex3fv(vb);  
       glVertex3fv(vc);
      // outfile<<"v "<<va[0] <<" "<<va[1]<<" "<<va[2]<<endl;
      // outfile<<"v "<<vb[0] <<" "<<vb[1]<<" "<<vb[2]<<endl;  
      // outfile<<"v "<<vc[0] <<" "<<vc[1]<<" "<<vc[2]<<endl;  
       va[3] = ++num;
       vb[3] = ++num;
       vc[3] = ++num;
      // outfile2<<"f "<<va[3] <<" "<<vb[3]<<" "<<vc[3]<<endl;    
}  
  
void tetra(GLfloat *a, GLfloat *b, GLfloat *c, GLfloat *d)  
{  
  
      //glColor3fv(colors[0]);  
      triangle(a,b,c);  
      //glColor3fv(colors[1]);  
      triangle(a,c,d);  
      //glColor3fv(colors[2]);  
      triangle(a,d,b);  
      //glColor3fv(colors[3]);  
      triangle(b,d,c);  
}  
void divide_tetra(GLfloat *a, GLfloat *b, GLfloat *c, GLfloat *d, int m)  
{  
    GLfloat mid[12][4];  
    int j;  
        /* compute 12 midpoints fuzhu design */  
        for(j=0; j<3; j++) 
            mid[0][j] = alpha*a[j] + (1-alpha)*b[j];  
        for(j=0; j<3; j++) 
            mid[1][j] = alpha*a[j] + (1-alpha)*c[j];
        for(j=0; j<3; j++) 
            mid[2][j] = alpha*a[j] + (1-alpha)*d[j];
        for(j=0; j<3; j++) 
            mid[3][j] = alpha*b[j] + (1-alpha)*a[j];  
        for(j=0; j<3; j++) 
            mid[4][j] = alpha*c[j] + (1-alpha)*a[j]; 
        for(j=0; j<3; j++) 
            mid[5][j] = alpha*d[j] + (1-alpha)*a[j]; 
        //
        for(j=0; j<3; j++) 
            mid[6][j] = alpha*b[j] + (1-alpha)*c[j]; 
        for(j=0; j<3; j++) 
            mid[7][j] = alpha*c[j] + (1-alpha)*b[j]; 
        for(j=0; j<3; j++) 
            mid[8][j] = alpha*d[j] + (1-alpha)*c[j]; 
        for(j=0; j<3; j++) 
            mid[9][j] = alpha*c[j] + (1-alpha)*d[j]; 
        for(j=0; j<3; j++) 
            mid[10][j] = alpha*d[j] + (1-alpha)*b[j]; 
        for(j=0; j<3; j++) 
            mid[11][j] = alpha*b[j] + (1-alpha)*d[j]; 
        /* create 4 tetrahedrons  by subdivision */  
    if(m>0)  
    {  
        divide_tetra(a,mid[3],mid[4],mid[5], m-1);  
        divide_tetra(mid[0],b,mid[7],mid[10], m-1);  
        divide_tetra(mid[1],mid[6],c,mid[8], m-1);  
        divide_tetra(mid[2],mid[9],d,mid[11], m-1);  
  
    }  
     /* compute 24 anotherpoints fuzhu design */  
    else  
    {
        tetra(a,b,c,d); /* draw tetrahedron at end of recursion */  
    }
}  
  
void renderGL()  
{  
    //Define a triangle in space  
    GLfloat v[4][4]={
  //  {0.0, 0.0, 1.0},
   // {0.0, 0.942809, -0.33333},
   // {-0.816497, -0.471405, -0.333333},
  //  {0.816497, -0.471405, -0.333333}
       {0.0, 0.0, 0.4081,0},
       {0.0, 0.578, -0.4081,0},
       {-0.5, -0.288, -0.4081,0},
       {0.5, -0.288, -0.4081,0}, 
        };  
    // Clear the color and depth buffers.   
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  
    // We don't want to modify the projection matrix. */  
    glMatrixMode( GL_MODELVIEW );  
    glLoadIdentity( );  
    // Move down the z-axis.   
    glTranslatef( -1.0, -0.5, -5.0 );  
    GLfloat DiffuseLight[] = {dlr, dlg, dlb}; //set DiffuseLight[] to the specified values
    GLfloat AmbientLight[] = {alr, alg, alb}; //set AmbientLight[] to the specified values
    glLightfv (GL_LIGHT0, GL_DIFFUSE, DiffuseLight); //change the light accordingly
    glLightfv (GL_LIGHT1, GL_AMBIENT, AmbientLight); //changethe light accordingly
    GLfloat LightPosition[] = {lx, ly, lz, lw}; //set the LightPosition to the specified values
    glLightfv (GL_LIGHT0, GL_POSITION, LightPosition); //change the light accordingly
    //glRotatef(50,0,1,0);  
    //glRotatef(30,1,0,0);  
    glRotatef(angle, 1.0, 0.0, 0.0); //rotate on the x axis
    glRotatef(angle, 0.0, 1.0, 0.0); //rotate on the y axis
    glRotatef(angle, 0.0, 0.0, 1.0); //rotate on the z axis

    glClear(GL_COLOR_BUFFER_BIT);  /* clear the window */    
    glBegin(GL_TRIANGLES);    
    divide_tetra(v[0],v[1],v[2],v[3],3);  
    glEnd();  
    SDL_GL_SwapBuffers( );  
    angle ++;
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
   /* if(!outfile){
    cout << "Unable to open otfile";
        exit(1); // terminate with error
    }
    if(!outfile2){
    cout << "Unable to open otfile2";
        exit(1); // terminate with error

    }
    //main loop  */
    while(true)   
    {  
        /* Process incoming events. */  
        handleEvents( );  
        /* Draw the screen. */  
        renderGL( );  
    }  
    return 0;  
}  
