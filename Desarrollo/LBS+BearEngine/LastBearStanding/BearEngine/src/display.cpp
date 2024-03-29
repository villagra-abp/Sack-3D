/*Display juego*/

#include "../include/display.h"

#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>
#endif
#include "../include/ttransform.h"


#include <iostream>

Display::Display(int width, int height, const std::string& title)
{
    malla=0;
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_DisplayMode dm;
    SDL_GetDesktopDisplayMode(0, &dm);
    widthx=dm.w-1;
    heighty=dm.w*9/16;
    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, widthx, heighty, SDL_WINDOW_OPENGL);
    m_glContext = SDL_GL_CreateContext(m_window);
    windowSurface = NULL;
    windowSurface = SDL_GetWindowSurface(m_window);
    ren = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    //fondo = NULL;
    //fondo = IMG_Load("res/fondo.jpg");
    //if(fondo == NULL)
    //    std::cout<<"ERROR CARGA IMG: "<<IMG_GetError()<<std::endl;

    GLenum status = glewInit();

    if(status != GLEW_OK){
    std::cerr<< "Glew ha fallado al iniciar"<< std::endl;
    }
    m_isClosed=false;

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}
Display::~Display()
{
    SDL_GL_DeleteContext(m_glContext);
    //SDL_FreeSurface(fondo);
    SDL_DestroyWindow(m_window);
    // fondo = NULL;
    m_window = NULL;
    SDL_QUIT;
}

void Display::Clear(float r, float g, float b, float a){
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
 }

bool Display::IsClosed(){
    return m_isClosed;
}
SDL_Texture* Display::CreaTextura(const std::string &archivo){
    SDL_Texture *texture       = NULL;
    SDL_Surface *imagenCargada = IMG_Load(archivo.c_str());
    texture = SDL_CreateTextureFromSurface(ren, imagenCargada);
    SDL_FreeSurface(imagenCargada);
    return texture;
}
void Display::RenderTextura(SDL_Texture *tex, int x, int y){
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;

    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(ren, tex, NULL, &dst);
}
int Display::Update(TNodo* TfCamera){
//void Display::Update(){

    TTransform * tras= static_cast<TTransform*> (TfCamera->getPadre()->getEntidad()); //static_cast<TTransform*> (TfCamera);
    TTransform * rot= static_cast<TTransform*> (TfCamera->getPadre()->getPadre()->getEntidad()); //static_cast<TTransform*> (TfCamera);

    SDL_GL_SwapWindow(m_window);

    //SDL_Event e;
    //while(SDL_PollEvent(&e)){
    //    myEventReceiver->OnEvent(&e);
    //}


    return malla;
}


/*Para usar el motor a parte *

#include "../include/display.h"

#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>
#endif
#include "../include/ttransform.h"


#include <iostream>

Display::Display(int width, int height, const std::string& title)
{
    malla=0;
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    m_glContext = SDL_GL_CreateContext(m_window);
    windowSurface = NULL;
     windowSurface = SDL_GetWindowSurface(m_window);
    fondo = NULL;
    fondo = IMG_Load("res/fondo.jpg");
    if(fondo == NULL)
        std::cout<<"ERROR CARGA IMG: "<<IMG_GetError()<<std::endl;

    GLenum status = glewInit();

    if(status != GLEW_OK){
    std::cerr<< "Glew ha fallado al iniciar"<< std::endl;
    }
    m_isClosed=false;

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}
Display::~Display()
{
    SDL_GL_DeleteContext(m_glContext);
    SDL_FreeSurface(fondo);
    SDL_DestroyWindow(m_window);
     fondo = NULL;
    m_window = NULL;
    SDL_QUIT;
}

void Display::Clear(float r, float g, float b, float a){
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
 }

bool Display::IsClosed(){
    return m_isClosed;
}

int Display::Update(TNodo* TfCamera){


    TTransform * tras= static_cast<TTransform*> (TfCamera->getPadre()->getEntidad()); //static_cast<TTransform*> (TfCamera);
    TTransform * rot= static_cast<TTransform*> (TfCamera->getPadre()->getPadre()->getEntidad()); //static_cast<TTransform*> (TfCamera);

    SDL_GL_SwapWindow(m_window);

    SDL_Event e;

    while(SDL_PollEvent(&e)){
        if(e.type == SDL_QUIT){
            m_isClosed = true;
        }else if(e.type == SDL_KEYDOWN){
            switch(e.key.keysym.sym){ // Controles de teclado, mover la camara con WASD y salir con ESC
                case SDLK_ESCAPE:
                     m_isClosed = true;
                     break;
                case SDLK_w:

                  tras->SetPos(tras->GetPos()+glm::vec3(0,1,0));
                    break;
                case SDLK_a:

                  tras->SetPos(tras->GetPos()+glm::vec3(-1,0,0));

                    tras->GetPos().x-1;
                    break;
                case SDLK_s:

                  tras->SetPos(tras->GetPos()+glm::vec3(0,-1,0));

                    tras->GetPos().y-1;
                    break;
                case SDLK_d:

                  tras->SetPos(tras->GetPos()+glm::vec3(1,0,0));

                    tras->GetPos().x+1;
                    break;
                    case SDLK_q:

                  tras->SetPos(tras->GetPos()+glm::vec3(0,0,1));

                    tras->GetPos().z+1;
                    break;
                    case SDLK_e:

                  tras->SetPos(tras->GetPos()+glm::vec3(0,0,-1));

                    tras->GetPos().z-1;
                    break;
                    case SDLK_r:

                  tras->SetRot(tras->GetRot()+glm::vec3(0,glm::radians(5.0),0));

                    break;
                    case SDLK_t:

                  tras->SetRot(tras->GetRot()+glm::vec3(glm::radians(5.0),0,0));
                    break;
                case SDLK_m:
                    malla=malla+1;
                    if(malla>=2) malla=0;
                default:
                    break;

            }
        }
    }


    return malla;
}
*/

