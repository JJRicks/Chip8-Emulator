#include "Chip8.h"
#include <iostream>
Chip8 myChip8;

int main(int argc, char** argv) {
	// setupGraphics();
	// setupInput();
	myChip8.initialize();
	myChip8.loadGame("pong");
	for (;;) { 
		myChip8.emulateCycle();
		if (myChip8.drawFlag) {
			//drawGraphics();
		}
		myChip8.setKeys();
	}
	return 0;
}

void setupGraphics() {
}

void setupInput() {
}

void drawGraphics() {
}
