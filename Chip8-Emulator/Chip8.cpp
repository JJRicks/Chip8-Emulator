#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "Chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void Chip8::initialize() {
	pc = 0x200;
	opcode = 0;
	I = 0;
	sp = 0;
	for (int i = 0; i < 80; ++i) {
		memory[i] = chip8_fontset[i];
	}
}

void Chip8::loadGame(const char* name) {
	FILE* file;
	char* buffer;
	unsigned long fileLen;

	//Open file
	file = fopen("PONG", "rb");
	if (!file)
	{
		fprintf(stderr, "Unable to open file %s", name);
		return;
	}

	//Get file length
	fseek(file, 0, SEEK_END);
	fileLen = ftell(file);
	fseek(file, 0, SEEK_SET);

	//Allocate memory
	buffer = (char*)malloc(fileLen + 1);
	if (!buffer)
	{
		fprintf(stderr, "Memory error!");
		fclose(file);
		return;
	}

	//Read file contents into buffer
	fread(buffer, fileLen, 1, file);
	fclose(file);

	//Do what ever with buffer
	for (int i = 0; i < (int)fileLen; ++i) {
		memory[i + 512] = buffer[i];
	}
	free(buffer);
}

void Chip8::emulateCycle() {
	opcode = memory[pc] << 8 | memory[pc + 1];

	switch (opcode & 0xF000) {
	case 0xA000:
		I = opcode & 0x0FFF;
		pc += 2;
		break;
	case 0x2000:
		stack[sp] = pc;
		++sp;
		pc = opcode & 0x0FFF;
		break;
	case 0x8000:
		switch (opcode & 0x000F) {
		case 0x0004:
			if (V[(opcode & 0x00F0 >> 4)] > (0xFF - V[(opcode & 0x0F00) >> 8])) {
				V[0xF] = 1;
			}
			else {
				V[0xF] = 0;
			}
			V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
			break;
		default:
			printf("Unknown 0x8000 opcode: 0x%X\n", opcode);
			break;
		}
	break;
	case 0xF000:
		switch (opcode & 0x00FF) {
		case 0x0033:
			memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
			memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
			memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
			pc += 2;
			break;
		default:
			printf("Unknown 0xF000 opcode: 0x%X\n", opcode);
			break;
		}
		break;
		printf("Unknown opcode: 0x%X\n", opcode);
	}
	if (delay_timer > 0) {
		--delay_timer;
	}
	if (sound_timer > 0) {
		if (sound_timer == 1) {
			printf("BEEP!\n");
		}
		--sound_timer;
	}
}

void Chip8::setKeys() {
}