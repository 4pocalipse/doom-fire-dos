static void SetGraphic() { _asm { mov ax, 0x13; int 0x10; } }
static void SetText() { _asm { mov ax, 0x03; int 0x10; } }

static unsigned char _far *VRAM = (unsigned char _far *) 0xA0009C41;
/* The VGA Video Memory starts at 0xA0000000, so, considering
that the Height of the fire is 75, to fit at the bottom of the
screen, it should start rendering at line 125 (200-75).

Size of the screen in bytes: 0xFA00 (64000);
Width of the screen: 				 0x140 (320);
Height of the fire canvas: 	 0x4B (75);
Extra byte to jump to start of the next line: 0x01;

0xA0000000 + 0xFA00 - 0x140 * 0x4B + 0x01 = 0xA0009C41*/

static void PutPixel(unsigned x, unsigned y, int c) { VRAM[y*320u+x] = c; }
