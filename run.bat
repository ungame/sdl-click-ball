cls
del *.exe
del *.o

g++ -c main.cpp -std=c++14 -g -Wall
g++ *.o -o main -lmingw32 -lSDL2main -lSDL2

.\main.exe