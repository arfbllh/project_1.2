#include "header.cpp"

bool on_menu = 1;
bool on_play = 0;
bool on_settig = 0;
bool on_help = 0;
bool on_score = 0;
bool on_quit = 0;

int button_animation = 7;
int button_interval = 80;
int button_width = 323;
int button_height = 119;
int buttonx = 478;
int play_buttony = 300;
int setting_buttony = play_buttony + button_interval;
int score_buttony = play_buttony + button_interval*2;
int help_buttony = play_buttony + button_interval*3;
int quit_buttony = play_buttony + button_interval*4;

int play_shadow = 0;
int setting_shadow = 0;
int help_shadow = 0;
int score_shadow = 0;
int quit_shadow = 0;

bool is_menu_medialoaded = 0;


bool on_button(int mouse_x,int mouse_y, int button_x, int button_y, int button_width, int button_height)
{
	if(mouse_x >= button_x && mouse_y >= button_y && mouse_x <= button_x + button_width && mouse_y <= button_y + button_height) return 1;
	return 0;
}


void show_menu_otherscreen(SDL_Event e)
{
	int x, y;
	if(on_menu)
	{
		SDL_GetMouseState(&x, &y);
		if(on_button(x, y, buttonx, play_buttony, button_width, button_height))
		{
			SDL_Delay(25);
			play_shadow = 1;
			setting_shadow = 0;
			help_shadow = 0;
			score_shadow = 0;
			quit_shadow = 0;
			if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP){
				on_play = 1;
				on_settig = 0;
				on_help = 0;
				on_score = 0;
				on_quit = 0;
				on_menu = 0;
			}

		}
		else if(on_button(x, y, buttonx, setting_buttony, button_width, button_height)){
			SDL_Delay(25);
			play_shadow = 0;
			setting_shadow = 1;
			help_shadow = 0;
			score_shadow = 0;
			quit_shadow = 0;
			if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP){
				on_play = 0;
				on_settig = 1;
				on_help = 0;
				on_score = 0;
				on_quit = 0;
				on_menu = 0;
			}
		}
		else if(on_button(x, y, buttonx, help_buttony, button_width, button_height)){
			SDL_Delay(25);
			play_shadow = 0;
			setting_shadow = 0;
			help_shadow = 1;
			score_shadow = 0;
			quit_shadow = 0;		
			if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP){
				on_play = 0;
				on_settig = 0;
				on_help = 1;
				on_score = 0;
				on_quit = 0;
				on_menu = 0;
			}
		}
		else if(on_button(x, y, buttonx, score_buttony, button_width, button_height)){
			SDL_Delay(25);
			play_shadow = 0;
			setting_shadow = 0;
			help_shadow = 0;
			score_shadow = 1;
			quit_shadow = 0;	

			if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP){
				on_play = 0;
				on_settig = 0;
				on_help = 0;
				on_score = 1;
				on_quit = 0;
				on_menu = 0;
			}	
		}
		else if(on_button(x, y, buttonx, quit_buttony, button_width, button_height)){
			SDL_Delay(25);
			play_shadow = 0;
			setting_shadow = 0;
			help_shadow = 0;
			score_shadow = 0;
			quit_shadow = 1;

			if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP){
				on_play = 0;
				on_settig = 0;
				on_help = 0;
				on_score = 0;
				on_quit = 1;
				on_menu = 0;
			}
		}
		else
		{
			play_shadow = 0;
			setting_shadow = 0;
			help_shadow = 0;
			score_shadow = 0;
			quit_shadow = 0;

			if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP){
				on_play = 1;
				on_settig = 0;
				on_help = 0;
				on_score = 0;
				on_quit = 0;
			}

		}

		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear(gRenderer);

		if(!play_shadow) play_button.render(buttonx, play_buttony);
		else play_button_sh.render(buttonx, play_buttony);

		if(!setting_shadow) setting_button.render(buttonx, setting_buttony);
		else setting_button_sh.render(buttonx, setting_buttony);

		if(!help_shadow) help_button.render(buttonx, help_buttony);
		else help_button_sh.render(buttonx, help_buttony);

		if(!score_shadow) score_button.render(buttonx, score_buttony);
		else score_button_sh.render(buttonx, score_buttony);

		if(!quit_shadow) quit_button.render(buttonx, quit_buttony);
		else quit_button_sh.render(buttonx, quit_buttony);
		//printf("%d %d\n", x, y);


		SDL_RenderPresent(gRenderer);

	}
}
/*
int main()
{
	if(!init()) return 0;
	if(!loadMedia()) return 0;

	SDL_Event e;
	bool quit = false;
	while(!quit)
	{
		while( SDL_PollEvent( &e ) != 0 )
		{
			if( e.type == SDL_QUIT )
			{
				quit = true;
				break;
			}
		}

		if(on_menu) {
				show_menu_otherscreen(e);
				continue;
		}

		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear(gRenderer);

		SDL_RenderPresent(gRenderer);

	}
}*/