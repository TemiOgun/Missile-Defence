// state.cpp


#include "headers.h"

#include "state.h"


// Draw each of the objects in the world


void State::draw() 

{
  int i;

  for (i=0; i<silos.size(); i++)
    silos[i].draw();

  for (i=0; i<cities.size(); i++)
    cities[i].draw();

  for (i=0; i<missilesIn.size(); i++)
    missilesIn[i].draw();

  for (i=0; i<missilesOut.size(); i++)
    missilesOut[i].draw();

  for (i=0; i<explosions.size(); i++)
    explosions[i].draw();
}
//explosion graphics
void State::explosion(vector position){
  explosions.add( Circle( position,//current position
			     0.2, // Speed
			     0.05,//radius
			     vector(1,0,0) ) );// color
      
}

//collision dection for missiles
bool State::collision(Missile m, Circle c){
     vector posC = c.position();
     vector posM = m.position();

     float dx = posC.x - posM.x;
     float dy = posC.y - posM.y;
    
     float distance = sqrt(dx*dx + dy*dy);

    if(distance < c.radius()){
     return true;
     }
    else{return false;}

}

//collision dection for missiles
bool State::collision(Missile m, vector p){
     vector M = m.position();

     float dx = M.x - p.x;
     float dy = M.y - p.y;
    
     float distance = sqrt(dx*dx + dy*dy);

    if((M.x>p.x - 0.04) && (M.x <p.x + 0.04) && (M.y<p.y + 0.03)){
     return true;
     }
    else{return false;}

}


// Update the state of the world after some time interval, deltaT
//
// CHANGE ALL OF THIS FUNCTION


void State::updateState( float deltaT )

{
  int i;

  // Update the time

  currentTime += deltaT;

  // Generate some new missiles.  The rate of missle generation
  // should increase with time.
  if (randIn01() < (0.0085 + currentTime/5000)) {	// New missile with a probability that increases with time

    missilesIn.add( Missile( vector( randIn01(), worldTop, 0), // source
			     vector( -0.02, -0.1, 0 ),   // velocity
			     0 ,                      // destination y
			     vector( 1,1,0 ) ) );    // colour
  }

  // Look for terminating missiles
  for (i=0; i<missilesIn.size(); i++)
  if (missilesIn[i].hasReachedDestination()) {
      explosion(missilesIn[i].position());
     missilesIn.remove(i);
      i--;
    }

  for (i=0; i<missilesOut.size(); i++)
    if (missilesOut[i].hasReachedDestination()) {
     explosion(missilesOut[i].position());
      missilesOut.remove(i);
      i--;
    }

  // Look for terminating explosions

  for (i=0; i<explosions.size(); i++)
    if (explosions[i].radius() >= explosions[i].maxRadius()) {
      explosions.remove(i);
      i--;
    }

  // Look for incoming missiles that hit an explosion and are
  // destroyed
  for(i=0;i<missilesIn.size();i++){
     for(int j = 0; j < explosions.size(); j ++){
	if(collision(missilesIn[i],explosions[j])){
	explosion(missilesIn[i].position());
      	missilesIn.remove(i);
      	i--;
	explosions.remove(j);
        j--;
	break;
	}
     }
  }

 for(i=0;i<missilesIn.size();i++){
     for(int j = 0; j < cities.size(); j ++){
	if(collision(missilesIn[i],cities[j].position())){
	explosion(missilesIn[i].position());
      	missilesIn.remove(i);
      	i--;
	cities.remove(j);
        j--;
	break;
	}
     }
  }

 for(i=0;i<missilesIn.size();i++){
     for(int j = 0; j < silos.size(); j ++){
	if(collision(missilesIn[i],silos[j].position())){
	explosion(missilesIn[i].position());
      	missilesIn.remove(i);
      	i--;
	silos.remove(j);
        j--;
	break;
	}
     }
  }


  // Update all the moving objects

  for (i=0; i<missilesIn.size(); i++){
    missilesIn[i].move( deltaT );
  }
  for (i=0; i<missilesOut.size(); i++)
    missilesOut[i].move( deltaT );

  for (i=0; i<explosions.size(); i++)
    explosions[i].expand( deltaT );
}


// Fire a missile

void State::fireMissile( int siloIndex, float x, float y )

{
  const float speed = 0.9;
    
  if (silos[siloIndex].canShoot()) {

    silos[siloIndex].decrMissiles();

     vector siloPos = silos[siloIndex].position();

    missilesOut.add( Missile( silos[siloIndex].position(),   // source
			      speed * vector(x-siloPos.x,y-siloPos.y,0), // velocity
			      y,                             // destination y
			      
			      vector( 1,1,0 ) ) );           // colour
  }
}



// Create the initial state of the world


void State::setupWorld()

{
  // Keep track of the time

  currentTime = 0;

  timeOfIncomingFlight = 6;	// 6 seconds for incoming missile to arrive

  // Seed the random number generator

#ifdef _WIN32
  srand( 12345 );
#else
  struct timeb t;
  ftime( &t );
  srand( t.time );
#endif

  // Silos are added in order left, middle, right so that they have
  // indices 0, 1, and 2 (respectively) in the "silos" array.

  silos.add(  Silo( vector( 0.1, 0, 0 ) ) );
  silos.add(  Silo( vector( 0.5, 0, 0 ) ) );
  silos.add(  Silo( vector( 0.9, 0, 0 ) ) );

  // Cities on the left

  cities.add( City( vector( 0.2, 0, 0 ) ) );
  cities.add( City( vector( 0.3, 0, 0 ) ) );
  cities.add( City( vector( 0.4, 0, 0 ) ) );

  // Cities on the right

  cities.add( City( vector( 0.6, 0, 0 ) ) );
  cities.add( City( vector( 0.7, 0, 0 ) ) );
  cities.add( City( vector( 0.8, 0, 0 ) ) );
}
