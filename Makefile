# Compiler settings
CXX = g++
CXXFLAGS = -IC:\SFML\SFML-2.6.2-windows-gcc-13.1.0-mingw-64-bit\SFML-2.6.2\include -std=c++17 -Wall
LDFLAGS = -LC:\SFML\SFML-2.6.2-windows-gcc-13.1.0-mingw-64-bit\SFML-2.6.2\lib
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Source files
SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = main.exe

# DLL management (Windows paths with backslashes)
SFML_BIN = C:\SFML\SFML-2.6.2-windows-gcc-13.1.0-mingw-64-bit\SFML-2.6.2\bin
REQUIRED_DLLS = \
    "$(SFML_BIN)\sfml-graphics-2.dll" \
    "$(SFML_BIN)\sfml-window-2.dll" \
    "$(SFML_BIN)\sfml-system-2.dll" \
    "$(SFML_BIN)\openal32.dll"

.PHONY: all clean run

all: $(TARGET) copy_dlls

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

copy_dlls: $(TARGET)
	@echo Copying required DLLs...
	@for %%d in ($(REQUIRED_DLLS)) do ( \
		if exist %%d ( \
			copy /y "%%d" . > nul && echo Copied %%~nxd \
		) else ( \
			echo ERROR: Could not find %%d \
		) \
	)

run: all
	@echo Running $(TARGET)...
	@$(TARGET)

clean:
	@echo Cleaning up...
	@del /Q $(OBJS) $(TARGET) *.dll 2> nul