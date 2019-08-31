#include "gamestate.h"
#include <stdio.h>
#include "components.h"

int GameState::playerActive = 1;
int GameState::p1Score = 0;
int GameState::p2Score = 0;
bool GameState::isP0BallMoving = false;
bool GameState::isP1BallMoving = false;
bool GameState::isP2BallMoving = false;
bool GameState::didPlayerStart = false;
bool GameState::isPlayBallTouched = false;
bool GameState::isOpponentBallTouched = false;

void GameState::activePlayerScored() {
  switch (GameState::playerActive) {
    case 1:
      GameState::p1Score++;
      break;
    case 2:
      GameState::p2Score++;
      break;
  };
};
bool GameState::shouldGetPoint() {
  return GameState::isPlayBallTouched && GameState::isOpponentBallTouched;
}
void GameState::switchPlayer() {
  resetState();
  if (GameState::playerActive == 1) {
    GameState::playerActive = 2;
  } else {
    // we know we have only 2 players
    // so if active is second player, switching it would be to the first one again
    GameState::playerActive = 1;
  }
}
void GameState::touchBall(int ball) {
  switch (ball) {
    case playBallIndex:
      GameState::isPlayBallTouched = true;
      break;
    case firstPlayerIndex:
      GameState::isOpponentBallTouched = true;
      break;
    case secondPlayerIndex:
      GameState::isOpponentBallTouched = true;
      break;
  }
}
void GameState::hitBalls(int b1, int b2) {
  if (b1 != GameState::playerActive) {
    GameState::touchBall(b1);
  }
  if (b2 != GameState::playerActive) {
    GameState::touchBall(b2);
  }
  if (GameState::shouldGetPoint() == true) {
    GameState::activePlayerScored();
  }
}
void GameState::ballRolling(int player) {
  switch (player) {
    case playBallIndex:
      GameState::isP0BallMoving = true;
      break;
    case firstPlayerIndex:
      GameState::isP1BallMoving = true;
      break;
    case secondPlayerIndex:
      GameState::isP2BallMoving = true;
      break;
  }
  if (player == GameState::playerActive) {
    GameState::didPlayerStart = true;
  }
}
void GameState::ballStopped(int ball) {
  if (GameState::didPlayerStart == false) {
    return;
  }
  switch (ball) {
    case playBallIndex:
      GameState::isP0BallMoving = false;
      break;
    case firstPlayerIndex:
      GameState::isP1BallMoving = false;
      break;
    case secondPlayerIndex:
      GameState::isP2BallMoving = false;
      break;
  }
  if(GameState::isP0BallMoving == false &&
    GameState::isP1BallMoving == false &&
    GameState::isP2BallMoving == false) {
    if (GameState::shouldGetPoint() == false) {
      switchPlayer();
    }
    GameState::resetState();
  }
}
void GameState::resetState() {
  GameState::didPlayerStart = false;
  GameState::isOpponentBallTouched = false;
  GameState::isPlayBallTouched = false;
}