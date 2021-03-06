#include <lua.hpp>
#include <string.h>

#define STUDENT "student" 
int g_hook_key = 0;

struct student
{
	student():m_num(0),m_ref(0){}
	int m_num;
	int m_ref; 
};

static int lset(lua_State *L)
{
	student *stu = (student*)luaL_checkudata(L,1,STUDENT);
	if(stu == NULL)
	{
		printf("userdata error\n");
		return 0;
	}
	int num = lua_tonumber(L,2);
	stu->m_num = num;
	return 0;
}

static int lcall_back(lua_State *L)
{
	student *stu = (student*)luaL_checkudata(L,1,STUDENT);
	if(stu == NULL)
	{
		printf("param 1 error\n");
		return 0;
	}	

	lua_rawgeti(L,LUA_REGISTRYINDEX,stu->m_ref);
	lua_pushvalue(L,2);
	lua_pushstring(L,"hello world");
	lua_call(L,2,0);

	return 0;
}

static int lp(lua_State *L)
{
	student *stu = (student*)luaL_checkudata(L,1,STUDENT);
	if(stu == NULL)
	{
		printf("userdata error\n");
		return 0;
	}
	printf("m_num %d\n",stu->m_num);
	return 0;
}

static int linit(lua_State *L)
{
	if(!luaL_newmetatable(L,STUDENT))
	{
		printf("fun linit %s have exits registry\n",STUDENT);
		return 0;
	}
	lua_newtable(L);
	lua_pushstring(L,"p");
	lua_pushcfunction(L,lp);
	lua_rawset(L,-3);

	lua_pushstring(L,"set");
	lua_pushcfunction(L,lset);
	lua_rawset(L,-3);

	lua_pushstring(L,"call_back");
	lua_pushcfunction(L,lcall_back);
	lua_rawset(L,-3);	

	lua_setfield(L,-2,"__index");

	return 0;
}

static int lcreate(lua_State *L)
{
	if(lua_type(L,1) != LUA_TTABLE)
	{
		printf("type not table\n");
	}
	student *stu = (student*)lua_newuserdata(L,sizeof(student));
	luaL_setmetatable(L,STUDENT);
	if(lua_getmetatable(L,-1) != 1)
	{
		printf("metatable error\n");
	}
	lua_pushstring(L,"test");
	lua_gettable(L,1);
	if(lua_type(L,-1) != LUA_TFUNCTION)
	{
		printf("type not function 2 is %s\n",lua_typename(L,lua_type(L,1)));
	}
	stu->m_ref = luaL_ref(L,LUA_REGISTRYINDEX);
	lua_pop(L,1);
	return 1;
}

static int l_one(lua_State *L)
{
	printf("\033[36mhello world\n\033[0m");	
	return 0;
}

int make_mask(const char* mode)
{
	int mask = 0;
	if(strchr(mode,'c'))
	{
		mask |= LUA_MASKCALL;
	}

	if(strchr(mode,'r'))
	{
		mask |= LUA_MASKRET;
	}

	if(strchr(mode,'l'))
	{
		mask |= LUA_MASKLINE;
	}

	return mask;
}

static void l_hook(lua_State *L,lua_Debug *ar)
{
	static const char *hook_names[] =
	{
		"call","return","line","count","tail call"
	};
	lua_rawgeti(L,LUA_REGISTRYINDEX,g_hook_key);	
	luaL_checktype(L,-1,LUA_TFUNCTION);
	lua_pushstring(L,hook_names[(int)ar->event]);
	if(lua_pcall(L,1,1,0) != 0)
	{
		printf("call error:%s\n",lua_tostring(L,-1));
		return ;
	}
	if(lua_toboolean(L,-1))
	{
		lua_yield(L,0);
	}
}

static int set_hook(lua_State *L)
{
	if(lua_type(L,1) != LUA_TFUNCTION)	
	{
		printf("stack 1 not a function\n");
		return 0;
	}
	unsigned int len = 0;
	const char *mode = luaL_checklstring(L,2,&len);
	int mask = make_mask(mode);
	lua_Hook fun = NULL;
	if(mode != 0)
	{
		lua_pushvalue(L,1);
		g_hook_key = luaL_ref(L,LUA_REGISTRYINDEX);	
		fun = l_hook;
	}
	lua_sethook(L,fun,mask,0);	
	return 0;
}

static const struct luaL_Reg mylib[] = 
{
	{"create",lcreate},
	{"set_hook",set_hook},
	{NULL,NULL}	
};

extern "C" int luaopen_luaso(lua_State *L)
{
	linit(L);
	luaL_newlib(L,mylib);	
	return 1;
}
