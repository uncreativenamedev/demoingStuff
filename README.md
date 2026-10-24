overview:
language needs to be able to:
- interact directly with the Winapi, ...
- support multiple operating systems (windows, linux, mac, ...).
- support c features like preprocessor defines, preprocessor if, structs, ...
- write directly to machine code.
- be as easy to parse as possible. every line has exactly the same construction (using escape characters for variable-length fields).

to do:
write an example program in several separate files: setting.r, includeType.r, includeVariable.r, and main.r.
this is important so I know exactly what I need and I'm not overlooking anything or overthinking....

goal: this seed program is used to rewrite itself in the new programming language.
0) add enough features for the seed program to be able to be used to read in code in the new language
to create an x64 Windows PE that returns 1.

technical design:
processing begins in the setting.r file, which contains settings for the program:
opens the includedType.r file which first enumerates all of the types local to the program,
then enumerates all the types from libraries.
opens the includedVariable.r which first enumerates all the variables global to the program,
then enumerates all exposed variables from libraries.

text is read in byte by byte, stored as a LineText then converted to LineBinaryIntermediate.
then depending on what the line represents, it becomes...

instructionGroupType[0]: # (preprocessor directive), ...
instructionGroupType[1]: d (define), s (program-wide setting), ...
instructionType: ...
