#include "gameLoopConstants.h"
#include "gameloop.h"
#include "gamestate.h"

#include <cstring>

GameloopState gameloopState;

void writeGameInfo() {
  // light will keep us away from coloring stuff
  glDisable(GL_LIGHTING);
  //draw table
  glLoadIdentity();
  glColor3f( 0.1f, 0.1f, 0.1f);
  glRectf(0.3f, 1.0f, 1.0f, 0.8f);

  const static char scores[8] = "Scores:";
  static char player1score[20];
  static char player2score[20];
  sprintf(player1score, "Player 1: %dpts", GameState::p1Score);
  sprintf(player2score,  "Player 2: %dpts", GameState::p2Score);

  glLoadIdentity();
  glColor3f(0.7f, 0.7f, 0.7f );
  // first print scores line
  glRasterPos2f(0.35, 0.95 );
  for( int i = 0; i < 7; i++ ) {
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, scores[i] );
  }
  // next print P1 score
  glRasterPos2f(0.35, 0.89 );
  for( int i = 0; i < 15; i++ ) {
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, player1score[i] );
  }
  // next print P2 score
  glRasterPos2f(0.35, 0.83 );
  for( int i = 0; i < 15; i++ ) {
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, player2score[i] );
  }

  // print current player
  glRasterPos2f(0.7, 0.95 );
  static const char activePlayer[22] = "Active player ";
  for( int i = 0; i < 21; i++ ) {
    glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, activePlayer[i] );
  }
  glRasterPos2f(0.8, 0.89);
  glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, GameState::playerActive + '0');
 
  // and player ball color

  color_t playerColor = 
    GameState::playerActive == 1
      ? firstPlayerBallColor
      : secondPlayerBallColor;
  glColor3f(playerColor.R, playerColor.G, playerColor.B);
  glTranslatef(0.88, 0.905, 0);
  glutSolidSphere(0.02, 50, 30);
  int numSegments = 100;
  glEnd();

  glEnable(GL_LIGHTING);
}

void calculateDirections (int oldx, int oldy, int x, int y) {
  gameloopState.dx = double(x) - oldx;
  gameloopState.dy = -(double(y) - oldy);
}
/* This function receives mouse actions, which we'll use to handle our dragging of the balls
*/
void ODLGameLoop_onMouse(int button, int state, int mousex, int mousey) {
  if (GameState::didPlayerStart == true) {
    // if the player already started play, it shouldn't be able to do it again until all balls are still
    return;
  }
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    gameloopState.startedDragging = 1;
    gameloopState.mousePrevX = double(mousex);
    gameloopState.mousePrevY = double(mousey);
  }
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && gameloopState.startedDragging == 1) {
    calculateDirections(gameloopState.mousePrevX, gameloopState.mousePrevY, mousex, mousey);
    gameloopState.startedDragging = 2;
  }
}

/* This function handles refreshing the display window
 */
void ODLGameLoop_onOpenGLDisplay() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Screen
  double now = glutGet(GLUT_ELAPSED_TIME);
  double dt = now - gameloopState.lastLoopTime;
  //printf("lastLoop:%f now:%f dt:%f \n", gameloopState.lastLoopTime, now, dt);
  int mousePrevX = gameloopState.startedDragging == 1 ? (int)gameloopState.moveBy.x : -1;
  int mousePrevY = gameloopState.startedDragging == 1 ? (int)gameloopState.moveBy.y : -1;
  BaseClass::updateAll(dt, mousePrevX, mousePrevY);
  writeGameInfo();
  glutSwapBuffers();
}

/* This function controls the speed of fixedUpdates and ensures they run at a fixed interval.
 */
void ODLGameLoop_onOpenGLIdle() {

  double now = glutGet(GLUT_ELAPSED_TIME);
  double timeElapsedMs = ((now-gameloopState.lastLoopTime)*1000000)/(CLOCKS_PER_SEC);


  gameloopState.timeAccumulatedMs += timeElapsedMs;

  while(gameloopState.timeAccumulatedMs >= DESIRED_STATE_UPDATE_DURATION_MS) { //
  	  //printf("dt:%f \n", gameloopState.timeAccumulatedMs);

      double dx = gameloopState.startedDragging == 2 ? gameloopState.dx : 0;
      double dy = gameloopState.startedDragging == 2 ? gameloopState.dy : 0;
      BaseClass::fixedUpdateAll( (float) gameloopState.timeAccumulatedMs, dx, dy);
      // now reset back startedDragging
      if (gameloopState.startedDragging == 2) {
        gameloopState.startedDragging = 0;
      }
      //ODLGameLoop_updateState();
      gameloopState.timeAccumulatedMs -= DESIRED_STATE_UPDATE_DURATION_MS;
      glutPostRedisplay();
  }

  gameloopState.lastLoopTime = now;
  writeGameInfo();
}


/* This function initializes glut and begins the game loop
 */
void ODLGameLoop_initOpenGL() {
    char title[] = "Karambol";
    int windowWidth  = VIEW_WIDTH;
    int windowHeight = VIEW_HEIGHT;
    int windowPosX   = WINDOW_POS_X;
    int windowPosY   = WINDOW_POS_Y;

    char *my_argv[] = { (char*) "dummyArgs", NULL };
    int   my_argc = 1;
    glutInit(&my_argc, my_argv);

    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(windowPosX, windowPosY);
    glutCreateWindow(title);


    glShadeModel(GL_SMOOTH);              // Enable Smooth Shading
    glClearColor(0.02f, 0.26f, 0.16f, 1.0f);       // Black Background
    // glClearColor(0.0, 0.0, 0.0, 1.0);
    glClearDepth(1.0f);                 // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);              // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);               // The Type Of Depth Testing To Do
    glEnable(GL_COLOR_MATERIAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    GLfloat qaBlack[] = {0.0, 0.0, 0.0, 1.0}; //Black Color
    GLfloat qaGreen[] = {0.0, 1.0, 0.0, 1.0}; //Green Color
    GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0}; //White Color
    GLfloat qaRed[] = {1.0, 0.0, 0.0, 1.0}; //Red Color

        // Set lighting intensity and color
    GLfloat qaAmbientLight[]    = {0, 0, 0, 1.0};
    GLfloat qaDiffuseLight[]    = {1.0, 1.0, 1.0, 1.0};
    GLfloat qaSpecularLight[]    = {1.0, 1.0, 1.0, 1.0};
        // Light source position
    GLfloat qaLightPosition[]    = {1, 1, -2, 0.5};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    // Set lighting intensity and color
    glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);
    
    // Set the light position
    glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
    
    // gluPerspective(60 * (float)VIEW_WIDTH/VIEW_HEIGHT, (float)VIEW_WIDTH/VIEW_HEIGHT, 0.01f, 100.0f);

    glutDisplayFunc(ODLGameLoop_onOpenGLDisplay); //set function that displays things
    glutIdleFunc(ODLGameLoop_onOpenGLIdle); //set function to update state
    glutMouseFunc(ODLGameLoop_onMouse);

    glutMainLoop();

}
