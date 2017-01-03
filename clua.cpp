#include <stdio.h>
#include <stdlib.h>
#include "lua.hpp"
#include <string.h>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc,char *argv[])
{	
	lua_State *L = luaL_newstate();
	if(L == NULL)
	{
		printf("create lua_State error\n");
		return 0;
	}

	luaL_openlibs(L);
	string tFile = "./luatest.lua";
	int tError = luaL_loadfile(L,tFile.c_str());
	if(tError != 0)
	{
		const char* tChar = lua_tostring(L,-1);
		printf("loadfile %s error %s\n",tFile.c_str(),tChar);
		lua_close(L);
		return 0;
	}

	if(lua_pcall(L,0,0,0) != 0)
	{
		const char* tChar = lua_tostring(L,-1);
		printf("call %s error %s\n",tFile.c_str(),tChar);
		lua_close(L);
		return 0;
	}

	lua_close(L);

	return 0;
}
