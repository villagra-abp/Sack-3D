/*******************************************************************************
Estudio Rorschach - Last Bear Standing
Copyright  2016. All Rights Reserved.

Project:       Last Bear Standing
File:          Pistola.cpp

Author:        Estudio Rorschach
Created:       08/12/2016 Jorge Puerto
Modified:      09/01/2017 Jorge Puerto

Overview:
Clase que contiene el codigo de funcionamiento para las Pistolas.
*******************************************************************************/

#include "Pistola.h"
#include "PhysicWorld.h"
#include "IrrManager.h"

#define PISTOLA 30                     ///< Int para las colisiones de las Pistolas
#define SENSORPISTOLA 35               ///< Int para las colisiones del area cogible de las Pistolas
#define RADTOGRAD 180 / 3.14159265     ///< Conversor de radianes a grados

/******************************************************************************
                               Pistola
*******************************************************************************/

//---------------------------------------------------------------------------
/**
   Constructor
*/
Pistola::Pistola(int modelo,vector3df pos){
    siendoCogida = false;
    vector3df tam2 = vector3df(5, 3, 1);
    vector3df tam = vector3df(tam2.X/MPP,tam2.Y/MPP,tam2.Z/MPP);
    node = IrrManager::Instance()->addCubeSceneNode(tam,SColor(255, 255, 0, 0));
    node->setPosition(vector3df(pos.X/MPP,pos.Y/MPP,0/MPP));
    b2BodyDef bodyDef;
    b2FixtureDef fixtureDef;
    b2PolygonShape polyShape;

    bodyDef.position.Set(pos.X/MPP,pos.Y/MPP);
    bodyDef.type = b2_dynamicBody;

    body  = PhysicWorld::Instance()->GetWorld()->CreateBody(&bodyDef);

    polyShape.SetAsBox((tam.X/2.0f),(tam.Y/2.0f));
    fixtureDef.shape = &polyShape;
    fixtureDef.friction = 0.5f;
    fixtureDef.restitution  = 0.2f;
    fixtureDef.density  = 2.0f;
    b2Fixture* fixture = body->CreateFixture(&fixtureDef);
    fixture->SetUserData((void*)PISTOLA);
    body->SetFixedRotation(true);
    polyShape.SetAsBox((tam.X*2.0f),(tam.Y*2.0f));
    fixtureDef.isSensor = true;
    b2Fixture* PistolaSensorFixture = body->CreateFixture(&fixtureDef);
    PistolaSensorFixture->SetUserData((void*)SENSORPISTOLA);
}
//---------------------------------------------------------------------------
/**
   Metodo que actualiza la posicion y rotacion del Pistola
*/
void Pistola::actualiza(){
    if(siendoCogida){
        node->setPosition(vector3df(body->GetPosition().x+((5.0f/MPP)*dir),body->GetPosition().y,0));
    }
    else node->setPosition(vector3df(body->GetPosition().x,body->GetPosition().y,0));
    node->setRotation(vector3df(0,0,body->GetAngle()*RADTOGRAD));
}

//---------------------------------------------------------------------------
/**
   Getters and setters
*/
b2Body* Pistola::getBody(){return body;}
void Pistola::setCogida(bool aux){

    vector3df tam2 = vector3df(5,3,1);
    vector3df tam = vector3df(tam2.X/MPP,tam2.Y/MPP,tam2.Z/MPP);
    body->DestroyFixture(body->GetFixtureList());
    body->DestroyFixture(body->GetFixtureList());
    b2BodyDef bodyDef;
    b2FixtureDef fixtureDef;
    b2PolygonShape polyShape;
    b2Fixture* PistolaSensorFixture;
    b2Fixture* fixture;
    if(aux){
        polyShape.SetAsBox(tam.X/2.0f,tam.Y/2.0f);
        fixtureDef.shape = &polyShape;
        fixtureDef.density  = 0.1f;
        fixtureDef.isSensor = true;
        fixture = body->CreateFixture(&fixtureDef);
        fixture->SetUserData((void*)30);
        body->SetFixedRotation(true);
        polyShape.SetAsBox((tam.X/4.0f),(tam.Y/4.0f));
        fixtureDef.isSensor = true;
        PistolaSensorFixture = body->CreateFixture(&fixtureDef);
        PistolaSensorFixture->SetUserData((void*)35);
    }
    else{
        b2PolygonShape polyShape;
        polyShape.SetAsBox(tam.X/2.0f,tam.Y/2.0f);
        fixtureDef.shape = &polyShape;
        fixtureDef.friction = 0.5f;
    fixtureDef.restitution  = 0.2f;
    fixtureDef.density  = 2.0f;
        fixture = body->CreateFixture(&fixtureDef);
        fixture->SetUserData((void*)30);
        body->SetFixedRotation(true);
        polyShape.SetAsBox((tam.X*2.0f),(tam.Y*2.0f));
        fixtureDef.isSensor = true;
        PistolaSensorFixture = body->CreateFixture(&fixtureDef);
        PistolaSensorFixture->SetUserData((void*)35);
    }
    siendoCogida = aux;
}
bool Pistola::getCogida(){return siendoCogida;}
//---------------------------------------------------------------------------
/**
   Metodo que ejecuta el usar de la clase
*/
void Pistola::usar(){
    GameResource<Bala>* balaGR = PhysicWorld::Instance()->CreateBala(new Bala(vector3df(body->GetPosition().x, body->GetPosition().y, 0), 300, 2, 15.0f, dir));
    Bala* bala = balaGR->Get();
    b2Vec2 vel = bala->getBody()->GetLinearVelocity();
    vel.x = bala->velocidad;
    if(dir==1) bala->getBody()->SetLinearVelocity(vel);
    else bala->getBody()->SetLinearVelocity(-vel);
    PhysicWorld::Instance()->GetBalas()->push_back(bala);
}
//---------------------------------------------------------------------------
/**
   Destructor
*/
Pistola::~Pistola(){}
