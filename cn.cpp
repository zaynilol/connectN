/*
Name: Zayan Mehmood
Roll number: 23L-2628

Name: M. Abdul Rehman
Roll number: 23L-2543
*/

/*
We changed up the skeleton code a bit. We'll elaborate how our version of connect n works here.
Basically, We made the main menu work like an actual main menu because the old way seemed somewhat off.
What We did is that if you go back to the main menu during a game and press play again, it loads up that game you were playing.
We also added a new option to start a completely new game (if you don't want to play the game that you're currently playing and want to start anew).
Save game and load game work differently. If you load up a game and then press play, it'll start the game that you just loaded from a file.
Other than that, We just added some error handling to prevent the code from breaking to an extent.
We also added a "Start New Game" option. If you don't want to load back a currently paused game, you can start a new game from the main menu.
*/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int MaxRows = 14;
const int MaxCols = 14;
const int MaxPlayers = 2;
const char DefaultFill = '.';

char Board[MaxRows][MaxCols];

// Array to keep track of the next row in each column
// This will save us from searching for the next
// empty slot in each column
int Next_Move[MaxCols] = {0};

string PlayerNames[MaxPlayers];
char PlayerMarks[MaxPlayers] = {'o', 'x'};

// Turning player represents the player making the move
int TurningPlayer = 0;

// Game End State represents the result of the game
// with -1 means the game is not over,
// 0 means the first player has won it,
// and 1 means the second player is the winner
int GameEndState = 0;

// Number of rows to be used in this game
// The maximum value is MaxRows
int Rows = 6;

// Number of columns to be used in this game
// The maximum value is MaxCols
int Columns = 7;

// How many consecutive positions to win
int ConnectLimit = 4;

struct currentGameState{
    char Board[MaxRows][MaxCols];
    int Next_Move[MaxCols];
    string PlayerNames[MaxPlayers];
    char PlayerMarks[MaxPlayers];
    int TurningPlayer;
    int GameEndState;
    int Rows;
    int Columns;
    int ConnectLimit;
};

bool GameNotLoaded = true;

int Menu(){
    int Choice;
    do {
        cout << "1. Play" << endl
             << "2. Save Game" << endl
             << "3. Load Game" << endl
             << "4. Start New Game" << endl
             << "5. Exit" << endl
             << "Enter Your Choice (1 - 5): ";
        cin >> Choice;
    } while (Choice < 1 || Choice > 5);
    return Choice;
}


// We made the user unable to make a connectivity limit of 1 and a board of 1 row and/or 1 column to make the game more interesting

void initNewGame(){
    GameEndState = 0;
    for (int i = 0; i < MaxCols; i++)
        Next_Move[i] = 0;

    // Reset the game board
    for (int r = 0; r < MaxRows; r++)
        for (int c = 0; c < MaxCols; c++)
            Board[r][c] = DefaultFill;

    do {
        cout << "\nEnter the number of rows: ";
        cin >> Rows;
        if (Rows <= 1 || Rows > MaxRows) {
            cout << "You entered an invalid number of rows. Please enter a value greater than 1 and less than or equal to " << MaxRows << "." << endl;
        }
    } while (Rows <= 1 || Rows > MaxRows);

    do {
        cout << "Enter the number of columns: ";
        cin >> Columns;
        if (Columns <= 1 || Columns > MaxCols) {
            cout << "You entered an invalid number of columns. Please enter a value greater than 1 and less than or equal to " << MaxCols << "." << endl;
        }
    } while (Columns <= 1 || Columns > MaxCols);

    do {
        cout << "Enter the Connectivity Limit: ";
        cin >> ConnectLimit;

        int maxRowsColumns;
        if (Rows > Columns) {
            maxRowsColumns = Rows;
        }
        else{
            maxRowsColumns = Columns;
        }

        if (ConnectLimit <= 1 || ConnectLimit > maxRowsColumns) {
            cout << "You entered an invalid connectivity limit. Please enter a value greater than 1 and less than or equal to the maximum of rows and columns." << endl;
        }
    } while (ConnectLimit <= 1 || (ConnectLimit > Rows && ConnectLimit > Columns));

    cout << "First Player Name? ";
    cin >> PlayerNames[0];
    cout << "Second Player Name? ";
    cin >> PlayerNames[1];
}


bool CheckWin(int R, int C){
    char currentPlayerMark = PlayerMarks[TurningPlayer];

    // to check for win horizontally
    int count = 0;
    for (int c = 0; c < Columns; ++c){
        if (Board[R][c] == currentPlayerMark){
            count++;
            if (count == ConnectLimit)
                return true;
        }
        else{
            count = 0;
        }
    }

    // to check for win vertically vertically
    count = 0;
    for (int r = 0; r < Rows; ++r){
        if (Board[r][C] == currentPlayerMark){
            count++;
            if (count == ConnectLimit)
                return true;
        }
        else{
            count = 0;
        }
    }

    // to check for win from top-left to bottom-right
    count = 0;
    for (int i = -ConnectLimit + 1; i < ConnectLimit; ++i){
        int r = R + i;
        int c = C + i;
        if (r >= 0 && r < Rows && c >= 0 && c < Columns && Board[r][c] == currentPlayerMark) {
            count++;
            if (count == ConnectLimit)
                return true;
        }
        else{
            count = 0;
        }
    }

    // to check for win from top-right to bottom-left
    count = 0;
    for (int i = -ConnectLimit + 1; i < ConnectLimit; ++i){
        int r = R - i;
        int c = C + i;
        if (r >= 0 && r < Rows && c >= 0 && c < Columns && Board[r][c] == currentPlayerMark){
            count++;
            if (count == ConnectLimit)
                return true;
        }
        else{
            count = 0;
        }
    }

    return false; // no one win yet
}

void ShowGame();

void MakeMove(int Col){
    int R = Next_Move[Col];
    if (R >= Rows) {
        cout << "Column is full! Please choose another column." << endl;
        return;
    }

    Board[R][Col] = PlayerMarks[TurningPlayer];

    if (CheckWin(R, Col)){
        ShowGame();
        cout << "Player " << PlayerNames[TurningPlayer] << " wins!" << endl;
        GameEndState = TurningPlayer + 1; // set game end state to the winning player
    }

    TurningPlayer = (TurningPlayer + 1) % MaxPlayers;
    Next_Move[Col]++;
}

bool GameisOn(){
    return GameEndState == 0;
}

bool SaveGame(const string &FileName){
    ofstream outFile(FileName);

    if (!outFile.is_open()){
        cout << "Error: Unable to open file for saving.\n";
        return false;
    }

    // save game data to the file
    outFile << TurningPlayer << ' ' << GameEndState << ' '
            << Rows << ' ' << Columns << ' ' << ConnectLimit << ' ';

    for (int i = 0; i < MaxPlayers; ++i){
        outFile << PlayerNames[i] << ' ' << PlayerMarks[i] << ' ';
    }

    // to save the game board up to rows x columns
    for (int r = 0; r < Rows; ++r){
        for (int c = 0; c < Columns; ++c){
            outFile << Board[r][c] << ' ';
        }
    }

    // saving the Next_Move array up to Columns
    for (int i = 0; i < Columns; ++i) {
        outFile << Next_Move[i] << ' ';
    }

    outFile.close();
    return true;
}

bool LoadSavedGame(const string &FileName){
    ifstream inFile(FileName);

    if (!inFile.is_open()) {
        cout << "Error: Unable to open file for loading.\n";
        return false;
    }

    // load game data from the file
    inFile >> TurningPlayer >> GameEndState >> Rows >> Columns >> ConnectLimit;

    for (int i = 0; i < MaxPlayers; ++i) {
        inFile >> PlayerNames[i] >> PlayerMarks[i];
    }

    // load the game board up to Rows x Columns
    for (int r = 0; r < Rows; ++r) {
        for (int c = 0; c < Columns; ++c) {
            inFile >> Board[r][c];
            if (Board[r][c] != DefaultFill){
                Next_Move[c] = r + 1; // update Next_Move based on the loaded board
            }
        }
    }

    // load the Next_Move array up to Columns
    for (int i = 0; i < Columns; ++i){
        inFile >> Next_Move[i];
    }

    inFile.close();
    return true;
}

void SaveGameState(const string &FileName){
    //using the current game state struct declared in the start to save the game state to integrate a pause-play menu like mentioned in a comment at the start of my code
    currentGameState currentState;
    currentState.TurningPlayer = TurningPlayer;
    currentState.GameEndState = GameEndState;
    currentState.Rows = Rows;
    currentState.Columns = Columns;
    currentState.ConnectLimit = ConnectLimit;

    for (int i = 0; i < MaxPlayers; ++i){
        currentState.PlayerNames[i] = PlayerNames[i];
        currentState.PlayerMarks[i] = PlayerMarks[i];
    }

    for (int r = 0; r < Rows; ++r){
        for (int c = 0; c < Columns; ++c){
            currentState.Board[r][c] = Board[r][c];
        }
    }

    for (int i = 0; i < Columns; ++i){
        currentState.Next_Move[i] = Next_Move[i];
    }

    ofstream outFile(FileName, ios::trunc);
    if (!outFile.is_open()){
        cerr << "Error: Unable to open file for saving.\n";
        return;
    }

    outFile << currentState.TurningPlayer << ' ' << currentState.GameEndState << ' '
            << currentState.Rows << ' ' << currentState.Columns << ' ' << currentState.ConnectLimit << ' ';

    for (int i = 0; i < MaxPlayers; ++i){
        outFile << currentState.PlayerNames[i] << ' ' << currentState.PlayerMarks[i] << ' ';
    }

    for (int r = 0; r < currentState.Rows; ++r){
        for (int c = 0; c < currentState.Columns; ++c){
            outFile << currentState.Board[r][c] << ' ';
        }
    }

    for (int i = 0; i < currentState.Columns; ++i){
        outFile << currentState.Next_Move[i] << ' ';
    }

    outFile.close();
}

void ShowGame(){
    for (int c = 0; c < Columns; c++)
        cout << "_________";
    cout << endl;
    for (int c = 0; c < Columns; c++)
        cout << "\t" << c;
    cout << endl;
    for (int c = 0; c < Columns; c++)
        cout << "_________";

    cout << endl;

    for (int r = Rows - 1; r >= 0; r--) {
        cout << "| " << r << " |\t";
        for (int c = 0; c < Columns; c++)
            cout << Board[r][c] << "\t";
        cout << endl
             << "|   |" << endl;
    }

    for (int c = 0; c < Columns; c++)
        cout << "_________";
    cout << endl;

    if (GameisOn())
        cout << endl
             << "\tPlayer:" << PlayerNames[TurningPlayer]
             << "("
             << PlayerMarks[TurningPlayer] << ")"
             << " Moving " << endl;
}

void TerminateGame(){
    //this function will reset the game and like start a new game for example
    if (GameEndState > 0) {
        cout << "Player " << PlayerNames[GameEndState - 1] << " wins!" << endl;
    }
    cout << "The game is over. Starting a new game." << endl;

    GameEndState = 0;
    for (int i = 0; i < MaxCols; i++)
        Next_Move[i] = 0;

    // Reset the game board
    for (int r = 0; r < MaxRows; r++)
        for (int c = 0; c < MaxCols; c++)
            Board[r][c] = DefaultFill;

    GameNotLoaded = false;

    // Start a new game
    initNewGame();
    SaveGameState("current_game_state.txt"); // saving the new game too to overwrite the old game's data for pause menu
}

void Play(){
    int Move;
    do {
        ShowGame();
        if (!GameisOn()){
            TerminateGame(); 
            continue; 
        }

        cout << "\tENTER NEGATIVE NUMBER FOR THE MAIN MENU" << endl;
        cout << "\tPlease Enter Your Move ? (0 - " << Columns - 1 << "): ";
        cin >> Move;

        // to check if Move is within the valid range
        if (Move < 0 || Move >= Columns) {
            if (Move < 0){
                break; 
            }
            cout << "This is an invalid move please enter a valid column number" << endl;
            continue;
        }

        MakeMove(Move);

        // saving the current state of the game after each move
        SaveGameState("current_game_state.txt");

    } while (GameisOn());
}

int main(){
    int Choice;
    string FileName;

    for (int i = 0; i < MaxCols; i++)
        Next_Move[i] = 0;

    do {
        Choice = Menu();
        switch (Choice) {
        case 1:
            if (GameNotLoaded){
                initNewGame();
                GameNotLoaded = false;
            }
            Play();
            break;
        case 2:
            cout << "Enter the file name to save the game: ";
            cin >> FileName;
            if (SaveGame(FileName)){
                cout << "Game saved successfully!\n";
            }
            else{
                cout << "Failed to save the game.\n";
            }
            break;
        case 3:
            cout << "Enter the file name to load the game: ";
            cin >> FileName;
            if (LoadSavedGame(FileName)){
                cout << "Game loaded successfully!\n";
                GameNotLoaded = false;
            }
            else{
                cout << "Failed to load the game.\n";
            }
            break;
        case 4:
            GameNotLoaded = false;
            initNewGame();
            Play();
            break;
        }
    } while (Choice != 5);

    return 0;
}