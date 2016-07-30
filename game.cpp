#include "game.h"

Game::Game(int w_width, int w_height):
    // ini list
    // sfml
    window(sf::VideoMode(w_width, w_height), "#opengl#", sf::Style::Close, sf::ContextSettings(0, 0, 8, 3, 1)),

    var_for_GLEW(init_glew()),

    // variables
    shake_timer(0),
    isRunning(true),
    ball_init_speed(275, 300),
    w_width(w_width),
    w_height(w_height),

    // shaders
    shad_DEF("res/shad_DEF"),
    shad_PARTICLES("res/shad_PARTICLES"),
    shad_POSTPROCESS("res/shad_POSTPROCESS"),

    // sprite renderer
    SP_REND(&shad_DEF),

    // post processor
    POST_PROCESSOR(shad_POSTPROCESS, w_width, w_height),

    // textures
    textures0{Texture("res/back_IMAGE.png"), Texture("res/paddle.png"), Texture("res/ball.png"), Texture("res/smoke_particle.png"),
              Texture("res/block.png"), Texture("res/solid_block.png"), Texture("res/powerUp0.png"), Texture("res/powerUp1.png"),
              Texture("res/powerUp2.png"), Texture("res/powerUp3.png"), Texture("res/powerUp4.png"), Texture("res/powerUp5.png"),
              Texture("res/FIRE.png")},

    textures{&textures0[0], &textures0[1], &textures0[2], &textures0[3], &textures0[4], &textures0[5], &textures0[6], &textures0[7], &textures0[8],
    &textures0[9], &textures0[10], &textures0[11]},

    lvl_tex_Pointers{&textures0[5], &textures0[4]},

    // particle generator
    SMOKE(&shad_PARTICLES, textures[3], 300, glm::vec4(0, 1, 0, 1), glm::vec2(5, 5), glm::vec2(10, 10), 1, 2),

    // level
    LEVEL("res/level0", w_width, w_height/2.5, lvl_tex_Pointers, SP_REND),

    // game objects
    paddle(&SP_REND, textures[1], glm::vec2(w_width/2.0 - 100/2.0, w_height - 20), glm::vec2(100, 20)),

    ball(&SP_REND, textures[2], paddle.position + glm::vec2(paddle.size.x/2 - 20, -40), 20, ball_init_speed)
{
    // ##### ##### music and sounds set up
    main_theme_music.openFromFile("res/islandvibez.wav");
    main_theme_music.setLoop(true);
    main_theme_music.setVolume(5);
    main_theme_music.play();

    SOUND_buff_paddle.loadFromFile("res/SOUND_paddle.ogg");
    SOUND_paddle.setBuffer(SOUND_buff_paddle);
    SOUND_paddle.setVolume(10);

    SOUND_buff_block.loadFromFile("res/SOUND_block.ogg");
    SOUND_block.setBuffer(SOUND_buff_block);
    SOUND_block.setVolume(5);

    SOUND_buff_solid_block.loadFromFile("res/SOUND_solid_block.ogg");
    SOUND_solid_block.setBuffer(SOUND_buff_solid_block);
    SOUND_solid_block.setVolume(50);

    SOUND_buff_powerUp.loadFromFile("res/SOUND_powerUp.ogg");
    SOUND_powerUp.setBuffer(SOUND_buff_powerUp);
    SOUND_powerUp.setVolume(50);

    // ##### ##### shaders set up
    glm::mat4 projection = glm::ortho(0.f, static_cast<float>(w_width), static_cast<float>(w_height), 0.f, -1.f, 1.f);

    shad_DEF.bind();
    glUniformMatrix4fv(glGetUniformLocation(shad_DEF.program, "projection"), 1, GL_FALSE, &projection[0][0]);

    shad_PARTICLES.bind();
    glUniformMatrix4fv(glGetUniformLocation(shad_PARTICLES.program, "projection"), 1, GL_FALSE, &projection[0][0]);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);

    for(int i = 0; i < 40;  ++i)
    {
        parti_LVL.push_back(Particle_generator(&shad_PARTICLES, &textures0[12], 300, glm::vec4(1, 1, 1, 1), glm::vec2(8, 8), glm::vec2(20, 20), 2, 150));
    }
}

void Game::START()
{
    clock.restart();
    while(isRunning)
    {
        this->getInput();
        timer_REFRESH = clock.restart().asSeconds();
        timer += timer_REFRESH;

        this->update();
        this->draw();
    }
}

void Game::getInput()
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            isRunning = false;
        else if(event.type == sf::Event::KeyPressed)
            if(event.key.code == sf::Keyboard::Escape)
                isRunning = false;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        if(ball.position.x >= paddle.position.x)
        {
            paddle.position.x += -8;
            if(ball.isStuck)
                ball.position.x += -8;

            if(paddle.position.x < 0)
            {
                paddle.position.x += 8;
                if(ball.isStuck)
                    ball.position.x += 8;
            }
        }
        else
        {
            paddle.position.x += -8;
            if(ball.isStuck)
                ball.position.x += -8;

            if(paddle.position.x < 0)
                paddle.position.x += 8;

            if(ball.position.x < 0 && ball.isStuck)
                ball.position.x += 8;

        }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        if(ball.position.x + ball.size.x <= paddle.position.x + paddle.size.x)
        {

            paddle.position.x += 8;
            if(ball.isStuck)
                ball.position.x += 8;

            if(paddle.position.x > w_width - paddle.size.x)
            {
                paddle.position.x += -8;
                if(ball.isStuck)
                    ball.position.x += -8;
            }
        }
        else
        {
            paddle.position.x += 8;
            if(ball.isStuck)
                ball.position.x +=8;

            if(paddle.position.x > w_width - paddle.size.x)
                paddle.position.x += -8;

            if((ball.position.x > w_width - ball.size.x) && ball.isStuck)
                ball.position.x += -8;
        }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        ball.isStuck = false;
    }
}

void Game::update()
{
    // ##### ##### ball movement && colissions
    if(ball.isStuck)
    {
        //ball.position = paddle.position + glm::vec2(paddle.size.x/2 - ball.radius, -ball.radius*2);
        ball.rotation += timer_REFRESH;
    }
    else
    {
        ball.move(timer_REFRESH, w_width);

        // collisions
        if(ball.position.y + ball.radius >= w_height)
        {
            ball.reset(ball_init_speed);
            paddle.position = glm::vec2(w_width/2 - paddle.size.x/2, w_height - paddle.size.y);
            ball.position = paddle.position + glm::vec2(paddle.size.x/2 - ball.radius, -ball.radius*2);
            LEVEL.load("res/level0", w_width, w_height/2.5);

            for(std::vector<PowerUp>::iterator i = powerUps.begin(); i != powerUps.end(); ++i)
            {
                if(i->isActive && (i->type != "invert" && i->type != "edge_det"))
                    i->duration = 0;
            }
        }
        ball_paddle_collision(275);
        ball_level_collision();
    }

    // ##### ##### particles
    SMOKE.update(timer_REFRESH, ball);

    for(std::vector<Particle_generator>::iterator i = parti_LVL.begin(); i != parti_LVL.end(); ++i)
    {
        if(i->isActive)
        {
            i->life -= timer_REFRESH;
            if(i->life <= 0)
            {
                i->isActive = false;
                //                for(std::vector<Particle>::iterator k = i->particles.begin(); k != i->particles.end(); ++k) // dla ciaglego strumienia partycji
                //                {
                //                    k->life = -1;
                //                }
            }
            else
                i->update(timer_REFRESH);
        }
    }

    // ##### ##### powerUps
    update_powerUps(timer_REFRESH);
    paddle_powerUp_collision();

    // ##### ##### shake effect
    if(shake_timer > 0)
    {
        shake_timer -= timer_REFRESH;
        if(shake_timer <= 0)
            POST_PROCESSOR.shake = false;
    }
}

void Game::draw()
{
    POST_PROCESSOR.beginRender(); // start drawing to framebuffer

    // back image
    SP_REND.render(textures[0], glm::vec2(0, 0), glm::vec2(w_width, w_height), glm::vec3(1, 1, 1), 0);

    paddle.draw();
    LEVEL.draw();

    for(std::vector<PowerUp>::iterator i = powerUps.begin(); i != powerUps.end(); ++i)
    {
        if(!i->isDestroyed)
            i->draw();
    }

    SMOKE.draw();

    ball.draw();

    for(std::vector<Particle_generator>::iterator i = parti_LVL.begin(); i != parti_LVL.end(); ++i)
    {
        if(i->isActive)
            i->draw();
    }

    POST_PROCESSOR.endRender(); // end

    // draw to screen
    glClear(GL_COLOR_BUFFER_BIT);
    POST_PROCESSOR.renderEffect(timer);
    window.display();
}

bool Game::check_collision(GameObject& one, GameObject& two)
{
    bool collisionX = (one.position.x + one.size.x >= two.position.x) && (two.position.x + two.size.x >= one.position.x);

    bool collisionY = (one.position.y + one.size.y >= two.position.y) && (two.position.y + two.size.y >= one.position.y);

    return collisionX && collisionY;
}

Game::coll_package Game::check_collision_with_ball(GameObject& object)
{
    // ball properties
    glm::vec2 ball_center(ball.position + static_cast<float>(ball.radius));
    // obj properties
    glm::vec2 obj_sideLen(object.size.x/2, object.size.y/2);
    glm::vec2 obj_center(object.position.x + obj_sideLen.x, object.position.y + obj_sideLen.y);
    // calc
    glm::vec2 diff = ball_center - obj_center;
    glm::vec2 clamped = glm::clamp(diff, -obj_sideLen, obj_sideLen);
    glm::vec2 closest_point = obj_center + clamped;
    diff = closest_point - ball_center;

    if(glm::length(diff) <= ball.radius)
        return std::make_tuple(true, from_whichDir(diff), diff);
    else
        return std::make_tuple(false, UP, glm::vec2(0, 0));
}

Game::ball_dir Game::from_whichDir(const glm::vec2& vector)
{
    glm::vec2 directions[] =
    {
        glm::vec2(0, 1), // up
        glm::vec2(1, 0), // right
        glm::vec2(0, -1), // down
        glm::vec2(-1, 0) // left
    };

    float max = 0;
    int best_match = 0;

    for(int i = 0; i < 4; ++i)
    {
        float dot_product = glm::dot(glm::normalize(vector), directions[i]);
        if(dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return static_cast<ball_dir>(best_match);
}

void Game::ball_level_collision()
{
    bool flag_x = true, flag_y = true;
    for(std::vector<Block>::iterator i = LEVEL.blocks.begin(); i != LEVEL.blocks.end(); ++i)
    {
        if(!i->isDestroyed)
        {
            coll_package coll_info = check_collision_with_ball(*i);
            if(std::get<0>(coll_info))
            {
                if(!i->isSolid)
                {
                    SOUND_block.play();
                    i->isDestroyed = true;
                    spawn_powerUp(*i);

                    for(std::vector<Particle_generator>::iterator x = parti_LVL.begin(); x != parti_LVL.end(); ++x)
                    {
                        if(!x->isActive)
                        {
                            x->isActive = true;
                            x->life = 3;
                            x->obj = i->returnBLOCK();
                            x->fire = true;
                            break;
                        }
                    }
                }
                else
                {
                    SOUND_solid_block.play();
                    POST_PROCESSOR.shake = true;
                    shake_timer = 0.2; // 200 ms
                }

                ball_dir dir = std::get<1>(coll_info);
                glm::vec2 diff_vector= std::get<2>(coll_info);

                if(!(ball.passThrough && !i->isSolid))
                {
                    // changing ball velocity
                    if((dir == LEFT || dir == RIGHT) && flag_x)
                    {
                        flag_x = false;
                        ball.velocity.x = -ball.velocity.x;
                        // position correction
                        float penetration = ball.radius - std::abs(diff_vector.x);

                        if(dir == LEFT)
                            ball.position.x += penetration;
                        else
                            ball.position.x -= penetration;
                    }
                    else if(flag_y)
                    {
                        flag_y= false;
                        ball.velocity.y = -ball.velocity.y;
                        float penetration = ball.radius - std::abs(diff_vector.y);
                        if(dir == UP)
                            ball.position.y -= penetration;
                        else
                            ball.position += penetration;
                    }
                }
            }
        }
    }
}

void Game::ball_paddle_collision(double output_x_speed_power)
{
    coll_package coll_info = check_collision_with_ball(paddle);

    if(!ball.isStuck && std::get<0>(coll_info))
    {
        SOUND_paddle.play();

        float paddle_center = paddle.position.x + paddle.size.x/2;
        float distance_from_center = (ball.position.x + ball.radius) - paddle_center;
        float percentage = distance_from_center / (paddle.size.x/2);

        glm::vec2 old_vel = ball.velocity;
        ball.velocity.x = percentage * output_x_speed_power;
        ball.velocity.y = -1 * abs(ball.velocity.y);
        ball.velocity = glm::normalize(ball.velocity) * glm::length(old_vel);

        ball.position.y = paddle.position.y - ball.size.y;

        ball.isStuck = ball.isSticky;
    }
}

bool Game::should_spawn(int chance)
{
    // random number generator
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 10000);

    return (dist(mt) % chance == 0);
}

void Game::spawn_powerUp(GameObject& block)
{
    if(should_spawn(8))
    {
        powerUps.push_back(PowerUp(&SP_REND, textures[6], block.position, glm::vec2(60, 60), "invert", 5, glm::vec2(0, 150)));
    }
    if(should_spawn(8))
    {
        powerUps.push_back(PowerUp(&SP_REND, textures[7], block.position, glm::vec2(100, 100), "edge_det", 3, glm::vec2(0, 120)));
    }
    if(should_spawn(15))
    {
        powerUps.push_back(PowerUp(&SP_REND, textures[8], block.position, glm::vec2(80, 80), "pass_through", 8, glm::vec2(0, 100)));
    }
    if(should_spawn(15))
    {
        powerUps.push_back(PowerUp(&SP_REND, textures[9], block.position, glm::vec2(70, 70), "sticky", 10, glm::vec2(0, 150)));
    }
    if(should_spawn(15))
    {
        powerUps.push_back(PowerUp(&SP_REND, textures[10], block.position, glm::vec2(100, 100), "paddle_increase", 14, glm::vec2(0, 200)));
    }
    if(should_spawn(10))
    {
        powerUps.push_back(PowerUp(&SP_REND, textures[11], block.position, glm::vec2(90, 90), "speed_up", 6, glm::vec2(0, 250)));
    }
}

void Game::paddle_powerUp_collision()
{
    for(std::vector<PowerUp>::iterator i = powerUps.begin(); i != powerUps.end(); ++i)
    {
        if(!i->isDestroyed)
        {
            if(i->position.y >= w_height - i->size.y)
            {
                i->isDestroyed = true;
            }
            else if(check_collision(paddle, *i))
            {
                activate_powerUp(*i);
                i->isDestroyed = true;
                i->isActive = true;
                SOUND_powerUp.setPlayingOffset(sf::seconds(0));
                SOUND_powerUp.play();
            }
        }
    }
}

void Game::activate_powerUp(PowerUp& powerUp)
{
    if(powerUp.type == "invert")
    {
        if(POST_PROCESSOR.edge_det == false)
        {
            POST_PROCESSOR.invert = true;
            main_theme_music.setPitch(10);
        }
    }
    else if(powerUp.type == "edge_det")
    {
        if(POST_PROCESSOR.invert == false)
        {
            POST_PROCESSOR.edge_det = true;
            main_theme_music.setPitch(2);
        }
    }
    else if(powerUp.type == "pass_through")
    {
        ball.passThrough = true;
        ball.color = glm::vec3(1, 0, 0);
    }
    else if(powerUp.type == "sticky")
    {
        ball.isSticky = true;
        paddle.color = glm::vec3(1, 0, 0);
    }
    else if(powerUp.type == "paddle_increase")
    {
        paddle.size.x += 38;
        paddle.position.x -= 19;
    }
    else if(powerUp.type == "speed_up")
    {
        ball.velocity *= 1.5;
        SMOKE.color = glm::vec4(0, 0, 0, 1);
        SMOKE.full_life = 2;
        SMOKE.size = glm::vec2(6, 6);
    }
}

void Game::update_powerUps(float time)
{
    for(std::vector<PowerUp>::iterator i = powerUps.begin(); i != powerUps.end(); ++i)
    {
        i->position += i->velocity * time;

        if(i->isActive)
        {
            i->duration -= time;
            if(i->duration <= 0)
            {
                i->isActive = false;
                if(i->type == "invert")
                {
                    if(!is_other_powerUp_active("invert"))
                    {
                        POST_PROCESSOR.invert = false;
                        main_theme_music.setPitch(1);
                    }
                }
                else if(i->type == "edge_det")
                {
                    if(!is_other_powerUp_active("edge_det"))
                    {
                        POST_PROCESSOR.edge_det = false;
                        main_theme_music.setPitch(1);
                    }
                }
                else if(i->type == "pass_through")
                {
                    if(!is_other_powerUp_active("pass_through"))
                    {
                        ball.passThrough = false;
                        ball.color = glm::vec3(1, 1, 1);
                    }
                }
                else if(i->type == "sticky")
                {
                    if(!is_other_powerUp_active("sticky"))
                    {
                        ball.isSticky = false;
                        paddle.color = glm::vec3(1, 1, 1);
                    }
                }
                else if(i->type == "paddle_increase")
                {
                    paddle.size.x -= 38;
                    paddle.position.x += 19;
                }
                else if(i->type == "speed_up")
                {
                    if(ball.velocity != ball_init_speed)
                        ball.velocity *= 1/1.5;

                    SMOKE.color = glm::vec4(0, 1, 0, 1);
                    SMOKE.full_life = 1;
                    SMOKE.size = glm::vec2(5, 5);
                }
            }
        }
    }
    powerUps.erase(std::remove_if(powerUps.begin(), powerUps.end(),
                                  [](PowerUp& powerUp){return powerUp.isDestroyed && !powerUp.isActive;}),
            powerUps.end());
}

bool Game::is_other_powerUp_active(const std::string type)
{
    for(std::vector<PowerUp>::iterator i = powerUps.begin(); i != powerUps.end(); ++i)
    {
        if(i->isActive)
            if(i->type == type)
                return true;
    }
    return false;
}
