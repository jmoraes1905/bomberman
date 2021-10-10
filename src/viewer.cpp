#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <memory>
//#include "func.h"
// g++ -osemi semiprt.cpp -lSDL2 -lSDL2_image

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
		void viewmapp(ModelMapp &M);
		void destroymapp(ViewerMapp &V)	;
	
	};
	
	
void ViewerMapp::viewmapp(ModelMapp &M){

// Inicializando o subsistema de video do SDL

  if ( SDL_Init (SDL_INIT_VIDEO) < 0 ) {
    std::cout << SDL_GetError();
    SDL_Quit();
  }

  // Criando uma janela
  SDL_Window* window = nullptr;

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
  SDL_Renderer* renderer = SDL_CreateRenderer(
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


void ViewerMapp::destroymapp(ViewerMapp &V){

SDL_Window* window = nullptr;

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
  SDL_Renderer* renderer = SDL_CreateRenderer(
      window, -1,
      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  
  if (renderer == nullptr) { // Em caso de erro...
    SDL_DestroyWindow(window);
    std::cout << SDL_GetError();
    SDL_Quit();
  }

  // fundo
  SDL_Texture *tabuleiro = IMG_LoadTexture(renderer, "./tabuleiro.png");

 	SDL_DestroyRenderer(renderer);
 	SDL_DestroyWindow(window);
 	SDL_Quit();	

}

class ViewerPersonagem{
  		private:

  		public:
    			void viewpersonagem(ModelMapp &M, ModelPersonagem &P, ControllerPersonagem &C);
    			void destroypersonagem(ViewerPersonagem &V);
};


void ViewerPersonagem::viewpersonagem(ModelMapp &M, ModelPersonagem &P, ControllerPersonagem &C){
  // Inicializando o subsistema de video do SDL

  if ( SDL_Init (SDL_INIT_VIDEO) < 0 ) {
    std::cout << SDL_GetError();
    SDL_Quit();
  }

  // Criando uma janela
  SDL_Window* window = nullptr;

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
  SDL_Renderer* renderer = SDL_CreateRenderer(
      window, -1,
      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  
  if (renderer == nullptr) { // Em caso de erro...
    SDL_DestroyWindow(window);
    std::cout << SDL_GetError();
    SDL_Quit();
  }

  
  // Carregando texturas
  // personagem
 // SDL_Texture *personagem = IMG_LoadTexture(renderer, "/Documentos/EA872/Tarefas_EA872/bomberman/bomberman.png");
  // fundo
  //SDL_Texture *tabuleiro = IMG_LoadTexture(renderer, "/Documentos/EA872/Tarefas_EA872/bomberman/tabuleiro.png");
  
  SDL_Texture *personagem = IMG_LoadTexture(renderer, "./bomberman.png");
  // fundo
  SDL_Texture *tabuleiro = IMG_LoadTexture(renderer, "./tabuleiro.png");


  // Quadrado onde a textura sera desenhada
  SDL_Rect target;
  target.x = P.posicao[0]*SECOES_X;
  target.y = P.posicao[1]*SECOES_Y;
  SDL_QueryTexture(personagem, nullptr, nullptr, &target.w, &target.h);

  // Controlador:
  //bool rodando = true;

  // Variaveis para verificar eventos
  SDL_Event evento; // eventos discretos
  const Uint8* state = SDL_GetKeyboardState(nullptr); // estado do teclado

/*  while(rodando){
    // Polling de eventos
    SDL_PumpEvents(); // atualiza estado do teclado
        
    if (state[SDL_SCANCODE_LEFT]){
    	C.move(M,P,-1,0);                     // altera mapa e posicao
    	target.x = (P.posicao[0])*SECOES_X;  // atualiza viewer com a nova posicao
    	}
    if (state[SDL_SCANCODE_RIGHT]){
     	C.move(M,P,1,0); 
     	target.x = (P.posicao[0])*SECOES_X;
     	}
    if (state[SDL_SCANCODE_UP]){ 
    	C.move(M,P,0,-1); 
    	target.y = (P.posicao[1])*SECOES_Y;
    	}
    if (state[SDL_SCANCODE_DOWN]){
    	C.move(M,P,0,1);  
    	target.y = (P.posicao[1])*SECOES_Y;
	}
    while (SDL_PollEvent(&evento)) {
      if (evento.type == SDL_QUIT) {
        rodando = false;
		      }
      // Exemplos de outros eventos.
      // Que outros eventos poderiamos ter e que sao uteis?
      //if (evento.type == SDL_KEYDOWN) {
      //}
      //if (evento.type == SDL_MOUSEBUTTONDOWN) {
      //}
		    }
*/
    // Desenhar a cena
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, tabuleiro, nullptr, nullptr);
    SDL_RenderCopy(renderer, personagem, nullptr, &target);
    SDL_RenderPresent(renderer);

    // Delay para diminuir o framerate
    SDL_Delay(50);
  	
}
void ViewerPersonagem::destroypersonagem(ViewerPersonagem &V){
	
  SDL_Window* window = nullptr;

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
  SDL_Renderer* renderer = SDL_CreateRenderer(
      window, -1,
      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  
  if (renderer == nullptr) { // Em caso de erro...
    SDL_DestroyWindow(window);
    std::cout << SDL_GetError();
    SDL_Quit();
  }

  SDL_Texture *personagem = IMG_LoadTexture(renderer, "./bomberman.png");
  // fundo
  //SDL_Texture *tabuleiro = IMG_LoadTexture(renderer, "./tabuleiro.png");

 	SDL_DestroyTexture(personagem);
 	//SDL_DestroyRenderer(renderer);
 	SDL_DestroyWindow(window);
 	//SDL_Quit();	
  
}

class TecladoPersonagem{

	private:
	
	public:
		bool readkeys(ModelMapp &M, ModelPersonagem &P, ControllerPersonagem &C);
};

bool TecladoPersonagem::readkeys(ModelMapp &M, ModelPersonagem &P, ControllerPersonagem &C){
	
  bool rodando = true;
  bool trigger = false;
  	
	// Variaveis para verificar eventos
  SDL_Event evento; // eventos discretos
  const Uint8* state = SDL_GetKeyboardState(nullptr); // estado do teclado

 // while(rodando){
    // Polling de eventos
    SDL_PumpEvents(); // atualiza estado do teclado
        
    if (state[SDL_SCANCODE_LEFT]){
    	C.move(M,P,-1,0);  
    	trigger = true;
    	//std::cout << P.posicao[0] << " " << P.posicao[1] << "\n";                   // altera mapa e posicao
    	//target.x = (P.posicao[0])*SECOES_X;  // atualiza viewer com a nova posicao
    	}
    if (state[SDL_SCANCODE_RIGHT]){
     	C.move(M,P,1,0); 
     	trigger = true;
     	//std::cout << P.posicao[0] << " " << P.posicao[1] << "\n";
     	//target.x = (P.posicao[0])*SECOES_X;
     	}
    if (state[SDL_SCANCODE_UP]){ 
    	C.move(M,P,0,-1); 
    	trigger = true;
    	//std::cout << P.posicao[0] << " " << P.posicao[1] << "\n";
    	//target.y = (P.posicao[1])*SECOES_Y;
    	}
    if (state[SDL_SCANCODE_DOWN]){
    	C.move(M,P,0,1);
    	trigger = true;  
    	//std::cout << P.posicao[0] << " " << P.posicao[1] << "\n";
    	//target.y = (P.posicao[1])*SECOES_Y;
	}
    while (SDL_PollEvent(&evento)) {
      if (evento.type == SDL_QUIT) {
        rodando = false;
		      }
      // Exemplos de outros eventos.
      // Que outros eventos poderiamos ter e que sao uteis?
      //if (evento.type == SDL_KEYDOWN) {
      //}
      //if (evento.type == SDL_MOUSEBUTTONDOWN) {
      }
		    //}
	
     return trigger;	
	}

int main() {
	
	ModelMapp M;
	ModelPersonagem P1;
	ControllerPersonagem C1;
	M.mapp_terrain();
	M.forbbid_terrain();
	TecladoPersonagem T1;
	ViewerMapp W1;
	ViewerPersonagem V1;

	
	bool t1;
	bool rodando = true;
	
	P1.set_personagem(M,7,7);
	W1.viewmapp(M);
	//V1.viewpersonagem(M,P1,C1);
	while(rodando){	
	
		t1 = T1.readkeys(M,P1,C1);
		if(t1==true) {
			//destroi o view antigo e cria um novo -- idealmente só para o personagem
			W1.destroymapp(W1);
			V1.destroypersonagem(V1);
			V1.viewpersonagem(M,P1,C1);
			t1 = false;
		}
	}

  
  
   
  return 0;
}
