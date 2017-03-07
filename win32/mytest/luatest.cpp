// lua.cpp : 定义控制台应用程序的入口点。
//
#pragma once
#include "stdafx.h"
#include "ScriptMgr.h"
#include "test.h"
using namespace std;
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

/*
lua_next() 这个函数的工作过程是：
1) 先从栈顶弹出一个 key
2) 从栈指定位置的 table 里取下一对 key-value，先将 key 入栈再将 value 入栈
3) 如果第 2 步成功则返回非 0 值，否则返回 0，并且不向栈中压入任何值

第 2 步中从 table 里取出所谓“下一对 key-value”是相对于第 1 步中弹出的 key 的。
table 里第一对 key-value 的前面没有数据，所以先用 lua_pushnil() 压入一个 nil 充当初始 key。
比如table中 假定是顺序存放 table["t1"] = "t2",table["t3"] = "t4";这时如果不用lua_pushnil();
而是lua_pushstring(L,"t1") 第一次循环中将 t3 t4入栈 t4(value) 为栈顶
*/

//打印栈内table信息
void StackDumpTable(lua_State* L, int index, int oldtop, bool bsub = false)
{
	if (!bsub)
	{
		printf("index: %d \n", index);
		lua_pushvalue(L, index);
	}
	lua_pushnil(L);
	while (lua_next(L, -2))
	{
		if (lua_type(L, -1) == LUA_TTABLE)
		{
			printf("[%s] = \n	{\n", lua_tostring(L, -2));
			StackDumpTable(L, -1, -2, true);
			printf("	}\n");
		}
		else
		{
			int t = lua_type(L, -2);
			int t2 = lua_type(L, -1);
			printf("	[%s] = %s	\n", lua_tostring(L, -2), lua_tostring(L, -1));
			lua_pop(L, 1);//把栈顶的值移出栈，让key成为栈顶以便继续遍历  
		}
	}
	lua_settop(L, oldtop);
}
//打印栈信息
void StackDump(lua_State* L)
{
	int i;
	int top = lua_gettop(L);
	for (i = 1; i <= top; i++) { /* repeat for each level */
		int t = lua_type(L, i);
		switch (t) {
		case LUA_TSTRING: /* strings */
			printf("index:%d	%s", i, lua_tostring(L, i));
			break;
		case LUA_TBOOLEAN: /* booleans */
			printf("index : %d,%s", i, lua_toboolean(L, i) ? "true" : "false");
			break;
		case LUA_TNUMBER: /* numbers */
			printf("index:%d	%g", i, lua_tonumber(L, i));
			break;
		case LUA_TTABLE:
			StackDumpTable(L, i, top);
			break;
		case LUA_TNIL:
			printf("index:%d	nil", i);
			break;
		default: /* other values */
			printf("index:%d	%s", i, lua_typename(L, t));
			break;
		}
		printf("\n"); /* put a separator */
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	ScriptMgr::Instance().Init();
	lua_State* L = ScriptMgr::Instance().L;
	luaL_dofile(L, "luafile/test.lua");

	LuaCallF(L, "globalfun", "si", "hello", 222);
	
	int result = LuaCall<int>(L, "Util.test0");
	printf("call test0 result = %d\n", result);
	
	LuaCall<void>(L, "Util.test1", 12, 45, "lua");

	LuaCallF(L, "Util.test2",""); // no input ,no returns

	LuaCallF(L, "Util.test1","ssi","hello","lua",100); //input three args

	int arg1 = 0;
	char* arg2 = "";
	int arg3 = 0;
	LuaCallF(L, "Util.test3", ">isi", &arg1,&arg2,&arg3);//return three results,no input
	printf("call test3 result = %d %s %d\n", arg1, arg2, arg3);

	LuaCallF(L, "Util.SubTable.SubFun", "iisisi>isi", 9, 1, "hello", 0, "lua", 8, &arg1, &arg2, &arg3);
	printf("call SubFun result = %d %s %d\n", arg1, arg2, arg3);


	//call c register function
	LuaCall<void>(L, "Util.lua_Call_C_StaticFunc");

	//call tolua++ export function
	result = LuaCall<int>(L, "Util.GetData");
	printf("call GetData first result %d\n", result);

	LuaCall<int>(L, "Util.SetData",100);
	printf("call SetData\n");

	result = LuaCall<int>(L, "Util.GetData");
	printf("call GetData second result %d\n", result);
	lua_close(L);

	
	return 0;
}





