#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include <vector>
#include <block.h>
#include <string>
#include <texture.h>
#include <sprite_renderer.h>

// 0 -> empty space, 1 -> solid, <2, 5> -> not solid, diffrent colors
// every row and column must have the same length
// texture 0 for solid blocks, texture 1 for other
class GameLevel
{

public:

    GameLevel(const std::string fileName, int lvl_width, int lvl_height, std::vector<Texture*>& textures,
              Sprite_renderer& spRend);

    void draw();

    bool isCompleted();

    void load(const std::string fileName, int lvl_width, int lvl_height);

    std::vector<Block> blocks;

private:

    std::vector<Texture*>& textures;

    Sprite_renderer& spRend;

    void process_data(const std::vector<std::vector<int>> &data, int lvl_width, int lvl_height);
};

#endif // GAMELEVEL_H
