#pragma once
#include "string"
#include "vector"
#include "tolua++.h"
//using namespace std;
class ScriptMgr
{
public:
	ScriptMgr();
	~ScriptMgr();
	static ScriptMgr& Instance();
};

#include "LuaCall.inl"