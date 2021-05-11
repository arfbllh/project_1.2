const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

class LButton
{
	public:
		LButton();
		void setPosition(int x, int y);
		void handleEvent(SDL_Event* e);
		void render();

	private:
		SDL_Point mPosition;
		int mCurrentSprite;
};

LButton play_btn;
LButton quit_btn;

LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;
	mCurrentSprite = 0;
}

void LButton::setPosition(int x, int y)
{
	mPosition = {x, y};
}

void LButton::handleEvent(SDL_Event* e)
{
	 if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
    {
        int x, y;
        SDL_GetMouseState( &x, &y );
        bool inside = true;

        if( x < mPosition.x )
        {
            inside = false;
        }
        else if( x > mPosition.x + BUTTON_WIDTH )
        {
            inside = false;
        }
        else if( y < mPosition.y )
        {
            inside = false;
        }
        else if( y > mPosition.y + BUTTON_HEIGHT )
        {
            inside = false;
        }
        if( !inside )
        {
            mCurrentSprite = 4;
        }
        else
        {
            switch( e->type )
            {
                case SDL_MOUSEMOTION:
                mCurrentSprite = 1;
                break;
            
                case SDL_MOUSEBUTTONDOWN:
                mCurrentSprite = 2;
                break;
                
                case SDL_MOUSEBUTTONUP:
                mCurrentSprite = 3;
                break;
            }
        }
    }

}