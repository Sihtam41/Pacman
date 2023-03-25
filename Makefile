 CC = g++
CPPFLAGS = -std=c++11 -c -g
LD = g++
LDFLAGS = -std=c++11
FINAL_TARGET = pacman


QT_DIR = ../qtbase-small-5.15.2-x64

#INCLUDE_DIR = -I$(QT_DIR)/include
INCLUDE_DIR = -I$(QT_DIR)/include -I$(QT_DIR)/include/QtCore

LIB_DIR = -L$(QT_DIR)/lib
LIBS = -lQt5Widgets -lQt5Gui -lQt5Core

default: $(FINAL_TARGET)

$(FINAL_TARGET): jeu.o main.o pacmanwindow.o
	$(LD) $+ -o $@ $(LDFLAGS) $(LIB_DIR) $(LIBS)

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(INCLUDE_DIR) $< -o $@

clean:
	del *.o $(FINAL_TARGET)

#mingw32-make