struct gameScoreStruct {
    int playerOne = 0;
    int playerTwo = 0;
};

class GameState {
    public:
      static int playerActive;
      static gameScoreStruct gameScore;
      static void activePlayerScored();
      static void switchPlayer();
};
int GameState::playerActive = 1;
