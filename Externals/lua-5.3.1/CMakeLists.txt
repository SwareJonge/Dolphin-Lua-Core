project(lua C)
include_directories (src ${CMAKE_CURRENT_BINARY_DIR})
set ( SRC_LIB src/lapi.c 
src/lbitlib.c
src/lcode.c 
src/lcorolib
src/lctype.c
src/ldebug.c 
src/ldo.c 
src/ldump.c 
src/lfunc.c 
src/lgc.c 
src/llex.c 
src/lmem.c 
src/lobject.c 
src/lopcodes.c 
src/lparser.c 
src/lstate.c 
src/lstring.c 
src/ltable.c 
src/ltm.c 
src/lundump.c 
src/lvm.c 
src/lzio.c 
src/lauxlib.c 
src/lbaselib.c 
src/ldblib.c 
src/liolib.c 
src/loadlib.c
src/lmathlib.c 
src/loslib.c 
src/ltablib.c 
src/lstrlib.c
src/lutf8lib.c
src/linit.c )
set ( SRC_LUA src/lua.c )
set ( SRC_LUAC src/luac.c)

add_library (lua STATIC ${SRC_LIB} ${SRC_LUA} ${SRC_LUAC})
