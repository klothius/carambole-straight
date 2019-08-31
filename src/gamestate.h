#ifndef GAMESTATE_H
#define GAMESTATE_H

class GameState {
    public:
      static int playerActive;
      static int p1Score;
      static int p2Score;
      static bool didPlayerStart;
      static void activePlayerScored();
      static void switchPlayer();
      static void hitBalls(int b1, int b2);
      static void ballStopped(int player);
      static void ballRolling(int player);

    protected:
      static bool isPlayBallTouched;
      static bool isOpponentBallTouched;
      static bool isP0BallMoving;
      static bool isP1BallMoving;
      static bool isP2BallMoving;
      static void resetState();
      static void touchBall(int ball);
      static bool shouldGetPoint();
};

#endif
