//
// Created by Christina Nguyen on 11/26/22.
//
#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

using std::unordered_map;
using std::string;

class TextureManager {
    static unordered_map<string, sf::Texture> textures;

public:
    static void LoadTexture(string textureName);
    static sf::Texture& GetTexture(string textureName);
    static void Clear();
};

#ifndef PROJECT3MINESWEEPER_TEXTUREMANAGER_H
#define PROJECT3MINESWEEPER_TEXTUREMANAGER_H

#endif //PROJECT3MINESWEEPER_TEXTUREMANAGER_H
