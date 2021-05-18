#include "menu_other_screen.cpp"
#include "loadMedia.cpp"

int main()
{
	if(!init()) return 0;
	if(!loadMedia()) return 0;

	SDL_Event e;
	bool quit = false;
	int screen = 0;
	SDL_Rect fly = {0, 0, 10, 25};
	while(!on_quit)
	{
		while( SDL_PollEvent( &e ) != 0 )
		{
			if( e.type == SDL_QUIT )
			{
				on_quit = true;
				break;
			}
			pinkman_handleEvent(e);
		}

		if(!on_play) {
				show_menu_otherscreen(e);
				continue;
		}
		else{
			pinkman_move();
			printf("%d %d\n", pinkmanx, pinkmany);
			SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			SDL_RenderClear(gRenderer);


			pinkman[screen/15].render(pinkmanx, pinkmany);
			SDL_RenderPresent(gRenderer);


			screen++;
			if(screen >= 44) screen = 1;
		}

	}

	Mix_PlayChannel(-1, gClick, 0);
	SDL_Delay(50);
	close();
}