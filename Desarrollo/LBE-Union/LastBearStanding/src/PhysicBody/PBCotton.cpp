#include "PhysicBody/PBCotton.h"
#include "World.h"

PBCotton::PBCotton(){
    m_pWorld = World::Inst()->GetWorld();
    m_pBody = NULL;
    m_category = 0;
    m_userData = 0;
    m_mask = 0;
}
PBCotton::~PBCotton(){
	DestroyBody();
}
int PBCotton::Inicialize( b2Vec2 pos, b2Vec2 tam){
	DestroyBody();
	InitBody(pos,tam);
    InitFixtures(tam);
    return m_bodyId;
}
b2Vec2 PBCotton::GetPosition(){
	return PhysicBody::DefGetPosition();
}
b2Vec2 PBCotton::GetLinearVelocity(){
	return PhysicBody::DefGetLinearVelocity();
}
float    PBCotton::GetRotation(){
    return PhysicBody::DefGetRotation();
}
int    PBCotton::GetId(){
    return PhysicBody::DefGetId();
}
void   PBCotton::SetRotation(float angle){
	PhysicBody::DefSetRotation(angle);
}
void   PBCotton::SetFixedRotation(bool fixed){
	PhysicBody::DefSetFixedRotation(fixed);
}
void   PBCotton::SetPosition(b2Vec2 pos){
	PhysicBody::DefSetPosition(pos);
}
void   PBCotton::SetAngularVelocity(float vel){
	PhysicBody::DefSetAngularVelocity(vel);
}
void   PBCotton::SetLinearVelocity(b2Vec2 vel){
	PhysicBody::DefSetLinearVelocity(vel);
}
void   PBCotton::SetCategory(uint16 i){
    PhysicBody::DefSetCategory(i);
}
void   PBCotton::SetUserData(int i){
    PhysicBody::DefSetUserData(i);
}
void   PBCotton::SetMask(uint16 i){
    PhysicBody::DefSetMask(i);
}
void PBCotton::Catch(int id){
    PhysicBody::DefCatch(id);
}
void PBCotton::Release(){
    PhysicBody::DefRelease();
}
void PBCotton::DestroyFixtures(){
    for (b2Fixture* f = m_pBody->GetFixtureList(); f;){
        b2Fixture* fixtureToDestroy = f;
        f = f->GetNext();
        m_pBody->DestroyFixture( fixtureToDestroy );
    }
}
void PBCotton::DestroyBody(){
    if(m_pWorld && m_pBody){
       DestroyFixtures();
       m_pWorld->DestroyBody(m_pBody);
       m_pBody = NULL;
    }
}
void PBCotton::InitBody(b2Vec2 pos,b2Vec2 tam){
    b2BodyDef bodyDef;
    bodyDef.bullet = true;
    bodyDef.position.Set(pos.x+(tam.x/2),-1*(pos.y-(tam.y/2)));
    bodyDef.type = b2_dynamicBody;
    m_pBody = m_pWorld->CreateBody(&bodyDef);
    m_bodyId = PhysicBody::GenerateId();
    m_pBody->SetUserData((void*)m_bodyId);
}
void PBCotton::InitFixtures(b2Vec2 tam){
    b2FixtureDef fixtureDef;
    b2PolygonShape polyShape;
    polyShape.SetAsBox(tam.x/2.0f,tam.y/2.0f);
    fixtureDef.shape = &polyShape;
    fixtureDef.friction = 0.1f;
    fixtureDef.restitution  = .2f;
    fixtureDef.density  =  .01f;
    fixtureDef.filter.categoryBits = M_PARTICULA;
    fixtureDef.filter.maskBits = M_PARTICULA|M_SUELO;
    b2Fixture* fixture = m_pBody->CreateFixture(&fixtureDef);
    fixture->SetUserData((void*)DATA_PARTICULA);
}
