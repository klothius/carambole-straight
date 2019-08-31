#ifndef COMPONENTS_H
#define COMPONENTS_H

struct color_t {
  float R;
  float G;
  float B;
};

const double playBallPositionX = 0.8;
const double playBallPositionY = 0;
const color_t playBallColor {
  1,
  0,
  0
};
const double firstPlayerBallPositionX = 0;
const double firstPlayerBallPositionY = -0.3;
const color_t firstPlayerBallColor {
  1,
  1,
  1
};
const double secondPlayerBallPositionX = 0;
const double secondPlayerBallPositionY = 0.3;
const color_t secondPlayerBallColor {
  0.91,
  0.84,
  0.42
};

#include <list>
#include <string>

using namespace std;

class BaseClass; // Forward declaration to resolve circular dependency
class Ball {
  friend class BaseClass;
  private:
  	list<BaseClass*> componentList;
  	void addBaseClass( BaseClass* c );

  public:
  	Ball( double x, double y, int player );
  	list<BaseClass*> getPart( string type );	// Returns a list of all components matching the type parameter
  	double x;
  	double y;
};

class BaseClass {
	private:
  	static list<BaseClass*> components;

  public:
  	BaseClass( Ball* parent, string type, int player ); // Similar as an init method: In this case, initilizes the type and parent fields, and adds self to the static components list.
  	static int numberOfBalls();
    static void updateAll( float dt, int x, int y );  // Run all variable updates (eg renderAll)
  	static void fixedUpdateAll( float dt, double dx, double dy ); // Updates on fixed interval (eg physicsUpdateAll)
  	virtual void update( float dt, int x, int y);
  	virtual void fixedUpdate( float dt, double dx, double dy );
  	const string type;
    const int player;
  	Ball* const parent; // Declare the pointer constant without making the data constant
};

class CircleRender : public BaseClass {
  private:
    color_t color;
    double calculatePos(double pos);

  public:
    CircleRender( Ball* parent, double radius, int player);
    void setColor(float R, float G, float B);
    double radius;
    void update( float dt, int x, int y );
};

class Physics : public BaseClass {
  public:
    Physics ( Ball* parent, double dx = 0, double dy = 0, double mass = 1, int player = 0 );
    void fixedUpdate( float dt, double dx, double dy );
    double dx;
    double dy;
    double mass;
};

class WallBounceScript : public BaseClass {
  public:
    WallBounceScript( Ball* parent, double radius, int player );
    void fixedUpdate( float dt, double dx, double dy );
    double radius;
};

class Collider; //Forward declaration, alerts the compiler that collider is coming
  typedef void ( * triggerFunc ) ( Collider* c1, Collider* c2, float dt); //New type called triggerFunc, takes a Collider pointer returns void
  list<BaseClass*> BaseClass::components;
  class Collider : public BaseClass{
    private:
      double radius;
      static list<Collider*> allColliders; //List of colliders
      list<triggerFunc> triggers; //list of trigger functions

    public:
      void addTrigger(triggerFunc trigger);  //add trigger
      Collider( Ball* parent, double radius, int player); //Collider init
      void fixedUpdate ( float dt, double dx, double dy ); //Update for a collider
  };

#endif
