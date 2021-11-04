
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <memory>
//#include "func.h"
// g++ -oview viewer.cpp -lSDL2 -lSDL2_image
#include "json.hpp"
#include <fstream>
#include <vector>

using nlohmann::json;


const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 512;
const int MAX_I = 16;
const int MAX_J = 16;

const int SECOES_X = SCREEN_WIDTH/MAX_I;
const int SECOES_Y = SCREEN_HEIGHT/MAX_J;

class ModelMapp{
	private:


	public:
		int terreno[16][16];
		void mapp_terrain();
		void forbbid_terrain();
		void build_blocks();
};


void ModelMapp::mapp_terrain(){

	int i,j;

	for(i=0;i<MAX_I;i++){
		for(j=0;j<MAX_J;j++)
			terreno[i][j] = 1; //1 codifica posição habilitada/disponivel
	}

}

void ModelMapp::forbbid_terrain(){

	int k;
	for(k=0;k<MAX_I;k++){
		terreno[0][k] = -1;
		terreno[MAX_I-1][k] = -1;
		terreno[k][0] = -1;
		terreno[k][MAX_J-1] = -1;
		}			

}

void ModelMapp::build_blocks(){

	int k,l;
	for(k=5;k<8;k++){
		for(l=5;l<8;l++)
			terreno[k][l] = 2;
			}
}
	

class ModelPersonagem{
	private:

	public: 
		int posicao[2];
		ModelPersonagem();
		void set_personagem(ModelMapp &M, int xini, int yini);// inicializa  personagem no mapa
};


ModelPersonagem::ModelPersonagem(){

	posicao[0] = 1;
	posicao[1] = 1;
}
void ModelPersonagem::set_personagem(ModelMapp &M, int xini, int yini){
	
	if(xini>MAX_I-1)
    		xini = MAX_I-1;
  
  	if(yini>MAX_J-1)
    		yini = MAX_J-1;        
	
	
	M.terreno[xini][yini] = 0;
	posicao[0] = xini;
	posicao[1] = yini;
}


class ControllerPersonagem{
	
	private:

	public:
		int posicao[2];
		void move(ModelMapp &M, ModelPersonagem &P, int x, int y);
		void attack(ModelMapp &M, ModelPersonagem &P, int target);
};

void ControllerPersonagem::move(ModelMapp &M, ModelPersonagem &P, int x, int y){
	
	if(x>0) x = 1;
	else if(x<0) x = -1;
	
	if(y>0) y = 1;
	else if (y<0) y = -1;
	
	if(M.terreno[P.posicao[0]+x][P.posicao[1]+y]==1){
		M.terreno[P.posicao[0]][P.posicao[1]] = 1; //Desocupa posicao antiga
	
		P.posicao[0] = P.posicao[0] +x;
		P.posicao[1] = P.posicao[1] +y;
	
		M.terreno[P.posicao[0]][P.posicao[1]] = 0; // Ocupa a nova posicao
	
	}
	
}

void ControllerPersonagem::attack(ModelMapp &M, ModelPersonagem &P, int target){

		switch(target){
		
			case(1):
				if(M.terreno[P.posicao[0]+1][P.posicao[1]]==2)
					 M.terreno[P.posicao[0]+1][P.posicao[1]] = 1;
			
			case(2):
				if(M.terreno[P.posicao[0]-1][P.posicao[1]]==2)
					 M.terreno[P.posicao[0]-1][P.posicao[1]] = 1;
					 
			case(3):
				if(M.terreno[P.posicao[0]][P.posicao[1]+1]==2)
					 M.terreno[P.posicao[0]][P.posicao[1]+1] = 1;
					 
			case(4):
				if(M.terreno[P.posicao[0]][P.posicao[1]-1]==2)
					 M.terreno[P.posicao[0]][P.posicao[1]-1] = 1;
			}		 
				
}

class ViewerPersonagem{

	private:
		bool onscreen;
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;
		
	public:
		ViewerPersonagem(ModelMapp &M, ModelPersonagem &P, ControllerPersonagem &C);
		
	};
	

ViewerPersonagem::ViewerPersonagem(ModelMapp &M, ModelPersonagem &P, ControllerPersonagem &C){

	// Inicializando o subsistema de video do SDL

  	if ( SDL_Init (SDL_INIT_VIDEO) < 0 ) {
    		std::cout << SDL_GetError();
    		SDL_Quit();
  	}

  // Criando uma janela
 // SDL_Window* window = nullptr;

  	window = SDL_CreateWindow("BombermanRPG v0.01",
      		SDL_WINDOWPOS_UNDEFINED,
      		SDL_WINDOWPOS_UNDEFINED,
      		SCREEN_WIDTH,
      		SCREEN_HEIGHT,
      		SDL_WINDOW_SHOWN);
  
  // Caso a janela não tiver sido setada corretamente
  	if (window == nullptr) {
    		std::cout << SDL_GetError();
    		SDL_Quit();
  	}

  // Inicializando o renderizador
  	renderer = SDL_CreateRenderer(
      		window, -1,
     	 	SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  
  	if (renderer == nullptr) { // Em caso de erro...
    		SDL_DestroyWindow(window);
    		std::cout << SDL_GetError();
    		SDL_Quit();
  	}
	

	SDL_Texture *personagem = IMG_LoadTexture(renderer, "./bomberman.png");
	SDL_Rect target;
  	target.x = P.posicao[0]*SECOES_X;
  	target.y = P.posicao[1]*SECOES_Y;
  	SDL_QueryTexture(personagem, nullptr, nullptr, &target.w, &target.h);
  	SDL_Texture *tabuleiro = IMG_LoadTexture(renderer, "./tabuleiro.png");
  	
  	SDL_RenderClear(renderer);
  	SDL_RenderCopy(renderer, personagem, nullptr, &target);
    	SDL_RenderPresent(renderer);
    	
    	// Controlador:
 	onscreen = true;
 	bool ready = false;
	SDL_Event evento; // eventos discretos
  	const Uint8* state = SDL_GetKeyboardState(nullptr); // estado do teclado
  	
	SDL_PumpEvents();// atualiza estado do teclado
	
	while(onscreen){
	
		if (state[SDL_SCANCODE_LEFT]){
			if(ready) C.attack(M,P,1);	
    			C.move(M,P,-1,0);  
    			std::cout << P.posicao[0] << " " << P.posicao[1] << "\n"; 
    			target.x = P.posicao[0]*SECOES_X;
    			target.y = P.posicao[1]*SECOES_Y;
    		        
    		}
    	
   		if (state[SDL_SCANCODE_RIGHT]){
     			C.move(M,P,1,0); 
     			std::cout << P.posicao[0] << " " << P.posicao[1] << "\n";
     			target.x = P.posicao[0]*SECOES_X;
    			target.y = P.posicao[1]*SECOES_Y;
     		}
     	
    		if (state[SDL_SCANCODE_UP]){ 
    			C.move(M,P,0,-1); 
    			std::cout << P.posicao[0] << " " << P.posicao[1] << "\n";
    			target.x = P.posicao[0]*SECOES_X;
    			target.y = P.posicao[1]*SECOES_Y;
    		}
    	
    		if (state[SDL_SCANCODE_DOWN]){
    			C.move(M,P,0,1);
    			std::cout << P.posicao[0] << " " << P.posicao[1] << "\n";
    			target.x = P.posicao[0]*SECOES_X;
    			target.y = P.posicao[1]*SECOES_Y;
		}
		
		if(state[SDL_SCANCODE_RETURN]){
			ready = true;
			std::cout << ready;
			
		}
		while (SDL_PollEvent(&evento)) {
      			if (evento.type == SDL_QUIT) {
        			onscreen = false;
      			}
      		}
      // Exemplos de outros eventos.
      // Que outros eventos poderiamos ter e que sao uteis?
      //if (evento.type == SDL_KEYDOWN) {
      //}
      //if (evento.type == SDL_MOUSEBUTTONDOWN) {
      //}   

    // Desenhar a cena
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, tabuleiro, nullptr, nullptr);
    SDL_RenderCopy(renderer, personagem, nullptr, &target);
    SDL_RenderPresent(renderer);

    // Delay para diminuir o framerate
    SDL_Delay(10);
  }

  SDL_DestroyTexture(personagem);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
 
 }
 
int main() {
	
	ModelMapp M;
	ModelPersonagem P1;
	ControllerPersonagem C1;
	M.mapp_terrain();
	M.forbbid_terrain();
	M.build_blocks();
	json j;
	
	char save, load;
	
	std::cin >> load;
	
	if(load =='l'){
		std::ifstream f1;
		f1.open("Exercicio.json");
  		if( f1.is_open()){
  			f1>>j;
  			f1.close();
			P1.set_personagem(M,j["Estado"]["personagem"]["x"],j["Estado"]["personagem"]["y"]);
			}
		}
  
	ViewerPersonagem V1(M,P1,C1);
	
	std::cin >> save;
	
	
	j["Estado"]["terreno"] = M.terreno;
	j["Estado"]["personagem"]["x"] = P1.posicao[0];
	j["Estado"]["personagem"]["y"] = P1.posicao[1];
		 
  	std::ofstream f;
  	f.open("Exercicio.json"); 
  	f<<j;
 	f.close();
  			
   
  return 0;
}
	
