#pragma once
#include "string"
#include "vector"
#include "tolua++.h"
//using namespace std;
class ScriptMgr
{
public:
	lua_State* L;

	ScriptMgr();
	~ScriptMgr();
	static ScriptMgr& Instance();
	void Init();
	void RegisterFunctions(luaL_reg Funcs[]);
};

#include "LuaCall.inl"