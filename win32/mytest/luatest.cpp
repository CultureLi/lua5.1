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




int  tolua_script_open(lua_State* tolua_S);

// void PrintLuaStack(lua_State* pLuaState)
// {
// 	int stackTop = lua_gettop(pLuaState);//获取栈顶的索引值  
// 	int nIdx = 0;
// 	int nType;
// 
// 
// 	printf("--栈顶(v)(%d)--\n", stackTop);
// 
// 	//显示栈中的元素  
// 	for (nIdx = stackTop; nIdx > 0; --nIdx)
// 	{
// 		nType = lua_type(pLuaState, nIdx);
// 		printf("(i:%d) %s(%s)\n", nIdx, lua_typename(pLuaState, nType), lua_tostring(pLuaState, nIdx));
// 	}
// 
// 	printf("--栈底--\n");
// }

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

//静态注册
static int get(lua_State* L)
{
	int top = lua_gettop(L);

	int arg1 = lua_tonumber(L, -1);
	int arg2 = lua_tonumber(L, -2);
	int arg3 = lua_tonumber(L, -3);
	int arg4 = lua_tonumber(L, -4);

	lua_settop(L, 0);
	lua_newtable(L);
	lua_pushnumber(L, arg1);
	lua_pushnumber(L, arg2);
	lua_settable(L, -3);
	lua_pushnumber(L, arg3);
	lua_pushnumber(L, arg4);
	lua_settable(L, -3);

	return 1;
}


int _tmain(int argc, _TCHAR* argv[])
{
	lua_State* L = lua_open();
	luaL_openlibs(L);


	//tolua test
	tolua_script_open(L);
	luaL_dofile(L, "luafile/test.lua");

	//lua_getglobal(L, "Util");
	//lua_getfield(L, -1, "GetData");
	//StackDump(L);

	//lua_pushstring(L, "GetData"); //push a key
	//lua_gettable(L, -2); //get the value,and and push the value in stack (top)
 	//lua_pcall(L, 0, LUA_MULTRET, 0);  //call the GetData function,get the result and push the result in stack (top)
 	//int result = lua_tointeger(L, -1);
 	//printf("result = %d\n", result);
// 	lua_pop(L, 1); // pop  the top 
// 
//  	lua_pushstring(L, "SetData");
//  	lua_pushnumber(L, 345);
//  	lua_pcall(L, 2, LUA_MULTRET, 0

//	lua_getglobal(L, "Util");
// 	lua_pushstring(L, "GetData");
// 	lua_pcall(L, 1, LUA_MULTRET, 0);
//	result = lua_tointeger(L, -1);
//	printf("%d\n", result
	//lua stack operate test
	/*
	lua_pushnumber(L, 999);
	lua_newtable(L);
	lua_pushstring(L, "t1");
	lua_pushstring(L, "t2");
	lua_settable(L, -3);        //{ ["t1"] = "t2"}

	lua_newtable(L);
	lua_pushstring(L, "t3");
	lua_pushstring(L, "t4");
	lua_settable(L, -3);		//{ ["t3"] = "t4"}

	lua_pushstring(L,"subtable");
	lua_insert(L,-2);
	lua_settable(L, -3);			//{}

	lua_newtable(L);
	lua_insert(L, -2);
	lua_pushstring(L, "haha");
	lua_insert(L, -2);
	lua_settable(L, -3);

	lua_pushstring(L, "Hello");
	lua_pushstring(L, "World");
	lua_settable(L, -3);

	lua_pushstring(L, "end");
	StackDump(L);*/


	/*lua_newtable(L);
	lua_pushstring(L, "ss");
	lua_pushstring(L, "data1");
	lua_settable(L,-3);

	lua_pushnumber(L, 999);
	lua_setfield(L,-2,"ok");

	lua_getfield(L, -1, "ss");
	string str = lua_tostring(L, -1);
	printf("%s", str);
	lua_pop(L,1);

	lua_pushnumber(L, 1);
	lua_pushnumber(L, 999);
	lua_settable(L, -3);

	lua_pushnumber(L, 2);
	lua_pushnumber(L, 222);
	lua_settable(L, -3);

	StackDump(L);*/


	//lua static register test
	/*lua_register(L, "get", get);

	luaL_dofile(L,"test.lua");

	lua_getglobal(L, "Util");
	lua_pushstring(L, "test1");
	lua_gettable(L, -2);
	//lua_getfield(L,-1,"test1");
	lua_pcall(L, 0, LUA_MULTRET, 0);
	*/

	int arg1 = 0;
	char* arg2 = NULL;
	int arg3 = 0;
	
	LuaCallF(L, "Util.SubTable.SubFun", "iisisi>isi", 9, 1, "hello", 0, "lua", 8, &arg1, &arg2, &arg3);
	printf("result = %d %s %d\n", arg1, arg2, arg3);


	LuaCallF(L, "globalfun", "si", "globalfun", 222);


	lua_close(L);

	
	return 0;
}





