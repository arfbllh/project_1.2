bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load dot texture
	success = success & gDotTexture.loadFromFile( "assets/dot.bmp" );
	//Load background texture
	success = success &	gBGTexture.loadFromFile( "assets/bg.png" );

	//success = success &	gFont = TTF_OpenFont("assets/fonts.ttf", 30);


	//menu meadia loading
	success = success & play_button.loadFromFile("assets/button_play.png");
	success = success & play_button_sh.loadFromFile("assets/button_play(2).png");
	success = success & setting_button.loadFromFile("assets/button_settings.png");
	success = success & setting_button_sh.loadFromFile("assets/button_settings(2).png");
	success = success &	help_button.loadFromFile("assets/button_help.png");
	success = success &	help_button_sh.loadFromFile("assets/button_help(2).png");
	success = success &	score_button.loadFromFile("assets/button_score.png");
	success = success &	score_button_sh.loadFromFile("assets/button_score(2).png");
	success = success &	quit_button.loadFromFile("assets/button_quit.png");
	success = success & quit_button_sh.loadFromFile("assets/button_quit(2).png");


	//music
	;
	success = success & ((gHelp = Mix_LoadMUS("assets/help.mp3")) != NULL);
	success = success & ((gClick = Mix_LoadWAV("assets/click.wav")) != NULL);


	//character
	success = success & pinkman[0].loadFromFile("assets/fly1.png");
	success = success & pinkman[1].loadFromFile("assets/fly2.png");
	success = success & pinkman[2].loadFromFile("assets/fly3.png");
	return success;
}
