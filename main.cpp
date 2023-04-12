#include <iostream>
#include <array>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Tile.h"
#include "Digits.h"
#include "TextureManager.h"
#include "Random.h"

bool isValid(int row, int column) {
    Board boardData;
    boardData.ReadFile("config.cfg");

    return (row >= 0) && (row < boardData.boardWidth) && (column >= 0) && (column < boardData.boardHeight);
}

int CountMines(int row, int column, vector<vector<Tile*>> board) {
    Board boardData;
    boardData.ReadFile("config.cfg");

    int count = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (isValid(row-i, column-j)) {
                if (board[row-i][column-j]->aMine) {
                    count++;
                }
            }
        }
    }
    return count;
}

int CountNumMines (int boardWidth, int boardHeight, vector<vector<Tile*>> gameBoard) {
    int numOfMines = 0;
    for (int i = 0; i < boardWidth; i++) {
        for (int j = 0; j < boardHeight; j++) {
            if (gameBoard[i][j]->aMine) {
                numOfMines++;
            }
        }
    }
    return numOfMines;
}

vector<vector<int>> ReadBoardFile (const string filename, int boardWidth, int boardHeight) {
    vector<vector<int>> testBrdValues;
    string fileName = filename + ".brd";
    int testMineCount = 0;

    ifstream boardFile;
    boardFile.open(fileName);

    for (int i = 0; i < boardHeight; i++) {
        for (int j = 0; j < boardWidth; j++) {
            string vals;
            vals = boardFile.get();

            if (vals == "\n") {
                vals = boardFile.get();
            }
            if (vals == "1") {
                vector<int> val;
                val.push_back(j);
                val.push_back(i);

                testBrdValues.push_back(val);
                testMineCount++;
            }
        }
    }
    return testBrdValues;
}

int main() {
    vector<sf::Vector2i> testBrdValues;

    struct Mine {
        int x;
        int y;
    };

    sf::Sprite loseFace(TextureManager::GetTexture("face_lose"));
    bool gameLose = false;
    bool gameWin = false;

    Board board;
    board.ReadFile("config.cfg");

    sf::RenderWindow window(sf::VideoMode(board.windowWidth, board.windowHeight), "Minesweeper");

    sf::Sprite hiddenTile(TextureManager::GetTexture("tile_hidden"));
    sf::Sprite revealedTile(TextureManager::GetTexture("tile_revealed"));

    vector<vector<Tile*>> gameBoard(board.boardWidth);

    window.clear();

    for (int i = 0; i < board.boardWidth; i++) {
        gameBoard[i] = vector<Tile*>(board.boardHeight);
        for (int j = 0; j < board.boardHeight; j++) {
            Tile* aTile = new Tile("tile_hidden", i, j);
            gameBoard[i][j] = aTile;
        }
    }

    vector<Tile*> gameButtons;

    Tile* debugButton = new Tile("debug", (board.boardWidth + 550)/32, (board.boardHeight + 520)/32);
    gameButtons.push_back(debugButton);

    Tile* test1Button = new Tile("test_1", (board.boardWidth + 600)/32, (board.boardHeight + 520)/32);
    gameButtons.push_back(test1Button);

    Tile* test2Button = new Tile("test_2", (board.boardWidth + 660)/32, (board.boardHeight + 520)/32);
    gameButtons.push_back(test2Button);

    Tile* test3Button = new Tile("test_3", (board.boardWidth + 740)/32, (board.boardHeight + 520)/32);
    gameButtons.push_back(test3Button);

    Tile* face = new Tile("face_happy", ((board.boardWidth * 35)/2)/32, (board.boardHeight + 520)/32);
    gameButtons.push_back(face);

    int boardMineCount = board.mineCount;
    vector<Mine> mines;

    for (int i = 0; i <= board.mineCount; i++) {
            int mineRandomX = Random::Int(0, board.boardWidth - 1);
            int mineRandomY = Random::Int(0, board.boardHeight - 1);

                Mine mine;
                mine.x = mineRandomX * 32;
                mine.y = mineRandomY * 32;

//                cout << i << endl;
                mines.push_back(mine);
                gameBoard[mineRandomX][mineRandomY]->isMine();
        }

    for (int i = 0; i < board.boardWidth; i++) {
        for (int j = 0; j < board.boardHeight; j++) {
            if (CountMines(i, j, gameBoard) > 0) {
                gameBoard[i][j]->hasANum();
            }
            else if (CountMines(i, j, gameBoard) == 0) {
                gameBoard[i][j]->hasNoNum();
            }
        }
    }

//    Tile* counter = new Tile("digits", (board.boardWidth + 25)/32, (board.boardHeight + 520)/32);
    Digits * digits = new Digits();
    sf::Vector2i digitsPosition(32, 512);
    digits->setPosition(digitsPosition);

    int digit1 = 0;
    int digit2 = 5;
    int digit3 = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i coordinates = sf::Mouse::getPosition(window);

                if (gameButtons[1]->tileSprite.getGlobalBounds().contains(coordinates.x, coordinates.y)) {
                    for (int i = 0; i < board.boardWidth; i++) {
                        for (int j = 0; j < board.boardHeight; j++) {
                            gameBoard[i][j]->isNotAMine();
                            gameBoard[i][j]->notRevealed();
                            gameBoard[i][j]->notFlagged();
                            gameBoard[i][j]->SetTexture("tile_hidden");
                        }
                    }

                    if (gameLose || gameWin) {
                        gameLose = false;
                        gameWin = false;
                        board.ResetGame(gameBoard, gameButtons, gameLose, gameWin);
                    }


                    vector<vector<int>> testBoard1 = ReadBoardFile("testBoard1", board.boardWidth, board.boardHeight);

                    for (int k = 0; k < testBoard1.size(); k++) {
                        gameBoard[testBoard1[k][0]][testBoard1[k][1]]->isMine();
                    }

                    for (int i = 0; i < board.boardWidth; i++) {
                        for (int j = 0; j < board.boardHeight; j++) {
                            if (CountMines(i, j, gameBoard) > 0) {
                                gameBoard[i][j]->hasANum();
                            }
                            else if (CountMines(i, j, gameBoard) == 0) {
                                gameBoard[i][j]->hasNoNum();
                            }
                        }
                    }
                    boardMineCount = CountNumMines(board.boardWidth, board.boardHeight, gameBoard);
                }
                else if (gameButtons[2]->tileSprite.getGlobalBounds().contains(coordinates.x, coordinates.y)) {
                    for (int i = 0; i < board.boardWidth; i++) {
                        for (int j = 0; j < board.boardHeight; j++) {
                            gameBoard[i][j]->isNotAMine();
                            gameBoard[i][j]->notRevealed();
                            gameBoard[i][j]->notFlagged();
                            gameBoard[i][j]->SetTexture("tile_hidden");
                        }
                    }

                    if (gameLose || gameWin) {
                        gameLose = false;
                        gameWin = false;
                        board.ResetGame(gameBoard, gameButtons, gameLose, gameWin);
                    }

                    vector<vector<int>> testBoard2 = ReadBoardFile("testboard2", board.boardWidth, board.boardHeight);

                    for (int k = 0; k < testBoard2.size(); k++) {
                        gameBoard[testBoard2[k][0]][testBoard2[k][1]]->isMine();
                    }

                    for (int i = 0; i < board.boardWidth; i++) {
                        for (int j = 0; j < board.boardHeight; j++) {
                            if (CountMines(i, j, gameBoard) > 0) {
                                gameBoard[i][j]->hasANum();
                            }
                            else if (CountMines(i, j, gameBoard) == 0) {
                                gameBoard[i][j]->hasNoNum();
                            }
                        }
                    }
                    boardMineCount = CountNumMines(board.boardWidth, board.boardHeight, gameBoard);
                }

                else if (gameButtons[3]->tileSprite.getGlobalBounds().contains(coordinates.x, coordinates.y)) {
                    for (int i = 0; i < board.boardWidth; i++) {
                        for (int j = 0; j < board.boardHeight; j++) {
                            gameBoard[i][j]->isNotAMine();
                            gameBoard[i][j]->notRevealed();
                            gameBoard[i][j]->notFlagged();
                            gameBoard[i][j]->SetTexture("tile_hidden");
                        }
                    }

                    if (gameLose || gameWin) {
                        gameLose = false;
                        gameWin = false;
                        board.ResetGame(gameBoard, gameButtons, gameLose, gameWin);
                    }

                    vector<vector<int>> testBoard3 = ReadBoardFile("testboard3", board.boardWidth, board.boardHeight);

                    for (int k = 0; k < testBoard3.size(); k++) {
                        gameBoard[testBoard3[k][0]][testBoard3[k][1]]->isMine();
                    }

                    for (int i = 0; i < board.boardWidth; i++) {
                        for (int j = 0; j < board.boardHeight; j++) {
                            if (CountMines(i, j, gameBoard) > 0) {
                                gameBoard[i][j]->hasANum();
                            }
                            else if (CountMines(i, j, gameBoard) == 0) {
                                gameBoard[i][j]->hasNoNum();
                            }
                        }
                    }

                    boardMineCount = CountNumMines(board.boardWidth, board.boardHeight, gameBoard);
                }
                else if (gameButtons[0]->tileSprite.getGlobalBounds().contains(coordinates.x, coordinates.y)) {
                    if (!gameLose && !gameWin) {
                        gameButtons[0]->debugClick();
                    }
                }
                else if (gameButtons[4]->tileSprite.getGlobalBounds().contains(coordinates.x,
                                                                                 coordinates.y) /*&& gameLose*/) {
                    gameLose = false;
                    gameWin = false;
                    board.ResetGame(gameBoard, gameButtons, gameLose, gameWin);

                    mines.clear();
                    board.RandomizeMines(gameBoard);
                    boardMineCount = CountNumMines(board.boardWidth, board.boardHeight, gameBoard);
                }
            }
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i coordinates = sf::Mouse::getPosition(window);

                for (int i = 0; i < board.boardWidth; i++) {
                    for (int j = 0; j < board.boardHeight; j++) {
                        if (gameBoard[i][j]->tileSprite.getGlobalBounds().contains(coordinates.x, coordinates.y)) {
                            if (!gameLose && !gameWin) {
                                if (CountMines(i, j, gameBoard) != 0) {
                                    gameBoard[i][j]->revealingTiles(i, j, gameBoard, board.boardWidth,
                                                                    board.boardHeight,
                                                                    gameButtons, gameLose);
                                    if (gameBoard[i][j]->aMine && !gameBoard[i][j]->flagged) {
                                        gameLose = true;
                                    }
                                    cout << "revealing" << endl;
                                } else if (CountMines(i, j, gameBoard) == 0) {
                                    gameBoard[i][j]->recursiveReveal(i, j, gameBoard, board.boardWidth,
                                                                     board.boardHeight);
                                    cout << "recursive reveal" << endl;
                                }
                            }
                        }
                    }
                }
            }
        }

//        if (gameButtons[4]->isLoseFace) {
//            gameLose = true;
//        }

//        int boardMineCount = CountNumMines(board.boardWidth, board.boardHeight, gameBoard);

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Right) {
                sf::Vector2i coordinates = sf::Mouse::getPosition(window);

                for (int i = 0; i < board.boardWidth; i++) {
                    for (int j = 0; j < board.boardHeight; j++) {
                        if (gameBoard[i][j]->tileSprite.getGlobalBounds().contains(coordinates.x, coordinates.y)) {
                            if (!gameLose && !gameWin) {
                                gameBoard[i][j]->placeFlag(i, j, gameBoard, boardMineCount);
                            }
                        }
                    }
                }
            }
        }

//        for (int i = 0; i < board.boardWidth; i++) {
//            for (int j = 0; j <board.boardHeight; j++) {
//                if ((!gameBoard[i][j]->aMine && gameBoard[i][j]->revealed) && (gameBoard[i][j]->aMine && !gameBoard[i][j]->revealed)) {
//                   gameWingameWin = true;
//                }
//            }
//        }

            for (int i = 0; i < gameButtons.size(); i++) {
                window.draw(gameButtons[i]->tileSprite);
            }

            for (int i = 0; i < board.boardWidth; i++) {
                for (int j = 0; j < board.boardHeight; j++) {
                    window.draw(gameBoard[i][j]->tileSprite);

                    if ((gameBoard[i][j]->revealed && gameBoard[i][j]->aMine) || (gameButtons[0]->debugClicked && gameBoard[i][j]->aMine)) {
                        board.Draw(window, "mine", i * 32, j * 32);
                    }

                    if (gameBoard[i][j]->revealed && !gameBoard[i][j]->aMine) {
                        if (CountMines(i, j, gameBoard) > 0) {
                            string numFile = "number_" + to_string(CountMines(i, j, gameBoard));
                            board.Draw(window, numFile, i * 32, j * 32);
//                            gameBoard[i][j]->hasANum();
                        }
                    }

//                    if (gameBoard[i][j]->flagged) {
//                        board.Draw(window, "flag", i * 32, j * 32);
//                    }
                }
            }

            board.CountFlags(gameBoard);

            int numMines = 0;

            for (int i = 0; i < board.boardWidth; i++) {
                for (int j = 0; j < board.boardHeight; j++) {
                    if (gameBoard[i][j]->aMine) {
                        ++numMines;
                    }
                }
            }

            if (numMines - board.CountFlags(gameBoard) >= 0) {
                digit1 = (numMines - board.CountFlags(gameBoard)) / 100;
                digit2 = ((numMines - board.CountFlags(gameBoard)) / 10) % 10;
                digit3 = (numMines - board.CountFlags(gameBoard)) % 10;

                digits->setTexture1(digit1);
                digits->setTexture2(digit2);
                digits->setTexture3(digit3);
            }
            else if (numMines - board.CountFlags(gameBoard) < 0) {
                digit1 = 10;
                digit2 = (0 - (numMines - board.CountFlags(gameBoard)) / 10) % 10;
                digit3 = (0 - (numMines - board.CountFlags(gameBoard))) % 10;

                digits->setTexture1(digit1);
                digits->setTexture2(digit2);
                digits->setTexture3(digit3);
            }

//            cout << boardMineCount << endl;

            for (int i = 1; i <= 3; i++) {
                window.draw(digits->GetSprite(i));
            }

            if ((boardMineCount - board.CountFlags(gameBoard)) == 0) {
                cout << "checking game win" << endl;
                if (board.CheckGameWin(gameBoard, gameButtons, gameWin, gameLose,
                                       CountNumMines(board.boardWidth, board.boardHeight, gameBoard))) {
                    gameWin = true;
                } else if (!board.CheckGameWin(gameBoard, gameButtons, gameWin, gameLose,
                                               CountNumMines(board.boardWidth, board.boardHeight, gameBoard))) {
                    gameLose = true;
                }
            }

            window.display();
    }
}



