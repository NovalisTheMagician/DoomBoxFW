#include <lua.h>
#include <lauxlib.h>

int main(int argc, char *argv[])
{
    lua_State *L = luaL_newstate();

    luaL_dostring(L, "print('Hello World')");

    lua_close(L);
    return 0;
}