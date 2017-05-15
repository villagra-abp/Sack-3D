#include "LButton.h"
#include "Menu.h"
#include <iostream>
LButton::LButton()
{
    mPosition.x = 0;
    mPosition.y = 0;

    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition( int x, int y )
{
    mPosition.x = x;
    mPosition.y = y;
}

void LButton::handleEvent( SDL_Event* e )
{
    //If mouse event happened
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );
    //Check if mouse is in button
        bool inside = true;

        //Mouse is left of the button
        if( x < mPosition.x )
        {
            inside = false;
        }
        //Mouse is right of the button
        else if( x > mPosition.x + BUTTON_WIDTH )
        {
            inside = false;
        }
        //Mouse above the button
        else if( y < mPosition.y )
        {
            inside = false;
        }
        //Mouse below the button
        else if( y > mPosition.y + BUTTON_HEIGHT )
        {
            inside = false;
        }
     //Mouse is outside button
        if( !inside )
        {
            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        //Mouse is inside button
        else
        {
            //Set mouse over sprite
            switch( e->type )
            {
                case SDL_MOUSEMOTION:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                //std::cout<<"HE PULSADO"<<std::endl;
                break;

                case SDL_MOUSEBUTTONDOWN:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                std::cout<<"HE PULSADO"<<std::endl;
                break;

                case SDL_MOUSEBUTTONUP:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                std::cout<<"HE SOLTADO"<<std::endl;
                break;
            }
        }
        //std::cout<<"raton"<<"x"<<x<<" y"<<y<<std::endl;
       std::cout<<"state"<<mCurrentSprite<<std::endl;
    }
}

void LButton::render(LTexture gButtonSpriteSheetTexture, SDL_Renderer* gRenderer)
{
    //Show current button sprite
    /*std::cout<<"PINTA BUTTONS"<<std::endl;
    std::cout<<"X"<<mPosition.x<<std::endl;
    std::cout<<"Y"<<mPosition.y<<std::endl;*/

    gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, gRenderer);
}

LButton::~LButton()
{
    //dtor
}
