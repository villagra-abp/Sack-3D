
#include "IrrManager.h"
#include "MyEventReceiver.h"
#include "DebugInfo.h"
#include "HUD.h"
#include "../BearEngine/include/shader.h"
#include "../BearEngine/include/tmotorbear.h"
#include <sstream>
IrrMngr* IrrMngr::pinstance = 0;
IrrMngr* IrrMngr::Inst(){
	if(pinstance == 0){
		pinstance = new IrrMngr;
	}
	return pinstance;
}
void IrrMngr::Reset(){
    smgr->clear();
}
const unsigned int IrrMngr::m_windowSize = 600;

void IrrMngr::Close(){device->closeDevice();}
IrrMngr::IrrMngr():m_debugMode(false){
	irr::IrrlichtDevice *nulldevice = irr::createDevice(irr::video::EDT_NULL);
	myEventReceiver = new MyEventReceiver();
	m_windowWidth = m_windowSize*16/9;
	m_windowHeight = m_windowSize;
	irr::core::dimension2d<irr::u32> deskres = nulldevice->getVideoModeList()->getDesktopResolution();
	//device = createDevice( irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(m_windowWidth,m_windowSize), 32, false, true, true, myEventReceiver );
	device = createDevice( irr::video::EDT_OPENGL, deskres, 32, true, true, true, myEventReceiver );
	driver = device->getVideoDriver();
	device->setWindowCaption(L"Last Bear Standing");
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();
	smgr->addLightSceneNode(0, irr::core::vector3df(-15,5,-10),irr::video::SColorf(255.0f, 255.0f, 255.0f));
    smgr->setAmbientLight(irr::video::SColor(0,255,255,255));
	timer = device->getTimer();
	//device->setResizable(true);

	m_gMotorBear.Reset(new TMotorBear(1080,720,"Last Bear Standing"));
    m_gShader.Reset(new Shader("../BearEngine/res/basicShaderLuz"));
    m_motorBear = m_gMotorBear.Get();
    m_shader = m_gShader.Get();
    m_camara = m_motorBear->crearObjetoCamaraCompleto(m_motorBear->getRaiz(),"Camara",glm::vec3(0,0,0), 70.0f, (float)1080/(float)720, 0.01f,1000.0f);
    m_motorBear->activarCamara(m_camara);
    m_motorBear->TrasladarObjeto(m_camara,glm::vec3(10,-10,10));
    //TNodo* cubo = m_motorBear->crearObjetoMallaCompleto(m_motorBear->getRaiz(), "../BearEngine/res/cubo.obj","Cubo");
}
void IrrMngr::setBackgroundImage(irr::video::ITexture* bimage){
  	//driver->removeTexture();
  	m_backgroundImage = bimage;
}

void IrrMngr::InstanciaVariables(int* puntuaciones){
  	debugInfo.Reset(new DebugInfo());
  	hud.Reset(new HUD(puntuaciones,smgr->getVideoDriver()->getScreenSize().Width,smgr->getVideoDriver()->getScreenSize().Height));
}
void IrrMngr::Update(){
    m_motorBear->Clear(0.0f,1.15f,0.3f,1.0f);
	//smgr->drawAll();
	m_shader->Bind();
	//hud.Get()->Draw();
	//if(m_debugMode)
	//	debugInfo.Get()->Draw(smgr->getVideoDriver()->getFPS());
	//endScene();

	m_motorBear->draw(m_shader);
	m_motorBear->UpdateDisplay();
}
//BearEngine
TNodo* IrrMngr::CreateBearNode(int id, glm::vec3 pos,glm::vec3 tam){
    std::ostringstream strm;
    strm << id;
	TNodo* nodo = m_motorBear->crearObjetoMallaCompleto(m_motorBear->getRaiz(), "../BearEngine/res/cubo.obj",(char*)strm.str().c_str());
	//std::cout<<id<<","<<(char*)strm.str().c_str()<<std::endl;
	m_motorBear->TrasladarObjeto(nodo,pos);
	m_motorBear->EscalarObjeto(nodo,tam);
    return nodo;
}
void IrrMngr::SetBearCubePosition(TNodo* nodo,glm::vec3 pos ){
	m_motorBear->TrasladarObjeto(nodo,pos);
}
void IrrMngr::SetBearCubeRotation(TNodo* nodo,float rot ){
	m_motorBear->RotarObjeto(nodo,glm::vec3(0,0,rot));
}
void IrrMngr::SetBearCameraPosition(float x, float y, float z){
    m_motorBear->TrasladarObjeto(m_camara,glm::vec3(x,y,z));
}
void IrrMngr::RemoveBearNode(int id){
    std::ostringstream strm;
    strm << id;
    m_motorBear->borrarRecurso((char*)strm.str().c_str());
    std::cout<<(char*)strm.str().c_str()<<std::endl;
}
//BearEngine
irr::scene::IMeshSceneNode* IrrMngr::addCubeSceneNode(irr::core::vector3df tam,irr::video::SColor color){
	irr::scene::IMesh* mesh = smgr->getGeometryCreator()->createCubeMesh(tam);
	irr::scene::IMeshSceneNode* node = smgr->addMeshSceneNode(mesh);
    smgr->getMeshManipulator()->setVertexColors(node->getMesh(), color);
	return node;
}
irr::scene::IMesh* IrrMngr::createCubeMesh(irr::core::vector3df pos, irr::core::vector3df tam, irr::video::SColor color){
	irr::scene::IMesh* mesh = smgr->getGeometryCreator()->createCubeMesh(tam);
	smgr->addMeshSceneNode(mesh)->setPosition(pos);
    smgr->getMeshManipulator()->setVertexColors(mesh, color);
    return  mesh;
}

void  IrrMngr::SwitchDebugMode(){
  if(m_debugMode){
    m_debugMode = false;
  }
  else{
    m_debugMode = true;
  }
}
irr::io::IXMLReader* IrrMngr::createXMLReader(irr::core::stringw file){return device->getFileSystem()->createXMLReader(file);}
float IrrMngr::getTime(){return timer->getTime();}
void IrrMngr::beginScene(){
	driver->beginScene(true, true, irr::video::SColor(255,255, 255, 255));
	//driver->draw2DImage(m_backgroundImage,irr::core::rect<irr::s32>(0, 0, m_windowWidth, m_windowHeight),irr::core::rect<irr::s32>(0, 0, m_backgroundImage->getSize().Width, m_backgroundImage->getSize().Height));
	driver->draw2DImage(m_backgroundImage,irr::core::rect<irr::s32>(0, 0, driver->getScreenSize().Width, driver->getScreenSize().Width),irr::core::rect<irr::s32>(0, 0, driver->getScreenSize().Width, driver->getScreenSize().Width));
}
void IrrMngr::endScene(){driver->endScene();}
void IrrMngr::drop(){driver->drop();}
irr::scene::ISceneManager* IrrMngr::getManager(){return smgr;}
MyEventReceiver* IrrMngr::getEventReciever(){return myEventReceiver;}
irr::IrrlichtDevice* IrrMngr::getDevice(){return device;}
irr::video::IVideoDriver* IrrMngr::getDriver(){return driver;}
irr::gui::IGUIEnvironment* IrrMngr::getGUI(){return guienv;}
irr::ITimer* IrrMngr::getTimer(){return timer;}
IrrMngr::~IrrMngr(){}