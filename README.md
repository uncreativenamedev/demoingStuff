overview:
language needs to be able to:
- interact directly with the Winapi, ...
- support multiple operating systems (windows, linux, mac, ...).
- support c features like preprocessor defines, preprocessor if, structs, ...
- write directly to machine code.
- be as easy to parse as possible. every line has exactly the same construction (using delimiter characters for variable-length fields).
- as precise as possible. don't worry about ease of reading or writing right now, those will come with the dedicated text editor.

to do:
write an example program in several separate files: setting.rmc, includeType.rmc, includeVariable.rmc, and main.rmc.
this is important so I know exactly what I need and I'm not overlooking anything or overthinking....

goal: this seed program is used to rewrite itself in the new programming language.
0) add enough features for the seed program to be able to be used to read in code in the new language
to create an x64 Windows PE that returns 1.

technical design:
processing begins in the setting.rmc file, which contains settings for the program:
opens the includedType.rmc file which first enumerates all of the types local to the program,
then enumerates all the types from libraries.
opens the includedVariable.rmc which first enumerates all the variables global to the program,
then enumerates all exposed variables from libraries.
All types defined in a program are global to the entire program.
Functions are considered to be a special type of variable. They are global to the program, unique, and also have a unique type.

text is read in byte by byte, stored as a LineText then converted to LineBinaryIntermediate.
instructionGroupTypeIndex, rather than storing characters to represent the groups of the instruction, stores the index values found in corresponding enums.
instructionTypeIndex does the same.
argumentLength is essentially the same as argumentLengthText, but converted to a number.
argumentTypeLocationIndex is essentially the same as argumentTypeLocationIndexText, but converted to numbers.
argumentTypeIndex stores indices corresponding to the argument types for each argument. How this works is each time the program encounters a new type it is added to an array in the includeType.rmc file (see detailed description of includeType.rmc for details).
argumentLocationIndex is essentially the same as argumentLocationIndexText, but converted to numbers.
argumentBinary is essentially the same as argumentText but converted to binary according to the type specified (for handling complex types pointers are expected to be used).

then depending on what the line represents, it becomes...

instructionGroupType[0]: represents a broad classification of the type of instruction.
instructionGroupType[1]: further specifies what grouping of instruction is being used.
instructionType: the specific name of the instruction being used (if any, for instance comment and exit file don't actually use instructions).
argumentLength: specifies the number of arguments, 0-9 (exit file is an example of using 0 arguments).
argumentTypeLocationIndex: 0: argument type is local to the program, other: found in the includeType.rmc file, with the number of values corresponding to argumentLength (this field may not be used, for instance comments and exit file don't)
argumentType: the type of each argument within the list of types in said location, with the number of values corresponding to argumentLength (this field may not be used, for instance comments and exit file don't).
argumentLocationIndex: 0: block scope, 1: function scope, 2: program scope, other: found in the includeVariable.rmc file (this field may not be used, for instance comments and exit file don't).
argument: can be a variable or a literal. the number of values here corresponds to argumentLength (this field may not be used, for instance exit file doesn't).

Predefined Values:
instructionGroupType[0]: u (undefined), / (comment), ~ (exit file), # (preprocessor directive), ...
instructionGroupType[1]: u (undefined), d (define), s (program-wide setting), ...
instructionType: createDefine, ...

For each variable-length field a delimiter character is specified at the start of the field, and then repeated at the end.
This can be anything as long as it doesn't appear in the field itself. By default $ is to be used.

all .rmc files must use Linux-style LF newlines (no carriage return byte), and UTF-8 without BOM encoding (ASCII characters only).

If using Visual Studio, write in .editorconfig file:
[*.rmc]
charset = utf-8
end_of_line = lf
trim_trailing_whitespace = true
insert_final_newline = false

Syntax examples:
Comment:
/u $$ 1 $$ $$ $$ $a comment$
instructionGroupTypeText[0], instructionGroupTypeText[1], space, instructionTypeText ($ specified as delimiter, no contents, $ as actual delimiter), 
space, argumentLengthText, space, argumentTypeLocationIndexText (see previous variable-length field), space, argumentLocationIndexText (see previous variable-length field), 
argumentText[0] (the first, and in this case only, argument. see previous variable-length field).

