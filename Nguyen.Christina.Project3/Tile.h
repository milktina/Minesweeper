//
// Created by Christina Nguyen on 12/3/22.
//
#pragma once
#include <iostream>
#include "Board.h"
#include "TextureManager.h"
#include <SFML/Graphics.hpp>

using namespace std;

class Tile {
public:
    int xCoord = 0;
    int yCoord = 0;

    sf::Texture tileTexture;
    sf::Sprite tileSprite;

    Tile();
    Tile(string fileName, int row, int column);

//    int numMines;
    bool aMine = false;

    bool revealed = false;
    bool flagged = false;
    bool isLoseFace = false;
    bool debugClicked = false;
    bool hasNum = false;

    void SetPosition (int x, int y);
    void SetTexture(string fileName);
    const sf::Sprite& GetSprite() const;
    void reveal ();
    void notRevealed ();
    bool wasRevealed ();
    void flag ();
    void notFlagged ();
    bool wasFlagged ();
    void isMine();
    void isNotAMine();
    bool labeledMine();
    void debugClick();
    void hasANum();
    void hasNoNum();
    void placeFlag(int x, int y, vector<vector<Tile*>> gameBoard, int boardMineCount);
    void revealingTiles(int x, int y, vector<vector<Tile*>> gameBoard, int boardWidth, int boardHeight, vector<Tile*> gameButtons, bool gameLose);
    void recursiveReveal(int x, int y, const vector<vector<Tile*>> gameBoard, int boardWidth, int boardHeight);
    bool checkAdjacentTiles (int x, int y, const vector<vector<Tile*>> gameBoard);
};

Tile::Tile() {
    tileSprite.setTexture(TextureManager::GetTexture("tile_hidden"));
}

Tile::Tile(string fileName, int row, int column) {
    tileSprite.setTexture(TextureManager::GetTexture(fileName));
    xCoord = row * 32;
    yCoord = column * 32;
    tileSprite.setPosition(xCoord, yCoord);
}

void Tile::SetPosition(int x, int y) {
    xCoord = x * 32;
    yCoord = y * 32;
    sf::Vector2f coordinates2((float)xCoord, (float)yCoord);
    tileSprite.setPosition(coordinates2);
}

void Tile::SetTexture(string fileName) {
    tileSprite.setTexture(TextureManager::GetTexture(fileName));
}

const sf::Sprite& Tile::GetSprite() const {
    return tileSprite;
}

void Tile::reveal() {
    revealed = true;
}

void Tile::notRevealed() {
    revealed = false;
}

bool Tile::wasRevealed() {
    return revealed;
}

void Tile::flag () {
    flagged = true;
//    tileSprite.setTexture(TextureManager::GetTexture("flag"));
}

void Tile::notFlagged() {
    flagged = false;
//    tileSprite.setTexture(TextureManager::GetTexture("tile_hidden"));
}

bool Tile::wasFlagged() {
    return flagged;
}

void Tile::isMine() {
    aMine = true;
}

void Tile::isNotAMine() {
    aMine = false;
}

bool Tile::labeledMine() {
    return aMine;
}

void Tile::debugClick() {
    debugClicked = !debugClicked;
}

void Tile::hasANum() {
    hasNum = true;
}

void Tile::hasNoNum() {
    hasNum = false;
}

void Tile::placeFlag(int x, int y, vector<vector<Tile*>> gameBoard, int boardMineCount) {
    if (/*gameBoard[x][y]->flagged ||*/ !gameBoard[x][y]->revealed) {
        if (/*!gameBoard[x][y]->revealed &&*/ !gameBoard[x][y]->flagged) {
            gameBoard[x][y]->flag();
            gameBoard[x][y]->SetTexture("flag");
            boardMineCount--;
        }
        else if (gameBoard[x][y]->flagged) {
            gameBoard[x][y]->notFlagged();
            gameBoard[x][y]->SetTexture("tile_hidden");
            boardMineCount++;
        }
    }
}

void Tile::revealingTiles(int x, int y, vector<vector<Tile*>> gameBoard, int boardWidth, int boardHeight, vector<Tile*> gameButtons, bool gameLose) {
    if (!gameBoard[x][y]->flagged) {
        gameBoard[x][y]->reveal();
        gameBoard[x][y]->SetTexture("tile_revealed");
    }

    if (gameBoard[x][y]->aMine && !gameBoard[x][y]->flagged) {
        for (int i2 = 0; i2 < boardWidth; i2++) {
            for (int j2 = 0; j2 < boardHeight; j2++) {
                if (gameBoard[i2][j2]->aMine) {
                    gameBoard[i2][j2]->reveal();
                    gameBoard[i2][j2]->SetTexture("tile_revealed");
                    gameButtons[4]->SetTexture("face_lose");
                    gameButtons[4]->isLoseFace = true;
//                    gameLose = true;
                }
            }
        }
    }
//    else if (gameBoard[x][y]->flagged) {
//        gameBoard[x][y]->flagged;
//    }
}

bool Tile::checkAdjacentTiles (int x, int y, const vector<vector<Tile*>> gameBoard) {
    bool clear = false;
    int numMines = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i != 0 && j != 0) {
                if (((x-i) >= 0) && ((x-i) < gameBoard.size()) && ((y-j) >= 0) && ((y-j) < gameBoard[0].size())) {
                    if (gameBoard[x-i][y-j]->aMine) {
                        numMines++;
                    }
                }
            }
        }
    }

//    for (int i = -1; i <= 1; i++) {
//        for (int j = -1; j <= 1; j++) {
//            if (i != 0 && j != 0) {
//                if (((x-(i*2)) >= 0) && ((x-(i*2)) < gameBoard.size()) && ((y-(j*2)) >= 0) && ((y-(j*2)) < gameBoard[0].size())) {
//                    if (gameBoard[x-(i*2)][y-(j*2)]->aMine) {
//                        hasNum = true;
//                    }
//                }
//            }
//        }
//    }

    if (numMines == 0) {
        clear = true;
    }
    else if (numMines > 0) {
        clear = false;
    }

    return clear;
}

void Tile::recursiveReveal(int x, int y, const vector<vector<Tile*>> gameBoard, int boardWidth, int boardHeight) {
////    if (!gameBoard[x][y]->aMine) {
//    if ((x >= 0) && (x < boardWidth) && (y >= 0) && (y < boardHeight) && !gameBoard[x][y]->hasNum && !gameBoard[x][y]->flagged
//    && !gameBoard[x][y]->aMine) {
        gameBoard[x][y]->reveal();
        gameBoard[x][y]->SetTexture("tile_revealed");
//    }
//
////        cout << "revealing" << endl;
//    }
//    if (gameBoard[x][y]->hasNum) {
//        cout << "true" << endl;
//    }
//    if (!gameBoard[x][y]->hasNum) {
//        cout << "false" << endl;
//    }
    if(checkAdjacentTiles(x, y, gameBoard)) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i != 0 || j != 0) {
                    if (((x+i) >= 0) && ((x+i) < boardWidth) && ((y+j) >= 0) && ((y+j) < boardHeight))
                    {
                        if (!gameBoard[x + i][y + j]->flagged && !gameBoard[x + i][y + j]->revealed && !gameBoard[x + i][y + j]->aMine) {
                            if (!gameBoard[x + i][y + j]->hasNum) {
                                gameBoard[x + i][y + j]->reveal();
                                gameBoard[x + i][y + j]->SetTexture("tile_revealed");
                                recursiveReveal(x + i, y + j, gameBoard, boardWidth, boardHeight);
                            }
                            else if (gameBoard[x+i][y+j]->hasNum) {
                                gameBoard[x + i][y + j]->reveal();
                                gameBoard[x + i][y + j]->SetTexture("tile_revealed");
                            }
                        }
                    }
                }
            }
        }
    }
}

#ifndef PROJECT3MINESWEEPERNEW2_TILE_H
#define PROJECT3MINESWEEPERNEW2_TILE_H

#endif //PROJECT3MINESWEEPERNEW2_TILE_H
