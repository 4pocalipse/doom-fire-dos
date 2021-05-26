#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <time.h>    /* For rand() */
#include "graphic.h" /* My custom graphics library using 13h VGA mode */

#define FIRE_HEIGHT  75
#define FIRE_WIDTH   321
#define TOTAL_PIXELS FIRE_HEIGHT*FIRE_WIDTH

static unsigned int FrameDelay = 25;

static unsigned int FirePalette[20] = {
	0, 			// 0 - Black
	111,    // 1 - Dark Red
	111,
	111,
	41,     // 2 - Dark Orange
	41,
	41,
	42,     // 3 - Orange
	42,
	42,
	43,     // 4 - Dark Yellow
	43,
	43,
	44,			// 5 - Yellow
	44,
	44,
	14,     // 6 - Light Yellow
	14,
	14,
	15      // 7 - White
};

static unsigned int PixelsArray[TOTAL_PIXELS];
static const unsigned Width = 320, Height = 200;

static void
CreateDataStructure()
{
	unsigned x;

	for (x = 0; x < TOTAL_PIXELS; x++)
		PixelsArray[x] = 0;
}

static void
ViewDataStructure()
{
	unsigned x;

	SetText();

	for (x = 0; x < TOTAL_PIXELS; x++) {
		printf("%d - %d\n", x, PixelsArray[x]);
	}

	getchar();
}

static void
UpdateCanvas()
{
	unsigned x, y, z;

	x = 0;
	y = 0;
	z = 0;

	while (1) {
		z++;

		if (x < FIRE_WIDTH-1) {
			if (y == FIRE_HEIGHT) {
				break;
			} else {
				PutPixel(x, y, FirePalette[PixelsArray[z] ] );
				x++;
			}
		} else {
			x = 0;
			y++;
		}
	}
}

static void
CreateFireSource()
{
	unsigned x;
	unsigned CanvasSize = TOTAL_PIXELS;
	unsigned LineSize = FIRE_WIDTH;

	for (x = CanvasSize - LineSize - 1; x < CanvasSize; x++)
		PixelsArray[x] = 19;
}

static void
UpdateFireIntensityPerPixel(unsigned int CurrentPixelIndex)
{
	unsigned int BelowPixelIndex = CurrentPixelIndex + FIRE_WIDTH;
	unsigned int BelowPixelIntensity, NewFireIntensity;
	unsigned int Decay;

	if (BelowPixelIndex >= TOTAL_PIXELS) {
		return;
	}

	BelowPixelIntensity = PixelsArray[BelowPixelIndex];

	if (BelowPixelIntensity == 0) {
		PixelsArray[CurrentPixelIndex] = 0;
		return;
	}

	Decay = rand() % 2;

	if (BelowPixelIntensity - Decay >= 0) {
		NewFireIntensity = BelowPixelIntensity - Decay;
	} else {
		NewFireIntensity = 0;
	}

	PixelsArray[CurrentPixelIndex] = NewFireIntensity;
}

static void
CalculatePropagation()
{
	unsigned x, y, PixelIndex;

	for (x = 0; x < FIRE_WIDTH; x++) {
		for (y = 0; y < FIRE_HEIGHT; y++) {
			PixelIndex = x + (FIRE_WIDTH * y);

			UpdateFireIntensityPerPixel(PixelIndex);
		}
	}

	UpdateCanvas();
}

static void
IncreaseFireSource()
{
	unsigned int Column;

	for (Column = 0; Column <= FIRE_WIDTH; Column++) {
		unsigned int PixelIndex = (TOTAL_PIXELS - FIRE_WIDTH) + Column;
		unsigned int CurrentFireIntensity = PixelsArray[PixelIndex];

		if (CurrentFireIntensity < 19) {
			unsigned int Increase = (unsigned int) rand() % 4;
			unsigned int NewFireIntensity;

			if (CurrentFireIntensity + Increase >= 19) {
				NewFireIntensity = 19;
			} else {
				NewFireIntensity += Increase;
			}

			PixelsArray[PixelIndex] = NewFireIntensity;
		}
	}
}

static void
DecreaseFireSource()
{
	unsigned int Column;

	for (Column = 0; Column <= FIRE_WIDTH; Column++) {
		unsigned int PixelIndex = (TOTAL_PIXELS - FIRE_WIDTH) + Column;
		unsigned int CurrentFireIntensity = PixelsArray[PixelIndex];
		unsigned int NewFireIntensity;

		if (CurrentFireIntensity > 0) {
			unsigned int Decay = rand() % 4;

			if (CurrentFireIntensity - Decay >= 0) {
				NewFireIntensity = 0;
			} else {
				NewFireIntensity = CurrentFireIntensity - Decay;
			}

			PixelsArray[PixelIndex] = NewFireIntensity;
		}
	}
}

static void
StartFire()
{
	char Key;

	while (!kbhit()) {
		CalculatePropagation();
		delay(FrameDelay);
	}

	Key = getch();

	if (Key == 'q' || Key == 'Q')
		return;
}

int
main(int argc, char **argv)
{
	unsigned x, y, c;
	char Key;

	srand(time(0)); // For Pseudo-random fire decay

	if (argv[1] == '-d')
		printf("DEBUG MODE ENABLED\n"
			     "------------------");

	printf("Press any key to START\n"
		   "While running, press Q to quit.");

	getchar();

	CreateDataStructure();
	CreateFireSource();
	if (argv[1] == '-d')
		ViewDataStructure();

	SetGraphic();

	StartFire();

	SetText();

	return EXIT_SUCCESS;
}
