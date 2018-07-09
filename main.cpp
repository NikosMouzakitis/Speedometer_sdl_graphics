#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#define window_width 100
#define window_height 500
#define fps 110

#define g2_barrier	(window_height-20)/3
#define g3_barrier	(window_height - 20)/4

using namespace std;

int gear;
int rpm_value;
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
			SDL_BlitSurface(image, NULL, dest, &rect);
		}
};


void cfr(uint32_t start_tick) {

	if( (1000/fps) > SDL_GetTicks() - start_tick) 

		SDL_Delay( 1000/fps - (SDL_GetTicks() - start_tick));
}

void update_speedometer(SDL_Renderer *renderer, int vel)
{


		SDL_RenderClear(renderer);
		int radius = 170;

		SDL_Event ev;
		SDL_SetRenderDrawColor(renderer, 0,110,150, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer,255,0,0,SDL_ALPHA_OPAQUE);
		
		int cx = window_width*2;
		int cy = window_height/2;
		
		for( float angle = 0.0; angle < 1000; angle++)
		{
			int x = cx -radius * cos(angle);
			int y = cy - radius *sin(angle);
			SDL_RenderDrawPoint(renderer, x,y);	
		}



		int x = cx - radius*cos((double)vel/180);
		int y = cy - radius*sin((double)vel/180);

		SDL_RenderDrawLine(renderer, window_width*2, window_height/2, x,y);	
		SDL_RenderPresent(renderer);
	
}

int main()
{
	SDL_Window *window = NULL;
	SDL_Window *window2 = NULL;
	SDL_Surface *screen = NULL;
	SDL_Event event;
	bool running = true;
	uint32_t starting_tick;
	unsigned int vel = 0;	// vehicle's speed.
	int speed_change = 0;
	int speed_limit = window_height - 20;
	window = SDL_CreateWindow("Speedometer",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_OPENGL);

	if(window == NULL) 
		cout << "Error creating window: " << SDL_GetError() << endl;

	if(window2 == NULL) 
		cout << "Error creating window: " << SDL_GetError() << endl;


	SDL_Renderer * renderer = NULL;

	/*	Creation of container for speedometer 	*/
	if(SDL_CreateWindowAndRenderer(4*window_width, window_height/2,SDL_WINDOW_ALWAYS_ON_TOP, &window2, &renderer) == 0)
	{
		SDL_SetWindowTitle(window2, "Speedometer");
		SDL_RenderClear(renderer);
		int radius = 170;

		SDL_Event ev;
		//SDL_SetRenderDrawColor(renderer, 0,150,150, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
		//SDL_SetRenderDrawColor(renderer,0,0,230,SDL_ALPHA_OPAQUE);
		
		int cx = window_width*2;
		int cy = window_height/2;
		
		for( float angle = 0.0; angle < 1000; angle++)
		{
			int x = cx -radius * cos(angle);
			int y = cy - radius *sin(angle);
			SDL_RenderDrawPoint(renderer, x,y);	
		}


		int x = cx - radius*cos(20);
		int y = cy - radius*sin(20);

		SDL_RenderDrawLine(renderer, window_width*2, window_height/2, x,y);	
		SDL_RenderPresent(renderer);
	
	
	}	

	screen = SDL_GetWindowSurface(window);
	
	if(screen == NULL) 
		cout << "Error getting window surface :  " << SDL_GetError() << endl;
	
	
	uint32_t white = SDL_MapRGB(screen->format, 255, 255, 255);
	uint32_t red = SDL_MapRGB(screen->format, 255, 0, 0);
	uint32_t black = SDL_MapRGB(screen->format, 0, 0, 0);
	int rv; 

	rv = SDL_FillRect(screen, NULL, white);

	if( (rv != 0) ) 
		cout << "Error: SDL_FillRect: " << SDL_GetError() << endl;
	
	Sprite rpm(white, 0 ,window_height/2);

	Sprite speedometer(red, 0 , window_height);
	speedometer.draw(screen);

	SDL_UpdateWindowSurface(window);
	SDL_UpdateWindowSurface(window2);

	gear = 0;

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
							if(!gear )
								break;
							if (vel < speed_limit) {

								switch(gear) {
									case 1:
										
										vel +=5;
										if(vel >= (speed_limit/3) ) {
											vel = speed_limit/3;
											cout << "Reached gear1 barrier" << endl;	
										}
										break;
									case 2:
										if(vel <= speed_limit/7) {
											cout<<"down a gear" << endl;
											//continue;
											break;
										}

										vel +=10;
											
										if(vel >= ( (speed_limit/2)+50 )) {
											cout << "Reached gear2 barrier" << endl;	
											vel = (speed_limit/2)+50 ;	
										}
										break;
									case 3: 
										if(vel <= (g2_barrier) ) {
											cout<<"down a gear" << endl;
											//continue;
											break;
										}
										
										if(vel <= g3_barrier + 10) {
											cout << "Reached gear3 barrier" << endl;	
											//continue;	
											break;
										}
										vel +=15;
										break;
								}	

							}	
							speedometer.speed_up(screen,vel);	
							SDL_FillRect(screen, NULL, white);
							speed_change++;	
							break;
						
						case SDLK_LEFT:
							gear -=1;

							if(gear <= 0)
								gear = 0;
							break;	
					
						case SDLK_RIGHT:
							gear +=1;

							if(gear >=3)
								gear = 3;	
							break;

						case SDLK_DOWN:
							
							if(!gear)
								break;	
							if( vel == 0)
								break;	
							
							if(vel >= 10)	
								vel -= 10;
							// clear the previous drawn speed, and display the new value.

							speedometer.speed_down(screen, vel);	
							speed_change++;	
							SDL_FillRect(screen, NULL, white);
							break;

						default:
							break;
					}
				default:
					break;	
			}
		}
		// decrease of speed when no accelerating or breaking	
		
		rpm_value = vel * 10 + 1;

	



		if( (vel > 0) && (!speed_change)) {
			vel--;	
			SDL_FillRect(screen, NULL, white);
			speedometer.relance(screen, vel);
		}
	
		
		
		cout << "speed: " << vel << "  ";
		cout << "gear: " << gear << " ";
		cout << "rpm : " <<rpm_value << endl;
		SDL_UpdateWindowSurface(window);
		update_speedometer(renderer,  vel);	


		cfr(starting_tick);
		speed_change = 0;
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
	return (0);
}
