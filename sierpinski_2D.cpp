#include <windows.h>
#include <SDL/SDL.h>  
#include <GL/gl.h>  
#include <GL/glut.h>  
#include <stdio.h>  
#include <stdlib.h>  

SDL_Surface *screen;  

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
    SDL_WM_SetCaption( "Sierpinski 2D", NULL );   
}  
//core logic
void renderGL()  
{  
    // Clear the color and depth buffers.   
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  
    // We don't want to modify the projection matrix. */  
    glMatrixMode( GL_MODELVIEW );  
    glLoadIdentity( );  
    // Move down the z-axis.   
    glTranslatef( -20.0, -20.0, -60.0 );  
    //Draw a square  
     /* A triangle */    
    GLfloat vertices[3][2]={{0.0,0.0},{25.0,50.0},{50.0,0.0}};    
    
    int i, j, k;    
    srand(10);        /* standard random number generator */    
    GLfloat p[2] ={7.5,5.0};  /* an arbitrary initial point inside  
                              traingle */    
    glClear(GL_COLOR_BUFFER_BIT);  /* clear the window */    
    glBegin(GL_POINTS);    
    /* compute and plots 5000 new points */    
    for( k=0; k<500000; k++)    
    {    
        j=rand()%3; /* pick a vertex at random */    
        /* Compute point halfway between selected vertex and old point */    
        p[0] = (p[0]+vertices[j][0])/2.0;    
        p[1] = (p[1]+vertices[j][1])/2.0;    
        /* plot new point */    
        glVertex2fv(p);    
    }    
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
    initSDL(width, height, bpp, flags);  
    //Set the OpenGL  
    initGL( width, height );  
      
    //main loop  
    while(true)   
    {  
        /* Process incoming events. */  
        handleEvents( );  
        /* Draw the screen. */  
        renderGL( );  
    }  
    return 0;  
}  