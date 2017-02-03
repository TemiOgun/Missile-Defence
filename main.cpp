// Missile defence game


#include "headers.h"
#include "missile.h"
#include "state.h"

// Window and world dimensions


#define WINDOW_X_POS   100
#define WINDOW_Y_POS    50

#define WINDOW_WIDTH   600
#define WINDOW_HEIGHT  400

#define WORLD_LEFT   -0.02	// The world lives in a [0,1] x [0,1] box
#define WORLD_RIGHT   1.02	// with a small (0.02) border all around.
#define WORLD_BOTTOM -0.02

float worldTop;			// calculated in main()


// ----------------------------------------------------------------
//
// The "state" contains the world and manages all actions.
//
// ----------------------------------------------------------------


#include "state.h"

State *state;


// ----------------------------------------------------------------
//
// Below are all the OpenGL event handlers.
//
// ----------------------------------------------------------------


void display()

{
  glClear( GL_COLOR_BUFFER_BIT );

  state->draw();

  glFlush();
  glutSwapBuffers();
}


void mouseClick( int button, int buttonState, int x, int y )

{
  if (buttonState == GLUT_DOWN) {
	
	//convert position of mouse to world state
	
	GLint viewport[4];     // Where The Viewport Values Will Be Stored
	glGetIntegerv(GL_VIEWPORT, viewport);  // Retrieves The Viewport Values (X, Y, Width, Height)
	GLdouble modelview[16];      // Where The 16 Doubles Of The Modelview Matrix Are To Be Stored
glGetDoublev(GL_MODELVIEW_MATRIX, modelview);       // Retrieve The Modelview Matrix
	GLdouble projection[16];   // Where The 16 Doubles Of The Projection Matrix Are To Be Stored
glGetDoublev(GL_PROJECTION_MATRIX, projection);     // Retrieve The Projection Matrix
	GLfloat winX, winY;
	winX = x;  //window x position
	winY = y;  //window y position
	winY = (float)viewport[3] - winY;
	
	GLdouble wx, wy, wz;
	gluUnProject( winX, winY, 0, modelview, projection, viewport, &wx, &wy, &wz);

    // Calculate the world coordinates of mouse (x,y)

  

   // Shoot from silo 0, 1, or 2

	if(wx<=0.33){
	state->fireMissile(0,wx,wy);
	}
	
	else if(wx>0.33 && wx <=0.66){
	state->fireMissile(1, wx, wy);
	}

	else if(wx>0.66){
	state->fireMissile(2, wx, wy);
	}
	
	


  glutPostRedisplay();
}

}
void keyPress( unsigned char c, int x, int y )

{
  switch (c) {
  case 27:
    exit(0);
  }

  glutPostRedisplay();
}


void idleAction()

{
  static bool firstTimeThrough = true;
  static unsigned int previousTime;
  unsigned int currentTime;

#ifdef LINUX
  // Get the current time (measured in milliseconds since the Unix Epoch)
  struct timeb t;
  ftime( &t );
  currentTime = t.time * 1000 + t.millitm;
#else
  // Just guess at the current time interval (30/1000 seconds)
  currentTime = previousTime + 30;
#endif

  // Update the state.  Provide the number of seconds 
  // since the last update (if using Unix).

  if (!firstTimeThrough)
    state->updateState( (currentTime - previousTime) / 1000.0 );
  else
    firstTimeThrough = false;

  previousTime = currentTime;
  
#ifdef LINUX
  usleep( 30000 );		// sleep for 0.030 seconds
#endif

  glutPostRedisplay();
}



// ----------------------------------------------------------------
//
// The main program
//
// ----------------------------------------------------------------


int main( int argc, char **argv )

{

  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
  glutInit( &argc, argv );

  glutInitWindowPosition( WINDOW_X_POS, WINDOW_Y_POS );
  glutInitWindowSize( WINDOW_WIDTH, WINDOW_HEIGHT );

  glutCreateWindow( "Missile Defence" );

  glutDisplayFunc( display );

  glutSetCursor(GLUT_CURSOR_CROSSHAIR);

  glutMouseFunc( mouseClick );
  glutKeyboardFunc( keyPress );
  glutIdleFunc( idleAction );

  worldTop = (WORLD_RIGHT - WORLD_LEFT) / (float) WINDOW_WIDTH * WINDOW_HEIGHT + WORLD_BOTTOM; 

  gluOrtho2D( WORLD_LEFT, WORLD_RIGHT, WORLD_BOTTOM, worldTop );

  std::cout << "top = " << worldTop  << std::endl;
  
  state = new State( worldTop );
  glutMainLoop();
//  std::cout << "position ="<< missilesIn[1].position() << std::endl;
  return 0;
}
