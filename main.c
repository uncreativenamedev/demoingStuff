#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

enum InstructionGroupType0 {
	InstructionGroupType0_undefined,
	InstructionGroupType0_exitFile,
	InstructionGroupType0_preprocessorDirective
};

enum InstructionGroupType1 {
	InstructionGroupType1_undefined,
	InstructionGroupType1_define,
	InstructionGroupType1_setting
};

enum InstructionType {
	InstructionType_undefined
};

struct LineText {
	uint8_t instructionGroupTypeText[2]; //see README.md
	uint8_t instructionTypeText[1024]; //the type of the instruction. see README.md
	uint8_t argumentLengthText; //the number of arguments, max 9
	uint8_t argumentTypeLocationIndexText[1024]; //0: argument type is local to the program, other: found in the includedType file
	uint8_t argumentTypeText[9][1024]; //the type of the argument
	uint8_t argumentLocationIndexText[1024]; //0: block scope, 1: function scope, 2: program scope, other: found in the includedVariable file
	uint8_t argumentText[9][1024]; //argument can be a variable or a literal
};

union ArgumentBinary {
	uint8_t u8;
	uint16_t u16;
	uint32_t u32;
	uint64_t u64;
};

struct LineBinaryIntermediate {
	uint64_t instructionTypeIndex; //the typeIndex of the instruction. see README.md
	uint64_t argumentTypeLocationIndex; //0 means the argument type is local to the program, the other indices are found in the includedType file
	uint64_t argumentTypeIndex[9]; //the typeIndex of the argument, within the list of types included from the location
	uint64_t argumentLocationIndex[9]; //0: block scope, 1: function scope, 2: program scope, other: found in the includedVariable file
	union ArgumentBinary argumentBinary[9]; //arguments can be a number of things. eg. literal, function 
	uint8_t instructionGroupTypeIndex[2]; //see README.md
	uint8_t argumentLength; //the number of arguments, max 9
};

uint8_t getLineBinaryIntermediateinstructionGroupTypeIndex0(uint8_t input) {
	uint8_t returnValue = InstructionGroupType0_undefined;
	if (input == (uint8_t)'~')
		returnValue = InstructionGroupType0_exitFile;
	else if (input == (uint8_t)'#')
		returnValue = InstructionGroupType0_preprocessorDirective;
	return returnValue;
}

uint8_t getLineBinaryIntermediateinstructionGroupTypeIndex1(uint8_t input) {
	uint8_t returnValue = InstructionGroupType1_undefined;
	if (input == (uint8_t)'d')
		returnValue = InstructionGroupType1_define;
	else if (input == (uint8_t)'s')
		returnValue = InstructionGroupType1_setting;
	return returnValue;
}

void readVariableLengthText(FILE* fileRead, uint8_t* destination) {
	uint64_t currentIndex = UINT64_C(0);
	destination[0] = UINT8_C(0);
	while (destination[currentIndex] != (uint8_t)'>' && currentIndex < UINT64_C(1024)) { //> is the exit indicator for text
		fread((void*)&destination[currentIndex], sizeof(uint8_t), 1, fileRead);
	}
}

int main(int argc, char** argv) {
	struct LineText* lineText = malloc(sizeof(struct LineText));
	if (lineText == NULL)
		return 0;
	lineText->instructionGroupTypeText[0] = UINT8_C(0);
	struct LineBinaryIntermediate lineBinaryIntermediate;
	FILE* fileRead = fopen(argv[1], "rb");
	FILE* fileWrite = fopen(argv[2], "wb");
	while (lineText->instructionGroupTypeText[0] != ((uint8_t)'~')) { //~ as instructionGroupTypeText[0] marks the end of execution for the file being written in
		fread((void*)&lineText->instructionGroupTypeText[0], sizeof(uint8_t), 1, fileRead);
		lineBinaryIntermediate.instructionGroupTypeIndex[0] = getLineBinaryIntermediateinstructionGroupTypeIndex0(lineText->instructionGroupTypeText[0]);
		fread((void*)&lineText->instructionGroupTypeText[1], sizeof(uint8_t), 1, fileRead);
		lineBinaryIntermediate.instructionGroupTypeIndex[1] = getLineBinaryIntermediateinstructionGroupTypeIndex1(lineText->instructionGroupTypeText[1]);
		readVariableLengthText(fileRead, lineText->instructionTypeText);
	}
	free(lineText);
	return 0;
}
