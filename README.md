# xmas-emu
This is a simple emulator I wrote for my team [enhance](https://ctftime.org/team/107311) in the [2019 x-mas ctf!](https://ctftime.org/event/926)

## About the emu

The emulator runs instructions till a print command is hit at which it prints the char to the terminal and a log file. After you enter a character here it will run to the next print, unless you enter s in which case you will go into step mode where the program reads a char for each instruction.

There are also some args that you can put into it (look at emu.c for the details) as well as info about the process being printed as it runs instructions.
