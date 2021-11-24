#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <memory>
//#include "func.h"
// g++ -oview viewer.cpp -lSDL2 -lSDL2_image
#include "json.hpp"
#include <fstream>
#include <vector>
#include <thread>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>


class Model{

	private:
		float result;
		
	public:
		Model();
		void soma(float a, float b);
		void exportjson(json j);
		
		};
		
Model::Model() {

	result = 0;
	
}	
		
void Model::soma(float a, float b){

	result = a+b;
}

voidModel::exportjson(json j){

  boost::asio::io_service io_service;

  udp::endpoint local_endpoint(udp::v4(), 0);
  udp::socket meu_socket(io_service, local_endpoint);
  
  // Encontrando IP remoto
  boost::asio::ip::address ip_remoto =
    boost::asio::ip::address::from_string("127.0.0.1");

  udp::endpoint remote_endpoint(ip_remoto, 9001);

  meu_socket.send_to(boost::asio::buffer(j), remote_endpoint);
  
  std::cout<< "Cheguei aqui!"<< std::endl;
  
  }
	
class Controller{

	private:
		int p;
	public: 
		Controller();
		void capture(float a, float b);
		void importjson(json j);
};


Controller::Controller(){
	p=0;
}
	
Controller::capture(float a, float b){

	
	
	
		
			
