task: 
write a program that can recreate itself, essentially.

step:
0) 
write a program that reads a file and can turn the contents of that file in turn into a program that can return a number. for now just on windows.
	print out the number on the command line like so: yourprogram.exe arg1 arg2 & call echo %^ERRORLEVEL%. (minus the period at the end).
	use C in Visual Studio 2022. Windows empty c++ project, create main.c. 		

set to Release x64. Project name REDACTEDCreator (REDACT THIS). set language standard to C 17. 
	for the sake of simplicity use the console and the int main(int argc, char** argv) {} main function, 
	as opposed to a windows app and windows main function. include stdio.h. add a new filter called Documentation that will 
	contain this file README.md (all files simply stored in the root directory of the project).

the program expects ten additional arguments: the targetID (0 = windows, 1 = linux, 2 = mac), the maxLength of REDACTED_Page (REDACT THIS), 
	the maxLength of REDACTED_Line (REDACT THIS), the dataSize used (0 = u8, 1 = u32), the inputPathMaxLength, the inputPath, the outputPathMaxLength, 
	the outputPath, the outputFileNameMaxLength, and the outputFileName. store all of this in REDACTED_Program (REDACT THIS).

for text arguments, remember to enclose them in quotation marks.

if the number of arguments isn't equal to the above number + 1, the program exits immediately.

!0.001 test this part to see if this all works properly: UNTESTED.
