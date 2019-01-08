all:
	g++ -std=c++11 -O3 -o game -I. -DGLM_ENABLE_EXPERIMENTAL \
            particle.cpp postprocessor.cpp shader.cpp sprite_renderer.cpp ball.cpp block.cpp game.cpp gamelevel.cpp gameobject.cpp main.cpp texture.cpp -lGLEW -lGL -lsfml-system \
            -lsfml-window -lsfml-audio
