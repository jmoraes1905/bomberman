#include <memory>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "func.h"

int main() {
	ModelMapp M;
	ModelPersonagem P1;
	ControllerPersonagem C1;
    ViewerPersonagem V1;

    M.mapp_terrain();
	P1.set_personagem(M);

    bool rodando = true;
    
    while(rodando){
	    ViewerPersonagem V1(M,P1,C1,rodando);
	}
	return 0;
}
