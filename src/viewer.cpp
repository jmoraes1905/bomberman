#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <memory>
//#include "func.h"
// g++ -oview viewer.cpp -lSDL2 -lSDL2_image

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

class ModelPersonagem{
	private:

	public: 
		int posicao[2];
		void set_personagem(ModelMapp &M, int xini, int yini);// inicializa  personagem no mapa
};

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
};

void ControllerPersonagem::move(ModelMapp &M, ModelPersonagem &P, int x, int y){
	
	
	if(M.terreno[P.posicao[0]+x][P.posicao[1]+y]!=-1){
		M.terreno[P.posicao[0]][P.posicao[1]] = 1; //Desocupa posicao antiga
	
		P.posicao[0] = P.posicao[0] +x;
		P.posicao[1] = P.posicao[1] +y;
	
		if(P.posicao[0] >MAX_I -1)
                	P.posicao[0]  = MAX_I -1;
                
        	else if(P.posicao[0] < 0)
        		P.posicao[0] = 0;
        	
        	if(P.posicao[1]>MAX_J -1)
        		P.posicao[1] = MAX_J -1;    
        	    
		else if(P.posicao[1]<0)
			P.posicao[1] =0;
	
		M.terreno[P.posicao[0]][P.posicao[1]] = 0; // Ocupa a nova posicao
	
	}
	
}

class ViewerMapp{

	private:
		
		
	public:
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;
		ViewerMapp(ModelMapp &M);
		//~ViewerMapp();
	
	};
	
	
ViewerMapp::ViewerMapp(ModelMapp &M){

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
 
 SDL_Texture *tabuleiro = IMG_LoadTexture(renderer, "./tabuleiro.png");

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, tabuleiro, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}


/*ViewerMapp::~ViewerMapp(){
 
 SDL_RenderClear(renderer);
 SDL_DestroyRenderer(renderer);
 SDL_DestroyWindow(window);
 SDL_Quit();	

}*/

class ViewerPersonagem{

	private:

	public:
		ViewerPersonagem(ModelPersonagem &P, ViewerMapp &V);
		void destroypersonagem(ViewerPersonagem &P, ViewerMapp &V);
	};
	

ViewerPersonagem::ViewerPersonagem(ModelPersonagem &P, ViewerMapp &V){

	SDL_Texture *personagem = IMG_LoadTexture(V.renderer, "./bomberman.png");
	SDL_Rect target;
  	target.x = P.posicao[0]*SECOES_X;
  	target.y = P.posicao[1]*SECOES_Y;
  	SDL_QueryTexture(personagem, nullptr, nullptr, &target.w, &target.h);
  	
  	//SDL_RenderClear(V.renderer);
  	SDL_RenderCopy(V.renderer, personagem, nullptr, &target);
    	SDL_RenderPresent(V.renderer);
    	
    	//std::cout << "Eu estive aqui" << std::endl;
 
 }
 
void ViewerPersonagem::destroypersonagem(ViewerPersonagem &P, ViewerMapp &V){

 //SDL_DestroyTexture(P.personagem);
 //SDL_DestroyTexture(tabuleiro);
 SDL_RenderClear(V.renderer);
 SDL_DestroyRenderer(V.renderer);
 SDL_DestroyWindow(V.window);
 SDL_Quit();		
	 
 }
 

class TecladoPersonagem{

	private:
	
	public:
		void readkeys(ModelMapp &M, ModelPersonagem &P, ControllerPersonagem &C);
		
};

void TecladoPersonagem::readkeys(ModelMapp &M, ModelPersonagem &P, ControllerPersonagem &C){
	
	//std::cout<< "Eu cheguei!"<< std::endl;
	SDL_Event evento; // eventos discretos
  	const Uint8* state = SDL_GetKeyboardState(nullptr); // estado do teclado
  	
	SDL_PumpEvents();// atualiza estado do teclado
	
	
	if (state[SDL_SCANCODE_LEFT]){
    		C.move(M,P,-1,0);  
    		std::cout << P.posicao[0] << " " << P.posicao[1] << "\n";         
    	}
    	
   	if (state[SDL_SCANCODE_RIGHT]){
     		C.move(M,P,1,0); 
     		std::cout << P.posicao[0] << " " << P.posicao[1] << "\n";
     	}
     	
    	if (state[SDL_SCANCODE_UP]){ 
    		C.move(M,P,0,-1); 
    		std::cout << P.posicao[0] << " " << P.posicao[1] << "\n";
    	}
    	
    	if (state[SDL_SCANCODE_DOWN]){
    		C.move(M,P,0,1);
    		std::cout << P.posicao[0] << " " << P.posicao[1] << "\n";
	}
   
}		      
		      

int main() {
	
	ModelMapp M;
	ModelPersonagem P1;
	ControllerPersonagem C1;
	M.mapp_terrain();
	M.forbbid_terrain();
	TecladoPersonagem T1;
	
	ViewerMapp W(M);
	P1.set_personagem(M,7,7);
	ViewerPersonagem V1(P1,W);
	
	T1.readkeys(M,P1,C1);
	
	
		
		
   
  return 0;
}
