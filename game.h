#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <GL/glew.h>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <glm/gtx/transform.hpp>
#include <tuple>
// my headers
#include <shader.h>
#include <texture.h>
#include <sprite_renderer.h>
#include <postprocessor.h>
#include <particle.h>
#include <gamelevel.h>
#include <ball.h>
#include <gameobject.h>
#include <powerup.h>

class Game
{
public:

    Game(int w_width, int w_height);

    void START();

private:

    enum ball_dir
    {
        UP,
        RIGHT,
        DOWN,
        LEFT
    };

    typedef std::tuple<bool, ball_dir, glm::vec2> coll_package;


    /*In other words, are members guaranteed to be initialized by order of declaration and destroyed in reverse order?

    Yes to both.*/


    // SFML members
    sf::Window window;
    sf::Music main_theme_music;
    sf::SoundBuffer SOUND_buff_paddle, SOUND_buff_block, SOUND_buff_solid_block, SOUND_buff_powerUp;
    sf::Sound SOUND_paddle, SOUND_block, SOUND_solid_block, SOUND_powerUp;
    sf::Clock clock;

    // game important variables
    bool var_for_GLEW;


    float timer_REFRESH; // frame time
    float timer; // never restarts
    float shake_timer;
    bool isRunning;
    glm::vec2 ball_init_speed;

    int w_width, w_height;

    // my objects
    Shader shad_DEF, shad_PARTICLES, shad_POSTPROCESS;

    Sprite_renderer SP_REND;

    PostProcessor POST_PROCESSOR;

    std::vector<Texture> textures0;

    std::vector<Texture*> textures;

    std::vector<Texture*> lvl_tex_Pointers;

    Particle_generator SMOKE;

    std::vector<Particle_generator> parti_LVL;

    GameLevel LEVEL;

    GameObject paddle;

    Ball ball;

    std::vector<PowerUp> powerUps;

    // functions

    void getInput();

    void update();

    void draw();

    // collision functions

    bool check_collision(GameObject& one, GameObject& two);

    coll_package check_collision_with_ball(GameObject& object);

    ball_dir from_whichDir(const glm::vec2& vector);

    void ball_level_collision();

    void ball_paddle_collision(double output_x_speed_power);

    bool init_glew() // fires in initialization list
    {
        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if(GLEW_OK != err)
            std::cout << "could not initialize glew\n";
        return true;
    }

    void spawn_powerUp(GameObject& block);

    void update_powerUps(float time);

    bool should_spawn(int chance);

    void activate_powerUp(PowerUp& powerUp);

    bool is_other_powerUp_active(const std::string type);

    void paddle_powerUp_collision();
};

#endif // GAME_H
