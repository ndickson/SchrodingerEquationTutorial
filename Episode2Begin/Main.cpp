#include "Main.h"

// NOTE: To test windowed instead of fullscreen, change to 0 instead of SDL_FULLSCREEN
#define FULLSCREEN SDL_FULLSCREEN

// Give the pixels colours
void fillImage(uint* pixels,int width,int height) {
	size_t i = 0;
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			pixels[i] = 0xFFA020;
			++i;
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
