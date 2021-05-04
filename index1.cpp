#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include<stdio.h>
#include<string>
#include<sstream>

#define keypressed e.key.keysym.sym

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int scrollingOffset = 0;


//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );
		
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

//The dot that will move around on the screen
class Dot
{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		//Maximum axis velocity of the dot
		static const int DOT_VEL = 10;

		//Initializes the variables
		Dot();

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e );

		//Moves the dot
		bool move();

		//Shows the dot on the screen
		void render();

		//The X and Y offsets of the dot
		int mPosX, mPosY;

		//The velocity of the dot
		int mVelX, mVelY;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();


bool checkCollision( SDL_Rect a, SDL_Rect b );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gDotTexture;
LTexture gBGTexture;
LTexture gOverTexture;
LTexture gScoreTexture;
LTexture gPointTexture;
LTexture gFirstScreenTexture;
LTexture gPuasedTexture;
SDL_Rect wall_u[5];
SDL_Rect wall_d[5];
TTF_Font *gFont = NULL;

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	
	//Return success
	return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

Dot::Dot()
{
    //Initialize the offsets
    mPosX = SCREEN_WIDTH/2;
    mPosY = SCREEN_HEIGHT/2;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

void Dot::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( keypressed )
        {
            case SDLK_UP: mVelY -= DOT_VEL; break;
            case SDLK_DOWN: mVelY += DOT_VEL; break;
            case SDLK_LEFT: mVelX -= DOT_VEL; break;
            case SDLK_RIGHT: mVelX += DOT_VEL; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( keypressed )
        {
            case SDLK_UP: mVelY += DOT_VEL; break;
            case SDLK_DOWN: mVelY -= DOT_VEL; break;
            case SDLK_LEFT: mVelX += DOT_VEL; break;
            case SDLK_RIGHT: mVelX -= DOT_VEL; break;
        }
    }
}

bool Dot::move()
{
    //Move the dot left or right
    mPosX += mVelX;

    //scrollingOffset -= mVelX;

    SDL_Rect dott = {mPosX, mPosY, DOT_WIDTH, DOT_HEIGHT};

    //If the dot went too far to the left or right
    bool collided = 0;
    for(int i = 0; i < 5; i++){
    	collided |= checkCollision(dott, wall_u[i]);
    	collided |= checkCollision(dott, wall_d[i]);
    }
    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > SCREEN_WIDTH ) || collided)
    {
        //Move back
        mPosX -= mVelX;
    }

    //Move the dot up or down
    mPosY += mVelY;

    //If the dot went too far up or down
    if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > SCREEN_HEIGHT )  || collided)
    {
        //Move back
        mPosY -= mVelY;
    }
    return collided;
}

void Dot::render()
{
    //Show the dot
	gDotTexture.render( mPosX, mPosY );
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

				if(TTF_Init() == -1)
				{
					printf("SDL_TTF could not initialize.\n");
					printf("SDL_TTF Error: %s\n", TTF_GetError());
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load dot texture
	if( !gDotTexture.loadFromFile( "assets/dot.bmp" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

	//Load background texture
	if( !gBGTexture.loadFromFile( "assets/bg.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}

	gFont = TTF_OpenFont("assets/fonts.ttf", 30);

	if(gFont == NULL)
	{
		printf("TTF_Font could not be loaded.\n");
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded images
	gDotTexture.free();
	gBGTexture.free();
	gFirstScreenTexture.free();
	gScoreTexture.free();
	gPuasedTexture.free();


	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

bool checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;
			bool started = false;

			//Event handler
			SDL_Event e;
			int game_over = 0;
			int score = 0;
			int paused = 0;
			std::stringstream score_text;

			//The dot that will be moving around on the screen
			Dot dot;
			
			
			SDL_Rect point;
			int is_point = 0;
			int cnt = 0;

			point.x = 500;
			point.y = 250;


			for(int i = 0; i < 5; i++) {
				wall_d[i].x = wall_u[i].x = 150 + i*150;
				wall_d[i].y = SCREEN_HEIGHT - 100;
				wall_d[i].h = 100;
				wall_u[i].h = 100;
				wall_u[i].y = 0;
				wall_d[i].w = wall_u[i].w = 10;
			}
			

			//The background scrolling offset

			//While application is running

			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					if(e.type == SDL_KEYDOWN && e.key.repeat == 0)
					{
						if(keypressed == SDLK_s){
							started = 1;
							paused = 0;
						}
						else if(keypressed == SDLK_p)
						{
							paused = 1;
						}
					}

					//Handle input for the dot
					dot.handleEvent( e );
				}

				if(started == 0 && pause)
				{
					SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
					SDL_RenderClear( gRenderer );

					gFirstScreenTexture.loadFromRenderedText("press s to start/resume the game\n", {0, 255, 120});
					gFirstScreenTexture.render(SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 - 120);
					gPuasedTexture.loadFromRenderedText("press p to pause anytime\n", {0, 122, 122});
					gPuasedTexture.render(SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 - 90);
					SDL_RenderPresent(gRenderer);
					continue;
				}

				game_over = dot.move();

				--scrollingOffset;
				--dot.mPosX;
				--point.x;
				SDL_Rect dott = {dot.mPosX, dot.mPosY, dot.DOT_WIDTH, dot.DOT_HEIGHT};
				for(int i = 0; i < 5; i++){
					--wall_u[i].x;
					--wall_d[i].x;
					if(dot.mPosX < -dot.DOT_WIDTH) game_over = 1;

					if(wall_d[i].x < 0){
						srand(time(0));
						wall_d[i].x += SCREEN_WIDTH;
						wall_u[i].x += SCREEN_WIDTH;

						int w = (rand()%40 + 10);
						int h = (rand()%150 + 10);
						wall_d[i].y = SCREEN_HEIGHT - (SCREEN_HEIGHT - h - 200);
						wall_d[i].w = w;
						wall_d[i].h = (SCREEN_HEIGHT - h - 200);

						wall_u[i].y = 0;
						wall_u[i].w = w;
						wall_u[i].h = h;
					}
				}
				if( scrollingOffset < -gBGTexture.getWidth() )
				{
					scrollingOffset = 0;
				}

				if(point.x < -point.w)
				{
					point.x = dot.mPosX + rand()%50 + 50;
					point.y = 150 + rand()%50;
					point.h = 50 + rand()%10;
					point.w = 50 + rand()%10;
					is_point = 0;
					cnt = 0;
				}

			
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render background
				gBGTexture.render( scrollingOffset, 0 );
				gBGTexture.render( scrollingOffset + gBGTexture.getWidth(), 0 );

				//Render objects
				dot.render();
				
				for(int i = 0; i < 5; i++){
	

					//Update screen
					SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
					SDL_RenderFillRect(gRenderer, &wall_u[i]);
					SDL_RenderFillRect(gRenderer, &wall_d[i]);
					SDL_RenderDrawRect( gRenderer, &wall_u[i]);
					SDL_RenderDrawRect(gRenderer, &wall_d[i]);

				}

				//point count

				if(checkCollision(dott, point) && !is_point) {
					is_point = 1;
					score += 10;
				}
				else if(is_point && cnt < 50)
				{
					gPointTexture.loadFromRenderedText("+10", {0, 128, 0});
					gPointTexture.render(point.x, point.y + 20 + cnt);
					cnt++;

				}

				if(is_point == 0){
					SDL_SetRenderDrawColor(gRenderer, 0x0C, 0xA4, 0x94, 0x00);
					SDL_RenderFillRect(gRenderer, &point);
					SDL_RenderDrawRect(gRenderer, &point);
				}


				if(game_over)
				{

					gOverTexture.loadFromRenderedText("Game Over", {0, 0, 0});
					gOverTexture.render(SCREEN_WIDTH/2 - 10, SCREEN_HEIGHT/2 - 10);
					gScoreTexture.loadFromRenderedText(score_text.str().c_str(), {0, 0, 0});
					gScoreTexture.render(SCREEN_WIDTH - 200, 0);
					SDL_RenderPresent( gRenderer );
					SDL_Delay(1000);
					break;
				}

				if(dot.mPosX%10 == 0) score++;
				score_text.str("");
				score_text<<"score: "<<score;

				gScoreTexture.loadFromRenderedText(score_text.str().c_str(), {0, 0, 0});
				gScoreTexture.render(SCREEN_WIDTH - 200, 0);


				SDL_RenderPresent( gRenderer );
				here:;
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}