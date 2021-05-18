#include "header.cpp"
#include "loadMedia.cpp"
//#include "button.cpp"

int Win_H, Win_W;
int W_rat;
LTexture play_btn;

int on_game, on_setting, on_help, on_about;


bool on_button(int mouse_x,int mouse_y, int btn_x, int btn_y, int btn_width, int btn_height)
{
	if(mouse_x >= btn_x && mouse_y >= btn_y && mouse_x <= btn_x + btn_width && mouse_y <= btn_y + btn_height) return 1;
	return 0;
}


int main()
{
	init();


	SDL_Event e;

	bool quit = false;

	LTexture btn_shadow;

	if(!play_btn.loadFromFile("assets/play_btn.png")) printf("Error\n");
	if(!btn_shadow.loadFromFile("assets/btn_shadow_sprite.png")) printf("Error\n");

	SDL_Rect btn_shadow_array[7];
	int px = 0, py = 0, ph = 119, pw = 363, inter = 363;
	for(int i = 0; i < 7; i++){
		btn_shadow_array[i].x = px;
		btn_shadow_array[i].y = py;
		btn_shadow_array[i].h = ph;
		btn_shadow_array[i].w = pw;
		px += inter;
	}
	int mouse_x, mouse_y;
	int play_btnx = 450, play_btny = 300, btn_height = play_btn.getHeight(), btn_width = play_btn.getWidth();
	bool on_play = false;
	int pf = 6;
	while(!quit)
	{
		SDL_GetWindowSize(gWindow, &Win_W, &Win_H);
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
			if( e.type == SDL_QUIT )
			{
				quit = true;
				break;
			}
			SDL_GetMouseState(&mouse_x, &mouse_y);
			if(on_button(mouse_x, mouse_y, play_btnx, play_btny, btn_width, btn_height)){
				if(e.type == SDL_MOUSEMOTION){
					//printf("on_button\n");
					SDL_Delay(25);
					pf--;
					if(pf < 0) pf = 6;
				}
				else if(e.type == SDL_MOUSEBUTTONDOWN){
					printf("button Down\n");
					pf = 0;
				}
				else if(e.type == SDL_MOUSEBUTTONUP){
					printf("button up");
					pf = 0;
				}
			}
			else {
				pf++;
				if(pf > 6) pf = 6;
			}
		}


		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear(gRenderer);



		play_btn.render(play_btnx, play_btny);
		btn_shadow.render(play_btnx, play_btny, btn_shadow_array + pf);
		//cout<<pf<<endl;
		//printf("%d %d\n", mouse_x, mouse_y);
		


		SDL_RenderPresent(gRenderer);
	}
	//printf("%d\n", on_button(0, 0, 0, 0, 1000, 10000));
	//SDL_Delay(500);
}