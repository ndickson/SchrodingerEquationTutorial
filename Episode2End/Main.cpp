#include "Main.h"

// NOTE: To test windowed instead of fullscreen, change to 0 instead of SDL_FULLSCREEN
#define FULLSCREEN SDL_FULLSCREEN

// Get a colour in byte order blue, green, red, (empty), from a magnitude (brightness) and phase angle (hue)
uint colourFromMagnitudeAndPhase(double magnitude,double phase) {
	// Our maximum displayable magnitude is 1, so clamp it to be at most 1.
	if (magnitude > 1) {
		magnitude = 1;
	}
	// In case phase is input in the range -tau/2 to tau/2, add tau to negative phases, so that they are from 0 to tau.
	if (phase < 0) {
		phase += M_TAU;
	}
	// Scale the phase from 0-tau to 0-6
	phase *= 6/M_TAU;
	// Determine which of the 6 colour ranges this phase is in
	int range = correctFloor(phase);
	// Avoid problem of round-off unintentionally resulting in range of -1 or 6
	range = min(5,max(0,range));
	// The fractional part of the range is just the original value minus the integer part
	double fraction = phase - range;

	// Compute the colour components based on the range and the fractional part
	double red;
	double green;
	double blue;
	switch (range) {
		case 0: red = 1.0;          green = fraction;     blue = 0.0;          break; // Red -> Yellow
		case 1: red = 1.0-fraction; green = 1.0;          blue = 0.0;          break; // Yellow -> Green
		case 2: red = 0.0;          green = 1.0;          blue = fraction;     break; // Green -> Cyan
		case 3: red = 0.0;          green = 1.0-fraction; blue = 1.0;          break; // Cyan -> Blue
		case 4: red = fraction;     green = 0.0;          blue = 1.0;          break; // Blue -> Magenta
		case 5: red = 1.0;          green = 0.0;          blue = 1.0-fraction; break; // Magenta -> Red
	}

	// Scale the colour components by the magnitude
	red *= magnitude;
	green *= magnitude;
	blue *= magnitude;

	// Convert the colour components from doubles in the range 0-1 to uints in the range 0-255
	uint intRed = correctRound(red*0xFF);
	uint intGreen = correctRound(green*0xFF);
	uint intBlue = correctRound(blue*0xFF);

	// Put the colours together in byte order blue, green, red, (empty)
	return (intRed << 16) | (intGreen << 8) | intBlue;
}


// Give the pixels colours
void fillImage(uint* pixels,int width,int height) {
	size_t i = 0;
	for (int y = 0; y < height; ++y) {
		// Initial value of 1
		Complex c(1.0,0.0);
		// Phase rotation of 1/400 of a cycle for each pixel across,
		// so one full cycle will be 400 pixels
		Complex r = Complex::fromMagnitudeAndPhase(1.0,M_TAU/400);
		for (int x = 0; x < width; ++x) {
			// Get the colour
			uint colour = colourFromMagnitudeAndPhase(c.magnitude(),c.phase());
			pixels[i] = colour;
			++i;
			// Rotate by multiplying
			c *= r;
		}
	}
}

// Main function, where the action begins
int main(int argc,char* argv[]) {
	// Initialize SDL
	int initFailed = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	if (initFailed!=0) {
		printf("SDL couldn't start. That really sucks. :(\n");
		fflush(stdout);
		exit(1);
	}

	// Default width and height in case unrestricted resolution (e.g. not fullscreen)
	int width = 800;
	int height = 450;

	// Check what screen resolutions are available
	SDL_Rect** modes = SDL_ListModes(NULL, FULLSCREEN|SDL_HWSURFACE|SDL_DOUBLEBUF);
	if (modes==(SDL_Rect**)0) {
		printf("No screen resolutions are available? WTF?\n");
		fflush(stdout);
		exit(1);
	}
	else if (modes==(SDL_Rect**)-1) {
		// No restrictions, so use the default
		printf("No restrictions on screen resolution: using default.\n");
		fflush(stdout);
	}
	else {
		printf("Available screen resolutions:\n");
		for(int i = 0; modes[i]; ++i) {
			if (modes[i]) {
				printf("    %d x %d\n",modes[i]->w,modes[i]->h);
			}
			else {
				printf("    NULL?\n");
			}
		}
		fflush(stdout);
		// Pick the first one.  Feel free to change this if it's not the preferred resolution.
		if (modes[0]) {
			width = modes[0]->w;
			height = modes[0]->h;
		}
		else {
			printf("The first one is NULL?\n");
			fflush(stdout);
			exit(1);
		}
	}
	// Make a window with the selected size and double-buffering (to avoid flicker as we write images to it)
	SDL_Surface* screen = SDL_SetVideoMode(width,height,0,FULLSCREEN|SDL_HWSURFACE|SDL_DOUBLEBUF);
	if (!screen)
	{
		printf("SDL couldn't create the window.  Epic fail!\n");
		fflush(stdout);
		exit(1);
	}
	SDL_WM_SetCaption("Schrodinger",0);

	// Allocate an array to store pixel colours
	uint* pixels = new uint[width*height];
	// Make an SDL_Surface for that array of pixel colours so that SDL can use it
	// NOTE: Having an alpha channel wouldn't make the window transparent, but would allow transparency in what you draw on top of an existing image.
	SDL_Surface* buffer = SDL_CreateRGBSurfaceFrom(pixels,width,height,32,width*4,0x00FF0000,0x0000FF00,0x000000FF,0);
	if (!buffer)
	{
		printf("SDL couldn't create the image buffer.  Oh noes!\n");
		fflush(stdout);
		exit(1);
	}

	// Make all of the pixels black to start
	for (int i = 0; i < width*height; ++i) {
		pixels[i] = 0x000000;
	}

	// Draw something else on top
	fillImage(pixels,width,height);

	// Copy the pixel array to the screen buffer
	int blitFailed = SDL_BlitSurface(buffer,NULL,screen,NULL);
	if (blitFailed!=0) {
		printf("SDL couldn't blit the image buffer.  Egad!\n");
		fflush(stdout);
		exit(1);
	}
	// Display the screen buffer (It's double-buffered, so we need to flip to the buffer we just wrote)
	int flipFailed = SDL_Flip(screen);
	if (flipFailed!=0) {
		printf("SDL couldn't flip the screen buffer.  Alas!\n");
		fflush(stdout);
		exit(1);
	}

	// Wait for events
	bool active = true;
	while (active) {
		SDL_Event event;
		if (SDL_WaitEvent(&event)) {
			if (event.type==SDL_QUIT) {
				active = false;
			}
			else if (event.type==SDL_KEYDOWN) {
				// Exit on keypress
				active = false;
			}
		}
	}

	// Uninitialize SDL
	SDL_Quit();

	// End the program
	return 0;
}
