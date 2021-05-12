#include "menu_other_screen.cpp"


int main()
{
	if(!init()) return 0;
	if(!loadMedia()) return 0;

	SDL_Event e;
	bool quit = false;
	while(!on_quit)
	{
		while( SDL_PollEvent( &e ) != 0 )
		{
			if( e.type == SDL_QUIT )
			{
				on_quit = true;
				break;
			}
		}

		if(!on_play) {
				show_menu_otherscreen(e);
				continue;
		}
		else{

			SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			SDL_RenderClear(gRenderer);
			SDL_RenderPresent(gRenderer);
		}

	}

	Mix_PlayChannel(-1, gClick, 0);
	SDL_Delay(50);
	close();
}