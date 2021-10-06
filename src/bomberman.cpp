#include <memory>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
const int MAX_I = 16;
const int MAX_J = 16;

/** Sugestão de convenção para nome das classes e métodos "MVCClasse" */
class ModelMapp{
    public:
		int terreno[MAX_I][MAX_J];
		void mapp_terrain();
        void forbiden_terrain();
};

void ModelMapp::mapp_terrain(){

	int i,j;

	for(i = 0 ; i < MAX_I ; i++){
		for( j = 0 ; j < MAX_J ; j++)
			terreno[i][j] = 1; /** 1 codifica posição habilitada/disponivel */
	}
}

void ModelMapp::forbiden_terrain(){
    int i, j;

    for (i = 0 ; i < MAX_I ; i++){
        for (j = 0 ; j < MAX_J ; j++){
            terreno[0][j] = -1;
            terreno[MAX_I][j] = -1;
            terreno[i][0] = -1;
            terreno[i][MAX_J] = -1;
        }
    }
}

class ModelPersonagem{
	public: 
		int posicao[2];
		void set_personagem(ModelMapp &M, int *x, int *y); /** inicializa  personagem no mapa */
};

void ModelPersonagem::set_personagem(ModelMapp &M, int *x, int *y){
	/*
    int xini, yini;
	std::cin >> xini;
	std::cin >> yini;
	*/
    
    posicao[0] = 1;
    posicao[1] = 1;

	if((*x) > MAX_I-1)
        *x = MAX_I-1;
    if((*y) > MAX_J-1)
      	*y = MAX_J-1;        
	
	M.terreno[*x][*y] = 0;

	posicao[0] = *x;
	posicao[1] = *y;
}
class ControllerPersonagem{

	public:
		int posicao[2];
		void move(ModelMapp &M, ModelPersonagem &P);
};

void ControllerPersonagem::move(ModelMapp &M, ModelPersonagem &P){
	
	int x=0,y=0;
	std::cin>> x;
	std::cin>> y;
	//Preciso descobrir como ler as direções do teclado
	if(x>15)
       x = 15;
    if(y>15)
       y = 15;        
	
	
	M.terreno[x][y] = 0; // No futuro podemos diferenciar posicoes ocupadas de posicoes proibidas
	M.terreno[P.posicao[0]][P.posicao[1]] = 1;
	
	P.posicao[0] = x;
	P.posicao[1] = y;
}
	

int main(){
    /**
     * Mapa -> M
     * Player -> P1
     * Controller P1 -> C1
     */

    int i, j;
    ModelMapp M;        
	ModelPersonagem P1;
	ControllerPersonagem C1;
    int pos_x = 1, pos_y = 1;

	M.mapp_terrain();
		
    /** Player no mapa */
	P1.set_personagem(M, &pos_x, &pos_y);
	
	for (i=0;i<MAX_I;i++){
		for(j=0;j<MAX_J;j++)
			std::cout << M.terreno[i][j];
		std::cout << "\n";
		}
	std::cout <<"("<< P1.posicao[0]<<"," << P1.posicao[1] <<")" << "\n";
	
	//C1.move(M,P1);	
    pos_x = 3;
    pos_y = 3;
	P1.set_personagem(M, &pos_x, &pos_y);

	for (i=0;i<16;i++){
		for(j=0;j<16;j++)
			std::cout << M.terreno[i][j];
		std::cout << "\n";
		}
	std::cout <<"("<< P1.posicao[0]<<"," << P1.posicao[1] <<")" << "\n";

	return 0;
}
