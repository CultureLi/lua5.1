// lua.cpp : �������̨Ӧ�ó������ڵ㡣
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
// 	int stackTop = lua_gettop(pLuaState);//��ȡջ��������ֵ  
// 	int nIdx = 0;
// 	int nType;
// 
// 
// 	printf("--ջ��(v)(%d)--\n", stackTop);
// 
// 	//��ʾջ�е�Ԫ��  
// 	for (nIdx = stackTop; nIdx > 0; --nIdx)
// 	{
// 		nType = lua_type(pLuaState, nIdx);
// 		printf("(i:%d) %s(%s)\n", nIdx, lua_typename(pLuaState, nType), lua_tostring(pLuaState, nIdx));
// 	}
// 
// 	printf("--ջ��--\n");
// }

/*
lua_next() ��������Ĺ��������ǣ�
1) �ȴ�ջ������һ�� key
2) ��ջָ��λ�õ� table ��ȡ��һ�� key-value���Ƚ� key ��ջ�ٽ� value ��ջ
3) ����� 2 ���ɹ��򷵻ط� 0 ֵ�����򷵻� 0�����Ҳ���ջ��ѹ���κ�ֵ

�� 2 ���д� table ��ȡ����ν����һ�� key-value��������ڵ� 1 ���е����� key �ġ�
table ���һ�� key-value ��ǰ��û�����ݣ��������� lua_pushnil() ѹ��һ�� nil �䵱��ʼ key��
����table�� �ٶ���˳���� table["t1"] = "t2",table["t3"] = "t4";��ʱ�������lua_pushnil();
����lua_pushstring(L,"t1") ��һ��ѭ���н� t3 t4��ջ t4(value) Ϊջ��
*/

//��ӡջ��table��Ϣ
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
			lua_pop(L, 1);//��ջ����ֵ�Ƴ�ջ����key��Ϊջ���Ա��������  
		}
	}
	lua_settop(L, oldtop);
}
//��ӡջ��Ϣ
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

//��̬ע��
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





