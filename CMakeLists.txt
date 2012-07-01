Project(Villages)

cmake_minimum_required(VERSION 2.8)

Include(FindSDL)
Include(FindSDL_image)
Include(FindSDL_ttf)
Include(FindSDL_mixer)

FILE(GLOB WINDOWDLLS lib/windows/dlls/*.dll)

FILE(GLOB SDLGFX includes/sdl-gfx/*.h includes/sdl-gfx/*.c)
FILE(GLOB TinyXML2 includes/tinyxml2/*.h includes/tinyxml2/*.cpp)

set (
	SOURCES
	${SDGFX}
	${TinyXML2}
)

set (
	DLLS
	${WINDOWDLLS}
)

Find_Package(SDL REQUIRED)
Find_Package(SDL_image REQUIRED)
Find_Package(SDL_ttf REQUIRED)
Find_Package(SDL_mixer REQUIRED)

include_directories(
	includes/sdl/
	includes/sdl-gfx/
	includes/tinyxml2/
	src/
)

if(${WIN32})

	link_libraries (
		${SDL_LIBRARY}
		${SDLIMAGE_LIBRARY}
		${SDLTTF_LIBRARY}
		${SDLMIXER_LIBRARY}
		../../lib/windows/SDLmain
	)

	file(COPY ${DLLS} DESTINATION Debug)

elseif(${UNIX})

	link_libraries (
		${SDL_LIBRARY}
		${SDLIMAGE_LIBRARY}
		${SDLTTF_LIBRARY}
		${SDLMIXER_LIBRARY}
	)

endif(${WIN32})

add_executable (
	Villages
	WIN32
	MACOSX_BUNDLE
	${SOURCES}
)