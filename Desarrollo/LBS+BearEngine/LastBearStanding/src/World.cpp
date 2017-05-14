
#include "PathFinding/Lista.h"
#include "PathFinding/Nodo.h"
#include "MyContactListener.h"
#include "BearManager.h"
#include "PlayerRed.h"
#include "Teleport.h"
#include "Metralla.h"
#include "Particle.h"
#include "Platform.h"
#include "Spawner.h"
#include "Cogible.h"
#include "Camera.h"
#include "Player.h"
#include "World.h"
#include "Bala.h"
#include "Bot.h"
#include "Map.h"
#include "SDL.h"
#include "GameObject.h"
#include "MyEventReceiver.h"
#include "PhysicBody/PBAlivePlayer.h"
#include "PhysicBody/PBDefault.h"

const int World::velocityIterations = 8;
const int World::positionIterations = 3;
World* World::pinstance = NULL;
World* World::Inst(){
	if(!pinstance){
    pinstance = new World;
  }
  return pinstance;
}
void World::Reset(){
  if(pinstance)delete pinstance;
  pinstance = new World;

}
World::World():m_debugMode(false){
	world.Reset(new b2World(b2Vec2(0.0f, -10.0f), false));
	contactListener.Reset(new MyContactListener);
	world.Get()->SetContactListener(contactListener.Get());
}
Lista* World::getListaNodos(){
  return m_Mapa.Get()->getListaNodos();
}
int World::getTimeMapa(){
  return m_Mapa.Get()->getTime();
}
void World::inicializaVariables(const char* mapFile,int *puntuaciones,int numMap){
  m_Mapa.Reset(new Map(mapFile,numMap));
  camara.Reset(new GameCamera());
  char* textFondo;
  switch(numMap){
    case 1:  textFondo = "media/Maps/Background/fridge.jpg"; break;
    case 2:  textFondo = "media/Maps/Background/room.jpg"; break;
    case 3:  textFondo = "media/Maps/Background/kitchen.jpg"; break;
    case 4:  textFondo = "media/Maps/Background/bathroom.jpg"; break;
    case 5:  textFondo = "media/Maps/Background/garden.jpg"; break;
    case 6:  textFondo = "media/Maps/Background/room.jpg"; break;
    case 7:  textFondo = "media/Maps/Background/room.jpg"; break;
    case 8:  textFondo = "media/Maps/Background/garden.jpg"; break;
    case 9:  textFondo = "media/Maps/Background/room.jpg"; break;    
    case 10:  textFondo = "media/Maps/Background/garden.jpg"; break;
    default: textFondo = "media/Maps/Background/room.jpg"; break;
  }
  m_fondo.Inicialize(new PBDefault(),0,0,0,b2Vec2(0,0),glm::vec3(9,16,0.01f),NULL,textFondo, false);
  m_hud.Inicialize(puntuaciones);
  for (int i = 0; i < m_Players.Size(); ++i){
    if(Bot* bot = dynamic_cast<Bot*>(m_Players.Get(i))){
      bot->InicializaVariables();
    }
  }
  m_fondo.SetRotation(-90*3.14/180);
  TimeStamp = SDL_GetTicks();
  DeltaTime = SDL_GetTicks() - TimeStamp;
}
Player* World::getPlayer(int mando){
  for(int i = 0; i < m_Players.Size(); ++i){
     if(m_Players.Get(i) && m_Players.Get(i)->getMando() == mando)
        return m_Players.Get(i);
  }
  return NULL;
}
int World::getVivos(){
    int vivos = 0;

    for (int i = 0; i < m_Players.Size(); ++i){
      if(!m_Players.Get(i)->getMuerto())vivos++;
    }

    return vivos;
}
int World::getGanador(){
  for (int i = 0; i < m_Players.Size(); ++i){
    if(!m_Players.Get(i)->getMuerto())return i;
  }
  return -1;
}
void  World::SwitchDebugMode(){
  if(m_debugMode){
    m_debugMode = false;
  }
  else{
    m_debugMode = true;
  }
  for (int i = 0; i < m_Nodos.Size(); ++i){
    if(m_Nodos.Get(i)){
      m_Nodos.Get(i)->SetVisible(m_debugMode);
    }
  }
}
int World::Update(int fps,MyEventReceiver *events){
  DeltaTime = SDL_GetTicks() - TimeStamp;
  TimeStamp = SDL_GetTicks();
  world.Get()->Step(1.f/20.f, velocityIterations, positionIterations);
  world.Get()->ClearForces();
  UpdateBalas();
  UpdateParticles();
  UpdateMetrallas();
  UpdateCogibles();
  UpdateSpawners();
  int players;
  players = UpdatePlayers(events);
  glm::vec3 posCam = camara.Get()->update(TimeStamp, fps);
  m_hud.Update(posCam);
  m_fondo.SetPosition(b2Vec2(posCam.x,posCam.y));
  m_fondo.SetZ(posCam.z-35);
  m_fondo.Update();
  return players;
}

void World::UpdateCogibles(){
  for (int i = 0; i < m_Cogibles.Size(); ++i){
    if(m_Cogibles.Get(i)){
      m_Cogibles.Get(i)->actualiza();
      if(m_Cogibles.Get(i)->getAutoDestruir()){
        m_Cogibles.Remove(i);
      }
    }
  }
}
void World::UpdateBalas(){
  for (int i = 0; i < m_Balas.Size(); ++i){
    if(m_Balas.Get(i)){
      m_Balas.Get(i)->actualiza();
      if(m_Balas.Get(i)->getDestruir()){
          m_Balas.Remove(i);
      }
    }
  }
}
int World::UpdatePlayers(MyEventReceiver *events){
  int players = 0;
  for (int i = 0; i < m_Players.Size(); ++i){
    if(m_Players.Get(i)){
      m_Players.Get(i)->actualiza(events);
    }
    if(!m_Players.Get(i)->getMuerto())players++;
  }
  return players;
}
void World::UpdateSpawners(){
  for (int i = 0; i < m_Spawners.Size(); ++i){
    if(m_Spawners.Get(i)){
      m_Spawners.Get(i)->actualiza();
    }
  }
}
void World::UpdateMetrallas(){
  for (int i = 0; i < m_Metrallas.Size(); ++i){
    if(m_Metrallas.Get(i)){
      m_Metrallas.Get(i)->actualiza();
      if(m_Metrallas.Get(i)->getDestruir()){
          m_Metrallas.Remove(i);
      }
    }
  }
}
void World::UpdateParticles(){
  for (int i = 0; i < m_Particles.Size(); ++i){
    if(m_Particles.Get(i)){
      m_Particles.Get(i)->actualiza();
      if(m_Particles.Get(i)->getDestruir()){
          m_Particles.Remove(i);
      }
    }
  }
}
