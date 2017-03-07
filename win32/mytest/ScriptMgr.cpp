#pragma  once
#include "stdafx.h"
#include "ScriptMgr.h"
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include "mylib.h"
int  tolua_script_open(lua_State* tolua_S);



ScriptMgr::ScriptMgr()
{

}

ScriptMgr::~ScriptMgr()
{

}

ScriptMgr& ScriptMgr::Instance()
{
	static ScriptMgr mgr;
	return mgr;
}

void ScriptMgr::Init()
{
	L = lua_open();
	luaL_openlibs(L);
	tolua_script_open(L);

	RegisterFunctions(mylib);
}


void ScriptMgr::RegisterFunctions(luaL_reg Funcs[])
{
	for (int i = 0;; i++)
	{
		if (Funcs[i].name == NULL || Funcs[i].func == NULL)
		{
			break;
		}
		lua_register(L, Funcs[i].name, Funcs[i].func);
	}
}