#include <lua.hpp>

static int l_one(lua_State *L)
{
	printf("\033[36mhello world\n\033[0m");	
	return 0;
}

static const struct luaL_Reg mylib[] = 
{
	{"one",l_one},
	{NULL,NULL}	
};

extern "C" int luaopen_luaso(lua_State *L)
{
	luaL_newlib(L,mylib);	
	return 1;
}
