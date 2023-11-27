#include <sys/types.h>	// This provides typedefs needed by libgte.h and libgpu.h
#include <stdio.h>	// Not necessary but include it anyway
#include <libetc.h>	// Includes some functions that controls the display
#include <libgte.h>	// GTE header, not really used but libgpu.h depends on it
#include <libgpu.h>	// GPU library header

// Define environment pairs and buffer counter
DISPENV disp[2];
DRAWENV draw[2];
int db;

void init(void)
{
	// Reset GPU and enable interrupts
	ResetGraph(0);

	// Configures the pair of DISPENVs for 320x240 mode (NTSC)
	SetDefDispEnv(&disp[0], 0, 0, 320, 240);
	SetDefDispEnv(&disp[1], 0, 240, 320, 240);

	// Configures the pair of DRAWENVs for the DISPENVs
	SetDefDrawEnv(&draw[0], 0, 240, 320, 240);
	SetDefDrawEnv(&draw[1], 0, 0, 320, 240);

	// Specifies the clear color of the DRAWENV
	setRGB0(&draw[0], 63, 0, 127);
	setRGB0(&draw[1], 63, 0, 127);

	// Enable background clear
	draw[0].isbg = 1;
	draw[1].isbg = 1;

	// Apply environments
	PutDispEnv(&disp[0]);
	PutDrawEnv(&draw[0]);

	// Make sure db starts with zero
	db = 0;

	// Load the internal font texture
	FntLoad(960, 0);
	// Create the text stream
	FntOpen(0, 8, 320, 224, 0, 100);
}

void display(void)
{
	// Wait for GPU to finish drawing and V-Blank
	DrawSync(0);
	VSync(0);

	// Flip buffer counter
	db = !db;

	// Apply environments
	PutDispEnv(&disp[db]);
	PutDrawEnv(&draw[db]);

	// Enable display
	SetDispMask(1);
}

int main()
{
	// Initialize graphics and stuff
	init();

	// Main loop
	while (1)
	{
		FntPrint("HELLO WORLD!");

		FntFlush(-1);
		display();
	}

	return 0;
}