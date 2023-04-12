//
// Created by Christina Nguyen on 12/7/22.
//
#pragma once
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

using namespace std;

class Digits {
    sf::Sprite sprite1;
    sf::Sprite sprite2;
    sf::Sprite sprite3;

    int x;
    int y;

public:
    Digits();
    void setTexture1(int num);
    void setTexture2(int num);
    void setTexture3(int num);
    void setPosition(sf::Vector2i position);
    const sf::Sprite& GetSprite(int spriteNum) const;
};

Digits::Digits() {
    sprite1.setTexture(TextureManager::GetTexture("digits"));
    sprite1.setTextureRect(sf::IntRect(21 * 0, 0, 21, 32));

    sprite2.setTexture(TextureManager::GetTexture("digits"));
    sprite2.setTextureRect(sf::IntRect(21 * 5, 0, 21, 32));

    sprite3.setTexture(TextureManager::GetTexture("digits"));
    sprite3.setTextureRect(sf::IntRect(21 * 0, 0, 21, 32));

    sf::Vector2i position(0, 512);
    setPosition(position);
}

void Digits::setTexture1(int num) {
    sprite1.setTexture(TextureManager::GetTexture("digits"));
    sprite1.setTextureRect(sf::IntRect(21 * num, 0, 21, 32));
}

void Digits::setTexture2(int num) {
    sprite2.setTexture(TextureManager::GetTexture("digits"));
    sprite2.setTextureRect(sf::IntRect(21 * num, 0, 21, 32));
}

void Digits::setTexture3(int num) {
    sprite3.setTexture(TextureManager::GetTexture("digits"));
    sprite3.setTextureRect(sf::IntRect(21 * num, 0, 21, 32));
}

void Digits::setPosition(sf::Vector2i position) {
    x = position.x;
    y = position.y;

    sf::Vector2f position1((float)x, (float)y);
    sprite1.setPosition(position1);

    sf::Vector2f position2((float)x + 21, (float)y);
    sprite2.setPosition(position2);

    sf::Vector2f position3((float)x + 45, (float)y);
    sprite3.setPosition(position3);
}

const sf::Sprite& Digits::GetSprite(int spriteNum) const {
    vector<sf::Sprite> digitsSprites;
    switch (spriteNum) {
        case 1:
            return sprite1;
        case 2:
            return sprite2;
        case 3:
            return sprite3;
        default:
            return sprite1;
    }
}

#ifndef PROJECT3MINESWEEPERNEW2_DIGIT_H
#define PROJECT3MINESWEEPERNEW2_DIGIT_H

#endif //PROJECT3MINESWEEPERNEW2_DIGIT_H
