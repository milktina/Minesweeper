#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include "TextureManager.h"
#include "Random.h"
#include "Tile.h"
#include <SFML/Graphics.hpp>

using namespace std;

class Board {
public:
    struct Mine {
        int x;
        int y;
    };
    int boardWidth;
    int boardHeight;
    int windowWidth;
    int windowHeight;
    int xCoord;
    int yCoord;
    int mineCount;
    int tileCount;
    string widthValue;
    string heightValue;
    string mineCountValue;

//    void makeTilesArray();
//    Tile gameBoard[boardWidth][boardHeight];

//    Tile tile;
//    vector<Tile*> board;

//    sf::Sprite gameBoard[][];

    void ReadFile(string filename);
    void SetBoardWidth (string widthValue);
    void SetBoardHeight (string heightValue);
    void SetWindowWidth (int widthValue);
    void SetWindowHeight (int heightValue);
    void SetMineCount (int mineCount);
    void SetTileCount (int width, int height);
    int CountFlags(vector<vector<Tile*>> gameBoard);

//    void DrawBoard(sf::RenderWindow &window);
    void DrawButtons(sf::RenderWindow &window);
    void Draw(sf::RenderWindow &window, string fileName, int x, int y);
    void ResetGame (vector<vector<Tile*>> gameBoard, vector<Tile*> gameButtons, bool lose, bool win);
    void RandomizeMines(vector<vector<Tile*>> gameBoard);
    int DrawBoard(vector<vector<Tile*>> gameBoard);
    bool CheckGameWin (vector<vector<Tile*>> gameBoard, vector<Tile*> gameButtons, bool gameWin, bool gameLose, int mineCount);

//    void Tile::CountMines(Tile tileArray[][]);
};

//Board::Board() {
//
//}

void Board::ReadFile(string filename) {
    ifstream file;
    file.open(filename);
    if(!file.is_open())
    {
        cout<<"Unable to open the file."<<endl;
    }

    getline(file, widthValue);
    SetBoardWidth(widthValue);
    SetWindowWidth(stoi(widthValue));
    getline(file, heightValue);
    SetBoardHeight(heightValue);
    SetWindowHeight(stoi(heightValue));
    getline(file, mineCountValue);
    SetMineCount(stoi(mineCountValue));

//    for (int i = 0; i < boardHeight; i++) {
//        for (int j = 0; j < boardWidth; j++) {
//            cout << "x ";
//        }
//        cout << endl;
//    }

    SetTileCount(stoi(widthValue), stoi(heightValue));

    file.close();
}

void Board::SetBoardWidth(std::string widthValue) {
    this->boardWidth = stoi(widthValue);
}

void Board::SetBoardHeight(std::string heightValue) {
    this->boardHeight = stoi(heightValue);
}
void Board::SetWindowWidth (int configWidthValue) {
    this->windowWidth = (configWidthValue * 32) ;
}

void Board::SetWindowHeight (int configHeightValue) {
    this->windowHeight = (configHeightValue * 32) + 100;
}

void Board::SetMineCount(int configMineCount) {
    this->mineCount = configMineCount;
}

void Board::SetTileCount (int rows, int columns) {
    this->tileCount = rows * columns;
}

void Board::DrawButtons(sf::RenderWindow &window) {
    sf::Sprite smileyFace(TextureManager::GetTexture("face_happy"));

    smileyFace.setPosition((boardHeight * 52)/2, boardHeight + 495);
    window.draw(smileyFace);
}

void Board::Draw(sf::RenderWindow &window, string fileName, int x, int y) {
    sf::Sprite drawableSprite;

    drawableSprite.setTexture(TextureManager::GetTexture(fileName));
    drawableSprite.setPosition(x, y);

    window.draw(drawableSprite);
}

void Board::ResetGame (vector<vector<Tile*>> gameBoard, vector<Tile*> gameButtons, bool lose, bool win) {
    lose = false;
    win = false;

    for (int i = 0; i < boardWidth; i++) {
        for (int j = 0; j < boardHeight; j++) {
            gameBoard[i][j]->isNotAMine();
            gameBoard[i][j]->notRevealed();
            gameBoard[i][j]->notFlagged();
            gameBoard[i][j]->SetTexture("tile_hidden");
            gameButtons[4]->SetTexture("face_happy");
        }
    }

    for (int k = 0; k < boardWidth; k++) {
            gameBoard[k].clear();
    }

    DrawBoard(gameBoard);
}

void Board::RandomizeMines(vector<vector<Tile*>> gameBoard) {
    float count = mineCount;
    cout << count << endl;
        while (count > 0) {
        int mineRandomX = Random::Int(0, boardWidth - 1);
        int mineRandomY = Random::Int(0, boardHeight - 1);

        if (!gameBoard[mineRandomX][mineRandomY]->aMine) {
            count--;
            gameBoard[mineRandomX][mineRandomY]->isMine();
        }
    }
}

int Board::CountFlags(vector<vector<Tile*>> gameBoard) {
    int numFlags = 0;

    for (int i = 0; i < boardWidth; i++) {
        for (int j = 0; j < boardHeight; j++) {
            if (gameBoard[i][j]->flagged) {
             numFlags++;
            }
        }
    }
    return numFlags;
}

int Board::DrawBoard(vector<vector<Tile*>> gameBoard) {
    for (int i = 0; i < boardWidth; i++) {
        gameBoard[i] = vector<Tile *>(boardHeight);
        for (int j = 0; j < boardHeight; j++) {
            Tile *aTile = new Tile("tile_hidden", i, j);
            gameBoard[i][j] = aTile;
        }
    }
}


bool Board::CheckGameWin (vector<vector<Tile*>> gameBoard, vector<Tile*> gameButtons, bool gameWin, bool gameLose, int mineCount) {
    int notWin = 0;
    bool win = false;

    if (!gameLose) {
        cout << "working" << endl;
        for (int i = 0; i < boardWidth; i++) {
            for (int j = 0; j < boardHeight; j++) {
                if ((!gameBoard[i][j]->aMine && gameBoard[i][j]->revealed) ||
                    (gameBoard[i][j]->aMine && gameBoard[i][j]->flagged)) {
                    continue;
                }
                else if ((!gameBoard[i][j]->aMine && !gameBoard[i][j]->revealed) ||
                           (gameBoard[i][j]->aMine && !gameBoard[i][j]->flagged))
                    notWin++;
            }
        }
    }

//    gameWin = true;

    if (notWin == 0) {
        win = true;
        gameButtons[4]->SetTexture("face_win");
        win = true;
//        return true;
        gameWin = true;
        cout << "is win" << endl;
//        gameButtons[4]->SetTexture("face_win");
    }
    else if (notWin > 0) {
        gameButtons[4]->SetTexture("face_lose");
//        return false;
        gameLose = true;
//        gameButtons[4]->SetTexture("face_lose");
    }

    return win;
}