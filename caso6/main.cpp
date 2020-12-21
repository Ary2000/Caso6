#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <omp.h>

#include "Pajaro.h"
#include "Pajaro.cpp"

std::string acortarOracion(std::string linea){
    return linea;
}

double valorAbsoluto(double numero) { return numero >= 0 ? numero : numero * -1; }

int main()
{
    std::ifstream fileStream;
    fileStream.open("C:\\Users\\Ary\\VSC_Projects\\caso6\\resultados.txt");
    if(!fileStream.fail()){
        
        std::string linea;
        std::getline(fileStream, linea);
        int tamanoAves = std::stoi(linea);
        std::vector<Pajaro> pajaros;
        for(int id = 0; id < tamanoAves; id++){
            pajaros.push_back(Pajaro(id));
        }
        int idActual = 0;
        std::vector<std::string> data;
        std::vector<std::vector<std::string>> listaData;
        while(std::getline(fileStream, linea)){
            data.clear();
            std::getline(fileStream, linea);

            //ID
            data.push_back(linea.substr(linea.find(' ') + 1, linea.size()));
            std::getline(fileStream, linea);

            std::getline(fileStream, linea);
            //Velocidades
            data.push_back(linea.substr(linea.find(' ') + 1, linea.size()));

            std::getline(fileStream, linea);
            //Distancias
            std::getline(fileStream, linea);
            data.push_back(linea.substr(linea.find(' ') + 1, linea.size()));

            listaData.push_back(data);
        }
        int np, id, i, N;
        N = listaData.size();
        #pragma omp parallel private(np, id, i)
        {
            np = omp_get_num_threads();
            id = omp_get_thread_num();
            for(int i = id*N/np; i < (id+1)*N/np; i++){
                int posicion = std::stoi(listaData[i][0]);
                double velocidad = std::stod(listaData[i][1]);
                pajaros[posicion].totalDistancia += std::stod(listaData[i][2]);
                if(pajaros[posicion].maxVelocidad < velocidad)
                    pajaros[posicion].maxVelocidad = velocidad;
                if(pajaros[posicion].minVelocidad > velocidad)
                    pajaros[posicion].minVelocidad = velocidad;
            }
        }

        #pragma omp critical
        double basura = 0;
        Pajaro velocidadAlta1 = pajaros[0];
        Pajaro velocidadAlta2 = pajaros[0];
        Pajaro velocidadBaja1 = pajaros[0];
        Pajaro velocidadBaja2 = pajaros[0];
        Pajaro distanciaAlta1 = pajaros[0];
        Pajaro distanciaAlta2 = pajaros[0];
        Pajaro distanciaBaja1 = pajaros[0];
        Pajaro distanciaBaja2 = pajaros[0];

        N = pajaros.size();
        #pragma omp parallel private(np, id, i)
        {
            np = omp_get_num_threads();
            id = omp_get_thread_num();
            for(int i = id*N/np; i < (id+1)*N/np; i++){
                Pajaro pajaroActual = pajaros[i];
                if(pajaroActual.totalDistancia > distanciaAlta1.totalDistancia){
                    distanciaAlta2 = distanciaAlta1;
                    distanciaAlta1 = pajaroActual;    
                }
                else if(pajaroActual.totalDistancia > distanciaAlta2.totalDistancia)
                    distanciaAlta2 = pajaroActual;
                if(pajaroActual.totalDistancia < distanciaBaja1.totalDistancia){
                    distanciaBaja2 = distanciaBaja1;
                    distanciaBaja1 = pajaroActual;
                }
                else if(pajaroActual.totalDistancia < distanciaBaja2.totalDistancia)
                    distanciaBaja2 = pajaroActual;
                if(pajaroActual.maxVelocidad > velocidadAlta1.maxVelocidad){
                    velocidadAlta2 = velocidadAlta1;
                    velocidadAlta1 = pajaroActual;
                }
                else if(pajaroActual.maxVelocidad > velocidadAlta2.maxVelocidad)
                    velocidadAlta2 = pajaroActual;
                if(pajaroActual.minVelocidad < velocidadBaja1.minVelocidad){
                    velocidadBaja2 = velocidadBaja1;
                    velocidadBaja1 = pajaroActual;
                }
                else if(pajaroActual.minVelocidad < velocidadBaja2.minVelocidad)
                    velocidadBaja2 = pajaroActual;
            }
        }

        #pragma omp critical

std::cout << "Rankings:" << std::endl;
        std::cout << "Pajaro mas rapido: " << velocidadAlta1.id << " Color asignado: Rojo" << std::endl;
        std::cout << "Segundo pajaro mas rapido " << velocidadAlta2.id << " Color asignado: Salmon" << std::endl;
        std::cout << "Pajaro mas lento: " << velocidadBaja1.id << " Color asignado: Azul" << std::endl;
        std::cout << "Segundo pajaro mas lento: " << velocidadBaja2.id << " Color asignado: Azul cielo intenso" << std::endl;
        std::cout << "Pajaro con distancia mas recorrida: " << distanciaAlta1.id << " Color asignado: Magenta" << std::endl;
        std::cout << "Segundo pajaro con distancia mas recorrida: " << distanciaAlta2.id << " Color asignado: Purpura" << std::endl;
        std::cout << "Pajaro mas lento: " << distanciaBaja1.id << " Color asignado: Lima" << std::endl;
        std::cout << "Segundo pajaro mas lento: " << distanciaBaja2.id << " Color asignado: Verde oscuro" << std::endl;
        
        std::cout << std::endl;

        std::cout << "if(this.distanciaTotal > " << distanciaAlta2.totalDistancia << "){" << std::endl;
        std::cout << "  colorActual = colorActual + 0x9966CC;" << std::endl;
        std::cout << "}" << std::endl;
        std::cout << "if(this.distanciaTotal > " << distanciaAlta1.totalDistancia << "){" << std::endl;
        std::cout << "  colorActual = colorActual + 0xFF00FF;" << std::endl;
        std::cout << "}" << std::endl;
        std::cout << "if(this.distanciaTotal < " << distanciaBaja2.totalDistancia << "){" << std::endl;
        std::cout << "  colorActual = colorActual + 0x33CC66;" << std::endl;
        std::cout << "}" << std::endl;
        std::cout << "if(this.distanciaTotal < " << distanciaBaja1.totalDistancia << "){" << std::endl;
        std::cout << "  colorActual = colorActual + 0x00FF00;" << std::endl;
        std::cout << "}" << std::endl;
        std::cout << "if(velocidadActual > " << velocidadAlta2.maxVelocidad << "){" << std::endl;
        std::cout << "  colorActual = colorActual + 0xFF9966;" << std::endl;
        std::cout << "}" << std::endl;
        std::cout << "if(velocidadActual > " << velocidadAlta1.maxVelocidad << "){" << std::endl;
        std::cout << "  colorActual = colorActual + 0xFF0000;" << std::endl;
        std::cout << "}" << std::endl;
        std::cout << "if(velocidadActual < " << velocidadBaja2.minVelocidad << "){" << std::endl;
        std::cout << "  colorActual = colorActual + 0x00CCFF;" << std::endl;
        std::cout << "}" << std::endl;
        std::cout << "if(velocidadActual < " << velocidadBaja1.minVelocidad << "){" << std::endl;
        std::cout << "  colorActual = colorActual + 0x0000FF;" << std::endl;
        std::cout << "}" << std::endl;
    }
    else
        std::cout << "Archivo no existe" << std::endl;
    return 0;
}