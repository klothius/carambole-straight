#include "components.h"
#include "components.cpp"
#include "gameloop.cpp"

/* trigger function that cause the balls to "bounce" when called
 *
 * @param (Collider* c1) first collider involved in collision
 * @param (Collider* cc) first second involved in collision
*/
void Bounce(Collider* c1, Collider* c2, float dt){
  // Access the physics components of both balls
  Physics* p1 = (Physics*) c1->parent->getPart("Physics").front();
  Physics* p2 = (Physics*) c2->parent->getPart("Physics").front();

  // Calculate new velocities using One-dimensional relativistic formula
  float newv1x = (p1->dx * (p1->mass - p2->mass) +
    (2 * p2->mass * p2->dx)) / (p1->mass + p2->mass);
  float newv1y = (p1->dy * (p1->mass - p2->mass) +
    (2 * p2->mass * p2->dy)) / (p1->mass + p2->mass);
  float newv2x = (p2->dx * (p2->mass - p1->mass) +
    (2 * p1->mass * p1->dx)) / (p1->mass + p2->mass);
  float newv2y = (p2->dy * (p2->mass - p1->mass) +
    (2 * p1->mass * p1->dy)) / (p1->mass + p2->mass);

  float oldDx1 = p1->dx;
  float oldDy1 = p1->dy;
  float oldDx2 = p2->dx;
  float oldDy2 = p2->dy;
  
    // Assign velocities
    p1->dx = newv1x;
    p1->dy = newv1y;

    p2->dx = newv2x;
    p2->dy = newv2y;

    // Back the balls off by one step of their new velocities so that collision does not register twice
    p1->parent->x += newv1x * dt;
    p1->parent->y += newv1y * dt;

    p2->parent->x += newv2x * dt;
    p2->parent->y += newv2y * dt;

    //these are round objects, so they will not stop if an object is not moving.
    if (oldDx1 == 0) {
      p2->dx = (oldDx2 < 0 ? 1 : -1) * (oldDx2 / 4);
    }
    if (oldDx2 == 0) {
      p1->dx = (oldDx1 < 0 ? 1 : -1) * (oldDx1 / 4);
    }
    if (oldDy1 == 0) {
      p2->dy = (oldDy2 < 0 ? 1 : -1) * (oldDy2 / 4);
    }
    if (oldDy2 == 0) {
      p1->dy = (oldDy1 < 0 ? 1 : -1) * (oldDy1 / 4);
    }
}

/* Creates a ball and associated components
 *
 * @param (double x) the starting x position of the ball
 * @param (double y) the starting y position of the ball
 * @param (double dx) the starting x speed of the ball
 * @param (double dy) the starting y speed of the ball
 * @param (double radius) the radius of the ball
*/
Ball* makeBall(double x, double y, double dx, double dy, double radius) {
  float mass = PI * pow(radius, 2);
  int player = BaseClass::numberOfBalls();
  Ball* obj = new Ball(x, y, player);
  CircleRender* circleRender = new CircleRender(obj, radius, player);
  switch(player) {
    case 0:
      circleRender->setColor(playBallColor.R, playBallColor.G, playBallColor.B);
      break;
    case 1:
      circleRender->setColor(firstPlayerBallColor.R, firstPlayerBallColor.G, firstPlayerBallColor.B);
      break;
    case 2:
      circleRender->setColor(secondPlayerBallColor.R, secondPlayerBallColor.G, secondPlayerBallColor.B);
      break;
  }
  Collider* collider = new Collider(obj, radius, player);
  Physics* physics = new Physics(obj, dx, dy, mass, player);
  WallBounceScript* wallBounceScript = new WallBounceScript(obj, radius, player);
  collider->addTrigger(Bounce);
  return obj;
}
/* Creates all balls needed to play 
*/
void makeAllBalls() {
  // play ball
  makeBall(playBallPositionX, playBallPositionY, 0, 0, 0.1);

  // first player ball
  makeBall(firstPlayerBallPositionX, firstPlayerBallPositionY, 0, 0, 0.1);

  // second player ball
  makeBall(secondPlayerBallPositionX, secondPlayerBallPositionY, 0, 0, 0.1);
  
  //moar balls
  // for(int i = 0; i < 3; i++) {
  //   double posX = (float)rand() / RAND_MAX;
  //   double posY = (float)rand() / RAND_MAX;
  //   makeBall(posX, posY, 0, 0, 0.1);
  // }
}
/* Creates a number of balls and starts the game loop
 *
*/
int main() {
  makeAllBalls();
  ODLGameLoop_initOpenGL();
}
