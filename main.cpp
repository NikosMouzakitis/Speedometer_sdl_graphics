#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define window_width 400
#define window_height 300
#define fps 60

using namespace std;

class Sprite {
	private:
		SDL_Surface *image;
		SDL_Rect rect;
		uint32_t usd_clr;
		int origin_x, origin_y;		
	public:
		Sprite(uint32_t color, int x, int y, int w = window_width, int h = window_height) {
						/*h - window height so it wont dissapear 
						 * when updating on new value when it 
						 * have some speed acquired already	*/		
			image = SDL_CreateRGBSurface(0,w,h,32,0,0,0,0);
			SDL_FillRect(image, NULL, color);
			rect = image->clip_rect;
	
			origin_x = origin_y = 0;
			usd_clr = color;
			rect.x = x-origin_x;
			rect.y = y-origin_y;
		}	
	
		void speed_up(SDL_Surface *dest, int vel) {
			rect.y = vel;
		//	cout << rect.x << endl;
			draw(dest);
		}
		void speed_down(SDL_Surface *dest, int vel) {
			rect.y = vel;
			draw(dest);
		}
		void relance(SDL_Surface *dest, int vel) {
			rect.y = window_height - vel;	
			draw(dest);
		}
		void draw(SDL_Surface *dest) {
		//	cout << rect.x << endl;
			SDL_BlitSurface(image, NULL, dest, &rect);
		}
};


void cfr(uint32_t start_tick) {
	if( (1000/fps) > SDL_GetTicks() - start_tick) 
		SDL_Delay( 1000/fps - (SDL_GetTicks() - start_tick));
}

int main()
{
	SDL_Window *window = NULL;
	SDL_Surface *screen = NULL;
	SDL_Event event;
	bool running = true;
	uint32_t starting_tick;
	unsigned int vel = 0;	// vehicle's speed.
	int speed_change = 0;

	window = SDL_CreateWindow("Speedometer Testbench v1",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_OPENGL);

	if(window == NULL) 
		cout << "Error creating window: " << SDL_GetError() << endl;

	screen = SDL_GetWindowSurface(window);
	
	if(screen == NULL) 
		cout << "Error getting window surface :  " << SDL_GetError() << endl;
	
	uint32_t white = SDL_MapRGB(screen->format, 255, 255, 255);
	uint32_t red = SDL_MapRGB(screen->format, 255, 0, 0);
	int rv; // used as a return value
	rv = SDL_FillRect(screen, NULL, white);
	
	if(rv != 0) 
		cout << "Error: SDL_FillRect: " << SDL_GetError() << endl;
		
	Sprite speedometer(red, 20 , window_height);
	speedometer.draw(screen);

	SDL_UpdateWindowSurface(window);

	while(running) {
	
		starting_tick = SDL_GetTicks();

		while(SDL_PollEvent(&event)) {
		
			switch(event.type) {

				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_UP:
							if (vel < window_height - 30)
								vel +=5;
							
						//	SDL_FillRect(screen, NULL, white);
							speedometer.speed_up(screen,vel);	
							speed_change++;	
							break;
						case SDLK_DOWN:
							if(vel >= 10)	
								vel -= 10;
							// clear the previous drawn speed, and display the new value.

							SDL_FillRect(screen, NULL, white);
							speedometer.speed_down(screen, vel);	
							speed_change++;	
							break;
						default:
							break;
					}
				default:
					break;	
			}
		}
		// decrease of speed when no accelerating or breaking	
		if( (vel > 0) && (!speed_change)) {
			vel--;	
			SDL_FillRect(screen, NULL, white);
			speedometer.relance(screen, vel);
		}
		cout << "speed: " << vel << endl;
		SDL_UpdateWindowSurface(window);
		cfr(starting_tick);
		speed_change = 0;
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
	return (0);
}
