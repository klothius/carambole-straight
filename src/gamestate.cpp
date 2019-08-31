#include "gamestate.h";

using namespace std;

void GameState::activePlayerScored() {
  switch (GameState::playerActive) {
    case 1:
      GameState::gameScore.playerOne++;
      break;
    case 2:
      GameState::gameScore.playerTwo++;
      break;
  };
};
void GameState::switchPlayer() {
  if (GameState::playerActive == 1) {
    GameState::playerActive = 2;
  } else {
    // we know we have only 2 players
    // so if active is second player, switching it would be to the first one again
    GameState::playerActive = 1;
  }
}