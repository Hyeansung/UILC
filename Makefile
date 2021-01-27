CC = gcc
CFLAGS = -WALL -I./include
TARGET = UILC_Calculation
execute: main.o UILC_CA.o UILC_Core.o
	gcc -o exe
main.o: min.o
