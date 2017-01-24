// Missile class


#ifndef MISSILE_H
#define MISSILE_H


#include "vector.h"

class Missile {
 public:

  // Constructors

  Missile() {}

  Missile( vector p, vector v, float y, vector c ) {
    pos0 = p;
    pos1 = p;
    velocity = v;
    colour = c;
    destY = y;
  }

  // Draw the missile and its trail

  void draw() {
    glColor3f( colour.x, colour.y, colour.z );
    glBegin( GL_LINES );
    glVertex2f( pos0.x, pos0.y );
    glVertex2f( pos1.x, pos1.y );
    glEnd();
  }

  // Move the missile over a time interval, deltaT

  void move( float deltaT ) {
    pos1 = pos1 + deltaT * velocity;
  }

  // Return the current position 

  vector position() {
    return pos1;
  }

  // Has the missile reached its destination?
  //Check to see if missile has reached the correct Y coordinate
  //return true if it has
  bool hasReachedDestination() {
    if(velocity.y < 0){
    	if(pos1.y < destY){return true;}
    	else {return false;}
    }
	
   else if( velocity.y > 0){
	if(pos1.y > destY){return true;}
	else{return false;}
   }   	
  }
 private:

  vector pos0;			// start position
  vector pos1;			// current position
  vector velocity;		// velocity
  vector colour;		// colour of missile trail
  float  destY;			// y position at destination
  			// x position as destination
};


#endif
