#include "gamelevel.h"
#include <iostream>
#include <fstream>
#include <sstream>

GameLevel::GameLevel(const std::string fileName, int lvl_width, int lvl_height, std::vector<Texture*>& textures,
                     Sprite_renderer& spRend):
    textures(textures),
    spRend(spRend)
{
    load(fileName, lvl_width, lvl_height);
}

void GameLevel::draw()
{
    for (std::vector<Block>::iterator i = blocks.begin(); i != blocks.end(); ++i)
        if(!i->isDestroyed)
            i->draw();

}

void GameLevel::load(const std::string fileName, int lvl_width, int lvl_height)
{
    blocks.clear();

    std::vector<std::vector<int>> tileData;
    std::ifstream file(fileName);
    std::string line;

    if(file)
    {
        while(std::getline(file, line))
        {
            std::stringstream sstream(line);
            std::vector<int> row;
            int tileCode;
            while(sstream >> tileCode)
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if(tileData.size() > 0)
            process_data(tileData, lvl_width, lvl_height);
        else
            std::cout << "no data to process" << std::endl;
    }
    else
        std::cout << "could not open " << fileName << std::endl;
}

void GameLevel::process_data(const std::vector<std::vector<int>>& data, int lvl_width, int lvl_height)
{
    int Y_tiles = data.size();
    int X_tiles = data[0].size();
    float tile_width = static_cast<float>(lvl_width)/X_tiles,
            tile_height = static_cast<float>(lvl_height)/Y_tiles;

    for(int y = 0; y < Y_tiles; y++)
    {
        for(int x = 0; x < X_tiles; x++)
        {
            if(data[y][x] == 1)
            {
                glm::vec2 position(x*tile_width, y*tile_height);
                glm::vec2 size(tile_width, tile_height);

                Block block(&spRend, textures[0], position, size, true, glm::vec3(1, 1, 1));
                blocks.push_back(block);
            }
            else if(data[y][x] > 1)
            {
                glm::vec3 color;

                if(data[y][x] == 2)
                    color = glm::vec3(0.8314, 0.6431, 0.1801);
                else if(data[y][x] == 3)
                    color = glm::vec3(0.8784, 0.3294, 0.9412);
                else if(data[y][x] == 4)
                    color = glm::vec3(0, 0.1882, 0);
                else if(data[y][x] == 5)
                    color = glm::vec3(0.9882, 0.141, 0.141);

                glm::vec2 position(x*tile_width, y*tile_height);
                glm::vec2 size(tile_width, tile_height);

                Block block(&spRend, textures[1], position, size, false, color);
                blocks.push_back(block);
            }
        }
    }
}

bool GameLevel::isCompleted()
{
    for (std::vector<Block>::iterator i = blocks.begin(); i != blocks.end(); ++i)
        if(!i->isDestroyed && !i->isSolid)
            return false;
    return true;
}
