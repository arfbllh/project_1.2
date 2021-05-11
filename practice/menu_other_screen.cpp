#include "header.cpp"

bool on_menu = 1;
bool on_play = 0;
bool on_settig = 0;
bool on_help = 0;
bool on_quit = 0;

int button_animation = 7;
int animation_interval =  323;
int button_width = 323;
int button_height = 119;
int buttonx = 275;
int play_buttony = 375;
int setting_buttony = play_buttony + 100;
int score_buttony = play_buttony + 200;
int help_buttony = play_buttony + 300;
int quit_buttony = play_buttony + 400;

int play_shadow_index = 1;
int setting_shadow_index = 7;
int help_shadow_index = 7;
int score_shadow_index = 7;
int quit_shadow_index = 7;

bool is_menu_medialoaded = 0;


bool on_button(int mouse_x,int mouse_y, int button_x, int button_y, int button_width, int button_height)
{
	if(mouse_x >= button_x && mouse_y >= button_y && mouse_x <= button_x + button_width && mouse_y <= button_y + button_height) return 1;
	return 0;
}


void show_menu_otherscreen()
{
	int x, y;
	if(on_menu)
	{
		SDL_GetMouseState(&x, &y);
		if(on_button(x, y, buttonx, play_buttony, button_width, button_height))
		{
			SDL_Delay(25);
			play_shadow_index = 0;
			if(play_shadow_index < 0) play_shadow_index = 0;
			setting_shadow_index++;
			help_shadow_index++;
			score_shadow_index++;
			quit_shadow_index++;
			if(setting_shadow_index > 6) setting_shadow_index = 6;
			if(help_shadow_index > 6) help_shadow_index = 6;
			if(score_shadow_index > 6) score_shadow_index = 6;
			if(quit_shadow_index > 6) quit_shadow_index = 6;
		}/*
		else if(on_button(x, y, buttonx, setting_buttony, button_width, button_height)){
			SDL_Delay(25);
			setting_shadow_index--;
			if(setting_shadow_index < 0) setting_shadow_index = 0;
			play_shadow_index++;
			help_shadow_index++;
			score_shadow_index++;
			quit_shadow_index++;
			if(play_shadow_index > 6) play_shadow_index = 6;
			if(help_shadow_index > 6) help_shadow_index = 6;
			if(score_shadow_index > 6) score_shadow_index = 6;
			if(quit_shadow_index > 6) quit_shadow_index = 6;
		}
		else if(on_button(x, y, buttonx, help_buttony, button_width, button_height)){
			SDL_Delay(25);
			help_shadow_index--;
			if(help_shadow_index < 0) help_shadow_index = 0;
			setting_shadow_index++;
			help_shadow_index++;
			score_shadow_index++;
			quit_shadow_index++;
			if(setting_shadow_index > 6) setting_shadow_index = 6;
			if(play_shadow_index > 6) play_shadow_index = 6;
			if(score_shadow_index > 6) score_shadow_index = 6;
			if(quit_shadow_index > 6) quit_shadow_index = 6;
		}
		else if(on_button(x, y, buttonx, score_buttony, button_width, button_height)){
			SDL_Delay(25);
			score_shadow_index--;
			if(score_shadow_index < 0) score_shadow_index = 0;
			setting_shadow_index++;
			help_shadow_index++;
			play_shadow_index++;
			quit_shadow_index++;
			if(setting_shadow_index > 6) setting_shadow_index = 6;
			if(help_shadow_index > 6) help_shadow_index = 6;
			if(play_shadow_index > 6) play_shadow_index = 6;
			if(quit_shadow_index > 6) quit_shadow_index = 6;
		}
		else if(on_button(x, y, buttonx, play_buttony, button_width, button_height)){
			SDL_Delay(25);
			quit_shadow_index--;
			if(quit_shadow_index < 0) quit_shadow_index = 0;
			setting_shadow_index++;
			help_shadow_index++;
			score_shadow_index++;
			play_shadow_index++;
			if(setting_shadow_index > 6) setting_shadow_index = 6;
			if(help_shadow_index > 6) help_shadow_index = 6;
			if(score_shadow_index > 6) score_shadow_index = 6;
			if(play_shadow_index > 6) play_shadow_index = 6;
		}*/
		else
		{
			play_shadow_index = 1;
			setting_shadow_index++;
			help_shadow_index++;
			score_shadow_index++;
			play_shadow_index++;
			if(play_shadow_index > 6) play_shadow_index = 6;
			if(setting_shadow_index > 6) setting_shadow_index = 6;
			if(help_shadow_index > 6) help_shadow_index = 6;
			if(score_shadow_index > 6) score_shadow_index = 6;
			if(quit_shadow_index > 6) quit_shadow_index = 6;

		}

		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear(gRenderer);

		if(play_shadow_index)	play_button.render(buttonx, play_buttony);
		else play_button_sh.render(buttonx, play_buttony);
		setting_button.render(buttonx, setting_buttony);
		help_button.render(buttonx, help_buttony);
		score_button.render(buttonx, score_buttony);
		quit_button.render(buttonx, quit_buttony);
		printf("%d %d\n", x, y);


		SDL_RenderPresent(gRenderer);

	}
}

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
			if(on_menu) {
				show_menu_otherscreen();
			}
		}
	}
}