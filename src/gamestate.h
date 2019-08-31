struct gameScore {
    int playerOne = 0;
    int playerTwo = 0;
};

class GameState {
    public:
      int playerActive = 1;
      gameScore gameScore;
      void activePlayerScored();
      void switchPlayer();
};