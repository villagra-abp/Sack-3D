#include "../include/tgestorrecursos.h"
#include "../include/mesh.h"
#include "../include/texture.h"

TGestorRecursos::TGestorRecursos()
{

}

TGestorRecursos::~TGestorRecursos()
{

   //std::cout<<recursos.size()<<std::endl;
   for(int i=0; i<recursos.size();i++){
//        std::cout<<recursos.at(i)->GetNombre()<<std::endl;
        borrarRecurso(recursos.at(i)->GetNombre());
        i--;
   }
}


TRecurso *TGestorRecursos::getRecurso(  char* nombre, int tipo){
//TRecurso *TGestorRecursos::getRecurso( const std::string& nombre, int tipo){

TRecurso* recu;
int encontrado=0;

    for(int i=0; i<recursos.size();i++){
        recu= recursos.at(i);
        if(recu->GetNombre()== nombre){

            recu=recursos.at(i);
            encontrado=1;
            return recursos.at(i);
        }
    }
    if(!encontrado){
        if(tipo==0){

            //Cargar fichero ,y apilar en el vector
            //Mesh* mesh = new Mesh(nombre);
            Mesh* mesh = new Mesh(nombre);
           // std::cout<<mesh<<std::endl;
          //  TRecursoMalla* malla= new TRecursoMalla;
            //malla->SetMeshes(load->getMeshes());
          // malla->SetMesh(mesh);
            recu=mesh;
            recu->setNombre(nombre);
            recu->setTipo(tipo);
            //std::cout<<"Malla Cargada"<<std::endl;
            recursos.push_back(recu);
            //std::cout<<recu->GetNombre()<<std::endl;
            //std::cout<<std::endl;
          //  delete(mesh); Peta
        }
        else if(tipo==1){

            //Cargar fichero ,y apilar en el vector
            //Mesh* mesh = new Mesh(nombre);
            Texture* text = new Texture(nombre);
           // std::cout<<mesh<<std::endl;
          //  TRecursoMalla* malla= new TRecursoMalla;
            //malla->SetMeshes(load->getMeshes());
          // malla->SetMesh(mesh);
            recu=text;
            recu->setNombre(nombre);
            recu->setTipo(tipo);
            //std::cout<<"Textura Cargada"<<std::endl;
            recursos.push_back(recu);
            std::cout<<recu->GetNombre()<<std::endl;
            std::cout<<std::endl;
          //  delete(mesh); Peta
        }
    }
    return recu;

}
void TGestorRecursos::borrarRecurso(char * nombre){
    TRecurso* recu;
    Mesh* mesh;
    int tipo;
    for(int i=0; i<recursos.size();i++){
            recu=recursos.at(i);
        if(recu->GetNombre()== nombre){
            tipo=recu->GetTipo();
           // std::cout<<"Recurso Encontrado"<<std::endl;
            if(tipo==0){
                mesh=static_cast<Mesh*>(recursos.at(i));
                delete(mesh);
            }

           recursos.erase(recursos.begin()+i);
            //std::cout<<"Recurso Borrado"<<std::endl;
            break;
        }
    }
}
void TGestorRecursos::verRecursos(){
    std::cout<<std::endl;
    for(int i =0; i<recursos.size();i++){
        std::cout<<recursos[i]->GetNombre()<<", ";
    }
      std::cout<<std::endl;
}
