/*-------------------------------------------------------

Implements all of the components used program.

---------------------------------------------------------*/

#include "components.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string>
#include <GL/gl.h>     // The GL Header File
#include <GL/glut.h>   // The GL Utility Toolkit (Glut) Header
#include <unistd.h>
#include "gamestate.h"


#define PI 3.14159265f

using namespace std;

/*Begin Ball-------------------------------------------------------*/

/* The constructor for a Ball. 
 *
 * @param (double x) starting x position of object
 * @param (double y) starting y position of object
 * @param (int player) flag telling if the ball is the int player one
 * 
 */
Ball::Ball( double x, double y, int player ) : x( x ), y( y ) {}

/* This function returns a list of all components that have the input string as their type
 *
 * @param (string type) the desired "type" of component
 */
list<BaseClass*> Ball::getPart( string type ) {
  list<BaseClass*> res;
  for(BaseClass* elem: componentList) {
    if ((elem->type).compare( type ) == 0) res.push_back( elem );
  }
  return res;
}

/**
 * This function adds a component to the Ball's component list.
 * 
 * @param (BaseClass* c) the component to add to the Ball's list
 */
void Ball::addBaseClass( BaseClass* c) {
  componentList.push_back( c );
}

/*End Ball---------------------------------------------------------*/

/*Begin BaseClass-------------------------------------------------------*/

/* This function is a constructor for a general BaseClass. 
 *  
 * @param (Ball* parent) the parent of the new BaseClass
 * @param (string type) the type of the new component
 * @param (int player) flag telling which player does this ball belong to
 */

BaseClass::BaseClass( Ball* parent, string type, int player ) : parent ( parent ), type ( type ), player ( player ) {
  BaseClass::components.push_back( this ); 
  parent->addBaseClass( this );
};

/* This function is returns a number of balls created 
 */
int BaseClass::numberOfBalls () {
  int ballCount = 0;
  for(BaseClass* elem: BaseClass::components) {
    // we only count components that are really rendering the ball - therefore, CircleRenderer
    if (elem->type == "CircleRender") {
      ballCount++;
    }
  }
  return ballCount;
}

/* This function is intended to be run at a variable interval (useful for things like rendering)
 * The defualt implementation does nothing
 *
 * @param (float dt) time since the last update in milliseconds
 * @param (int x) position of the component on x axis
 * @param (int y) position of the component on y axis
 */
void BaseClass::update( float dt, int x, int y ) {}

/* This function is intended to be run at a set interval (useful for things like physics)
 * The defualt implementation does nothing.
 *
 * @param (float dt) time since the last fixedUpdate in milliseconds
 * @param (double dx) velocity on x axis
 * @param (double dy) velocity on y axis
 */

void BaseClass::fixedUpdate( float dt, double dx, double dy ) {}

/* This function takes in a float (dt) representing the time since the last fixedUpdate
 * This function is intended to be run at a variable interval (useful for things like rendering)
 * It runs the update(float dt) funtion for every component
 */

/* This function runs update() on every component
 *
 * @param (float dt) time since the last fixedUpdate in milliseconds
 * @param (int x) position of the component on x axis
 * @param (int y) position of the component on y axis
 */
void BaseClass::updateAll( float dt, int x, int y ){
  //printf("floatingUpdateDt:%f \n", (float) dt);
  for(BaseClass* elem: BaseClass::components){
    if (elem->player == GameState::playerActive) {
      elem->update( dt, x, y );
      continue;
    }
    elem->update(dt, -1, -1);
  }
};

/* This function runs fixedUpdate() on every component
 *
 * @param (float dt) time since the last fixedUpdate in milliseconds
 * @param (double dx) velocity on x axis
 * @param (double dy) velocity on y axis
 */
void BaseClass::fixedUpdateAll( float dt, double dx = 0, double dy = 0 ){
  //printf("fixedUpdateDt:%f \n", (float) dt);
  for(BaseClass* elem: BaseClass::components){
    if (elem->player == GameState::playerActive) {
      elem->fixedUpdate( dt, dx, dy );
      continue;
    }
    elem->fixedUpdate(dt, 0, 0);
  }
};

/*End BaseClass-------------------------------------------------------*/


/*Begin Collider (extends BaseClass)-------------------------------------------------------*/

list<Collider*> Collider::allColliders;

/* The constructor for a Collider. 
 *
 * @param (Ball* parent) the parent of the new BaseClass
 * @param (double radius) the collision radius of the object
 * @param (int player) flag telling which player does this ball belong to
 */
Collider::Collider( Ball* parent, double radius, int player ) :
  radius( radius ), BaseClass ( parent, "Collider", player){
  Collider::allColliders.push_back( this );
  list<triggerFunc> triggers; //this list is all colliders
};

/* This function adds a trigger function (fuction called when there is a collision) to the collider.
 *
 * @param (triggerFunc trigger) a trigger function to be called when the collider detects a collision.
 */
void Collider::addTrigger(triggerFunc trigger) {
  triggers.push_back(trigger);
}

/* This function calulates the disatnce between 2 Balls 

 * @param (Ball* obj1)
 * @param (Ball* obj2)
 */
float calculateDistance(Ball* obj1, Ball* obj2) {
  return sqrt( pow((obj1->x - obj2->x), 2) + pow((obj1->y - obj2->y), 2) );
}

/**
 * This function detects if this collider is colliding with any other colliders 
 * that are not attached to the same parent object. If it detects a collision, 
 * it will invoke any collision resolution functions attached to the current collider.
 *
 * @param (float dt) The elapsed time since the last fixedUpdate in milliseconds
 * @param (double dx) velocity on x axis
 * @param (double dy) velocity on y axis
 */

void Collider::fixedUpdate(float dt, double dx, double dy) {
  // Retrieve a list of all colliders attached to the parent
  list <BaseClass*> parentList =  parent->getPart("Collider");

  Collider* parentCollider;
  for(Collider* otherCollider: Collider::allColliders) {
    for(BaseClass* elem: parentList) {
      // Skip this loop iteration if the other collider in consideration is
      // attached to the same object as the current collider
      parentCollider = (Collider*) elem;
      if (otherCollider == parentCollider) continue;

      // If there is no collision, skip this loop iteration
      float distance = calculateDistance(otherCollider->parent, parent);
      if (distance > this->radius + otherCollider->radius) {
        continue;
      }

      // Otherwise, execute any registered collision resolution functions
      for (triggerFunc trigger: triggers) trigger(this, otherCollider, dt);
    }
  }
}

/*End Coliider-------------------------------------------------------*/


/*Begin CircleRender-------------------------------------------------------*/

/* The constructor for a CircleRender. 
 *
 * @param (Ball* parent) the parent of the new BaseClass
 * @param (double radius) the collision radius of the object
 * @param (int player) flag telling which player does this ball belong to
 */
CircleRender::CircleRender( Ball* parent, double radius, int player ) :
radius ( radius ), BaseClass(parent, string("CircleRender"), player) {
  setColor(0, 0, 1); // all balls are default blue
}

/* This function sets the color the circle should render as.  
 *
 * @param (float R) The value of red (0-255)
 * @param (float G) The value of greem (0-255)
 * @param (float B) The value of blue (0-255)
 */
void CircleRender::setColor(float R, float G, float B) {
  color.R = R;
  color.G = G;
  color.B = B;
}
/**
 * This function detects if this collider is colliding with any other colliders 
 *
 * @param (float dt) The elapsed time since the last update in milliseconds
 * @param (int x) - location of the mouse on x axis
 * @param (int y) - location of the mouse on y axis
 */
void CircleRender::update( float dt, int x, int y ) {
  GLfloat ballRadius = (GLfloat) radius;   // Radius of the bouncing ball
  if (x != -1) {
    // if the x is present, that means we're explicitly telling the ball how much to move in which direction
    // we have to divide it by 10k, as the numbers we need are in between 0 and 1
    parent->x = parent->x + ((double)x / 10000);
    parent->y = parent->y + ((double)y / 10000);
  }
  GLfloat ballX = parent->x;       // Ball's center (x, y) position
  GLfloat ballY = parent->y;


  glMatrixMode(GL_MODELVIEW);    // To operate on the model-view matrix
  glLoadIdentity();              // Reset model-view matrix

  glTranslatef(ballX, ballY, 0.0f);  // Translate to (xPos, yPos)
  // Use triangular segments to form a circle
     glColor3f(color.R, color.G, color.B);
     glutSolidSphere(ballRadius, 50, 30);
  glEnd();
}
/*End CircleRenderer-------------------------------------------------------*/

/*Begin Physics-------------------------------------------------------*/

/* The constructor for a Physics. 
 *
 * @param (Ball* parent) the parent of the new BaseClass
 * @param (double dx) the speed in the x direction
 * @param (double dy) the speed in the y direction
 * @param (double mass) the mass of the object
 * @param (int player) flag telling which player does this ball belong to
 */
Physics::Physics ( Ball* parent, double dx, double dy, double mass, int player) :
mass ( mass ), dx ( dx ), dy ( dy ), BaseClass (parent, string("Physics"), player) {};

/**
 * This function changes the state of the object at a fixed rate
 *
 * @param (float dt) The elapsed time since the last fixedUpdate in milliseconds
 * @param (double customDx) - velocity for the ball on x axis
 * @param (double customDy) - velocity for the ball on y axis
 */
void Physics::fixedUpdate( float dt, double customDx = 0, double customDy = 0 ) {

  //if we have values set, let's use them
  double newDx = customDx != 0 ? customDx /100000 : dx;
  double newDy = customDy != 0 ? customDy /100000 : dy;
  
  if (customDx == 0) {
    // if we don't have values, let's slow down the balls slowly
    double slowDown = 0.000002;
    bool newDxPositive = newDx > 0;
    bool newDyPositive = newDy > 0;
    double steps = 0;
    double slowDownX = 0;
    double slowDownY = 0;
    // let's calculate how many steps should we take before slowing down the ball on each dimension to stop
    // using the default slowdown speed
    // then use the highest number of steps as the value for the other dimension as well
    // to calculate by how much should we slow down the ball with each step
    if (abs(newDx / slowDown) > abs(newDy / slowDown)) {
      steps = abs(newDx / slowDown);
      if (steps != 0) {
        slowDownX = slowDown;
        slowDownY = abs(newDy / steps);
      }
    } else {
      steps = abs(newDy / slowDown);
      if (steps != 0) {
        slowDownY = slowDown;
        slowDownX = abs(newDx / steps);
      }
    }
    double reducedDx = newDxPositive ? newDx - slowDownX : newDx + slowDownX;
    double reducedDy = newDyPositive ? newDy - slowDownY : newDy + slowDownY;
    bool shouldLowerDx = !(newDxPositive == true && reducedDx < 0) && !(newDxPositive == false && reducedDx > 0);
    bool shouldLowerDy = !(newDyPositive == true && reducedDy < 0) && !(newDyPositive == false && reducedDy > 0);
    newDx = shouldLowerDx ? reducedDx : newDx;
    newDy = shouldLowerDy ? reducedDy : newDy;
    
    if (shouldLowerDx == false && shouldLowerDy == false) {
      newDx = 0;
      newDy = 0;
    }
  }
  // limit max speed of the balls
  if (newDx > 0.0015) {
    newDx = 0.0015;
  } else if (newDx < -0.0015) {
    newDx = -0.0015;
  }
  if (newDy > 0.0015) {
    newDy = 0.0015;
  } else if (newDy < -0.0015) {
    newDy = -0.0015;
  }


   Physics* ball = (Physics*) parent->getPart("Physics").front();
  if (newDx == 0 && newDy == 0) {
    // this ball stopped, notify game state
    GameState::ballStopped(ball->player);
  } else {
    GameState::ballRolling(ball->player);
  }
  
  dx = newDx;
  dy = newDy;
  parent->x += newDx * dt;
  parent->y += newDy * dt;
};

/*End Physics------------------------------------------------------*/

/*Begin WalBounceScript-------------------------------------------------------*/

/* This function is a constructor for a WallBounceScript. 
 * It takes in a Ball (parent) and a double (radius). It saves radius as an object variable.
 * It then runs the parent constructor for BaseClass (to complete setup).
 */

/* The constructor for a WallBounceScript. 
 *
 * @param (Ball* parent) the parent of the new BaseClass
 * @param (double radius) the radius of the ball
 * @param (int player) flag telling which player does this ball belong to
 */
WallBounceScript::WallBounceScript( Ball* parent, double radius, int player ) :
radius ( radius ), BaseClass (parent, string( "WallBounceScript" ), player) {};

/* This function takes in a float (dt) representing the time since the last fixedUpdate
 * This function updates the x and y speeds in the Physics component of the Ball to make objects bounce off walls.
 */

/**
 * This function snsures that Balls don't leave the screen
 *
 * @param (float dt) The elapsed time since the last fixedUpdate in milliseconds
 * @param (double customDx) - velocity on x axis
 * @param (double customDy) - velocity on y axis
 */
void WallBounceScript::fixedUpdate( float dt, double customDx, double customDy ) {
  double x = parent->x;
  double y = parent->y;
  Physics* physComp = dynamic_cast<Physics*> (parent->getPart(string("Physics")).front());
  double dx = physComp->dx;
  double dy = physComp->dy;

  if (x+radius >= 1) {
    dx = -1 * fabs(dx);
  }
  if (x-radius <= -1) {
    dx = fabs(dx);
  }
  if (y+radius >= 1) {
    dy = -1 * fabs(dy);
  }
  if (y-radius <= -1) {
    dy = fabs(dy);
  }

  physComp->dx = dx;
  physComp->dy = dy;
}
/*End WalBounceScript-------------------------------------------------------*/
