
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "lupa.h"

static int lupa_run(lua_State *L) {
    int nargs, loadok;
    nargs = lua_gettop(L);
    loadok = luaL_loadbuffer(L, luaJIT_BC_lupa, luaJIT_BC_lupa_SIZE, "=lupa");

    if (loadok == 0) {
        lua_call(L, nargs, LUA_MULTRET);
        return 0;
    }

    return 1;
}

int main(int argc, char *argv[]) {
    lua_State *L;
    int i;

    L = luaL_newstate();
    if (L == NULL) {
        fprintf(stderr, "PANIC: failed to create main state!\n");
        return 1;
    }

    luaL_openlibs(L);

    lua_createtable(L, argc, 0);
    for (i = 0; i < argc; i++) {
        lua_pushstring(L, argv[i]);
        lua_rawseti(L, -2, i);
    }
    lua_setglobal(L, "arg");

    if (lupa_run(L)) {
        printf("%s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
        lua_close(L);
        return -1;
    }

    lua_close(L);
    return 0;
}

