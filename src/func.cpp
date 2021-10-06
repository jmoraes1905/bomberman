#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "func.h"

void ModelMapp::mapp_terrain(){

//  1 posição habilitada/disponivel
//  0 posição a ser habilitada (blocos a serem destruídos)
// -1 posição indisponível

	for( unsigned int i=0 ; i < MAX_I ; i++ ){
		for( unsigned int j=0 ; j < MAX_J ; j++ ){
		  terreno[i][j] = 1;
    }
	}
}

void ModelPersonagem::set_personagem(ModelMapp &M){
	
	int xini, yini;
	std::cin>>xini;
	std::cin>>yini;
	
	if(xini > MAX_I-1)
	    xini = MAX_I-1;
  
	if(yini > MAX_J-1)
    	yini = MAX_J-1;        
	
	M.terreno[xini][yini] = 0;
	
	posicao[0] = xini;
	posicao[1] = yini;

}

void ControllerPersonagem::move(ModelMapp &M, ModelPersonagem &P, int x, int y){
	
  /** Desocupa posição antiga */
  M.terreno[P.posicao[0]][P.posicao[1]] = 1;

  // TODO: De-bouncing of the position (tirar o while como atualizador da posição)
  //bool key_state = false;

  // Incrementa posição por x e por y
	P.posicao[0] += x;
	P.posicao[1] += y;

	if(P.posicao[0] > MAX_I-1)
        P.posicao[0] = MAX_I-1;
    else if(P.posicao[0] < 0)
        P.posicao[0] = 0;

    if(P.posicao[1] > MAX_J-1)
        P.posicao[1] = MAX_J-1;
    else if(P.posicao[1] < 0)
		P.posicao[1]=0;
	
    // Ocupa nova posição
	M.terreno[P.posicao[0]][P.posicao[1]] = 0;
}

/**Construtor do Viewer */
ViewerPersonagem::ViewerPersonagem(ModelMapp &M, ModelPersonagem &P, ControllerPersonagem &C, bool rodando){
  bool key_left = false;
  int h, v;

  /** Inicializando o subsistema de video do SDL */
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
  
  // Caso não consiga renderizar a janela corretamente
  if (renderer == nullptr) { 
    SDL_DestroyWindow(window);
    std::cout << SDL_GetError();
    SDL_Quit();
  }

  // Carregando texturas
  // personagem
  SDL_Texture *personagem = IMG_LoadTexture(renderer, "../assets/bomberman.png");
  // background
  SDL_Texture *tabuleiro = IMG_LoadTexture(renderer, "../assets/tabuleiro.png");

  // Quadrado onde a textura sera desenhada
  SDL_Rect target;

  target.x = P.posicao[0]*SECOES_X;
  target.y = P.posicao[1]*SECOES_Y;
  
  SDL_QueryTexture(personagem, nullptr, nullptr, &target.w, &target.h);

  // Variaveis para verificar eventos
  SDL_Event evento; // eventos discretos
  const Uint8* state = SDL_GetKeyboardState(nullptr); // estado do teclado

  while(rodando) {
    // Polling de eventos
    // atualiza estado do teclado
    SDL_PumpEvents(); 
          
    if (state[SDL_SCANCODE_LEFT]){
        key_left = true;
    }
    if (state[SDL_SCANCODE_LEFT] != key_left){
        h = -1;
        v = 0;
        C.move(M, P, h, v);
        target.x = (P.posicao[0])*SECOES_X;
        key_left = false;
    }
      // altera mapa e posicao
      // C.move(M,P,-1,0);
      // atualiza viewer com a nova posicao
      // target.x = (P.posicao[0])*SECOES_X;  

    if (state[SDL_SCANCODE_RIGHT]){
        if (state[SDL_SCANCODE_RIGHT]){
     	    C.move(M,P,1,0); 
     	    target.x = (P.posicao[0])*SECOES_X;            
            //key_state = false;
        }

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
    }

    // Desenhar a cena
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, tabuleiro, nullptr, nullptr);
    SDL_RenderCopy(renderer, personagem, nullptr, &target);
    SDL_RenderPresent(renderer);

    // Delay para diminuir o framerate
    SDL_Delay(60);
  }

  SDL_DestroyTexture(personagem);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();	
    
}
