Project(Villages)

cmake_minimum_required(VERSION 2.8)

Include(FindSDL)
Include(FindSDL_image)
Include(FindSDL_ttf)
Include(FindSDL_mixer)

FILE(GLOB WINDOWDLLS lib/windows/dlls/*.dll)

FILE(GLOB SDLGFX includes/sdl-gfx/*.h includes/sdl-gfx/*.c)
FILE(GLOB TinyXML2 includes/tinyxml2/*.h includes/tinyxml2/*.cpp)

FILE(GLOB Graphics src/Engine/Graphics/*.h src/Engine/Graphics/*.cpp)
File(GLOB Time src/Engine/Time/*.h src/Engine/Timer/*.cpp)
FILE(GLOB Util src/Engine/Util/*.h src/Engine/Util/*.cpp)

FILE(GLOB Config data/config/config.xml)
FILE(GLOB Tiles data/tiles/*.png)
FILE(GLOB Maps data/maps/*.xml)
FILE(GLOB Fonts data/fonts/*.tff)
FILE(GLOB Music data/music/*.ogg)

set (
	SOURCES
	src/main.cpp

	${SDLGFX}
	${TinyXML2}

	${Graphics}
	${Time}
	${Util}
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

	source_group(Source\ Files\\TinyXML2 FILES ${TinyXML2})
	source_group(Source\ Files\\SDL-Gfx FILES ${SDLGFX})

	source_group(Source\ Files\\Engine\\Graphics FILES ${Graphics})
	source_group(Source\ Files\\Engine\\Time FILES ${Time})
	source_group(Source\ Files\\Engine\\Util FILES ${Util})

	link_libraries (
		${SDL_LIBRARY}
		${SDLIMAGE_LIBRARY}
		${SDLTTF_LIBRARY}
		${SDLMIXER_LIBRARY}
		../../lib/windows/SDLmain
	)

	file(COPY ${DLLS} DESTINATION Debug)

	file(COPY ${Config} DESTINATION Debug/data/config)
	file(COPY ${Tiles} DESTINATION Debug/data/tiles)
	file(COPY ${Maps} DESTINATION Debug/data/maps)
	file(COPY ${Fonts} DESTINATION Debug/data/fonts)
	file(COPY ${Music} DESTINATION Debug/data/music)

elseif(${UNIX})

	link_libraries (
		${SDL_LIBRARY}
		${SDLIMAGE_LIBRARY}
		${SDLTTF_LIBRARY}
		${SDLMIXER_LIBRARY}
	)


	file(COPY ${Config} DESTINATION data/config)
	file(COPY ${Tiles} DESTINATION data/tiles)
	file(COPY ${Maps} DESTINATION data/maps)
	file(COPY ${Fonts} DESTINATION data/fonts)
	file(COPY ${Music} DESTINATION data/music)

endif(${WIN32})

add_executable (
	Villages
	WIN32
	MACOSX_BUNDLE
	${SOURCES}
)