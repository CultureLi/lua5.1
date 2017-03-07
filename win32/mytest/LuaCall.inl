////////////////////////////////////////////////////////////////////////////
// 类导出文件
// 此文件主要是封装了脚本的Call函数
// 使用这套函数模板可以方便调用脚本接口
////////////////////////////////////////////////////////////////////////////

#define DefineLuaType(type) \
	namespace Script{\
		namespace helper{\
			inline void PushParam(lua_State *L, type* pObj) {tolua_pushusertype(L, pObj, #type);}\
		}\
	}

namespace Script
{

	namespace helper
	{
		inline void PushParam(lua_State *L, int i) { lua_pushnumber(L,i); }
		inline void PushParam(lua_State *L, DWORD i) { lua_pushnumber(L,i); }
		inline void PushParam(lua_State *L, float f) { lua_pushnumber(L,f); }
		inline void PushParam(lua_State *L, double f) { lua_pushnumber(L,f); }
		inline void PushParam(lua_State *L, bool b) { lua_pushboolean(L,b); }
		inline void PushParam(lua_State *L, const char* sz) { lua_pushstring(L,sz); }
		inline void PushParam(lua_State *L, std::string &sz) { lua_pushstring(L,sz.c_str()); }
		inline void PushParam(lua_State *L, void* pv) { lua_pushlightuserdata(L,pv); }
		//inline void PushParam(lua_State *L, ByteBuffer& pv) { lua_pushlightuserdata(L,&pv); }

		template <typename Ret>
		inline Ret DefaultRet() { return Ret(0); }
		template <>
		inline void DefaultRet() {};

		template <typename Ret>
		inline Ret PopRet(lua_State *L) { Ret t = (Ret)lua_tonumber(L,-1); lua_pop(L,1);  return t;}
		template<>
		inline void PopRet(lua_State *L) { lua_pop(L,1); }
		template<>
		inline bool PopRet(lua_State *L) { bool b = lua_toboolean(L,-1) != 0; lua_pop(L,1);  return b; }
		template<>
		inline const char* PopRet(lua_State *L) { const char *sz = lua_tostring(L,-1); lua_pop(L,1); return sz;}
		
		inline bool CheckSuccess(lua_State* L, int iRet,  const char *szFunc)
		{
			if (iRet == 0)
				return true;

			printf("[%s]%s\n", szFunc, lua_tostring(L, -1));
			lua_pop(L, 1);

			// 不重载
			//ScriptMgr::Instance().Clear(szFile);
			return false;
		}

		inline std::vector<std::string> split(const std::string &s, const std::string &seperator){
			std::vector<std::string> result;
			typedef std::string::size_type string_size;
			string_size i = 0;

			while (i != s.size()){
				//找到字符串中首个不等于分隔符的字母；
				int flag = 0;
				while (i != s.size() && flag == 0){
					flag = 1;
					for (string_size x = 0; x < seperator.size(); ++x)
					if (s[i] == seperator[x]){
						++i;
						flag = 0;
						break;
					}
				}

				//找到又一个分隔符，将两个分隔符之间的字符串取出；
				flag = 0;
				string_size j = i;
				while (j != s.size() && flag == 0){
					for (string_size x = 0; x < seperator.size(); ++x)
					if (s[j] == seperator[x]){
						flag = 1;
						break;
					}
					if (flag == 0)
						++j;
				}
				if (i != j){
					result.push_back(s.substr(i, j - i));
					i = j;
				}
			}
			return result;
		}

	};
};


inline bool LuaPushFuncToStack(lua_State* L, const char* szFuncName)
{
	std::vector<std::string> v = Script::helper::split(szFuncName, ".");
	if (v.size() == 0)
		printf("function name error");

	lua_settop(L, 0);
	lua_getglobal(L, v[0].c_str());

	int i = 1;
	while (i < v.size())
	{
		lua_getfield(L, -1, v[i++].c_str());
		lua_remove(L, -2);
	}

	if (!lua_isfunction(L, -1))
	{
		lua_pop(L, 1);
		printf("[%s]:is not a function!\n", szFuncName);
		return false;
	}
	return true;
}

template<typename Ret>
inline Ret LuaCall(lua_State* L,const char *szFuncName)
{
	if (!LuaPushFuncToStack(L, szFuncName))
		return Script::helper::DefaultRet<Ret>();

	if (Script::helper::CheckSuccess(L, lua_pcall(L, 0, 1, 0), szFuncName))
		return Script::helper::PopRet<Ret>(L);
	
	return Script::helper::DefaultRet<Ret>();
};


template<typename Ret, typename T1>
inline Ret LuaCall(lua_State* L, const char *szFuncName, T1 arg1)
{
	if (!LuaPushFuncToStack(L, szFuncName))
		return Script::helper::DefaultRet<Ret>();

	Script::helper::PushParam(L, arg1);
	if (Script::helper::CheckSuccess(L, lua_pcall(L, 1, 1, 0),  szFuncName))
		return Script::helper::PopRet<Ret>(L);

	return Script::helper::DefaultRet<Ret>();
};

template<typename Ret, typename T1, typename T2>
inline Ret LuaCall(lua_State* L, const char *szFuncName, T1 arg1, T2 arg2)
{
	if (!LuaPushFuncToStack(L, szFuncName))
		return Script::helper::DefaultRet<Ret>();

	Script::helper::PushParam(L, arg1);
	Script::helper::PushParam(L, arg2);
	if (Script::helper::CheckSuccess(L, lua_pcall(L, 2, 1, 0),  szFuncName))
		return Script::helper::PopRet<Ret>(L);

	return Script::helper::DefaultRet<Ret>();
};

template<typename Ret, typename T1, typename T2, typename T3>
inline Ret LuaCall(lua_State* L, const char *szFuncName, T1 arg1, T2 arg2, T3 arg3)
{
	if (!LuaPushFuncToStack(L, szFuncName))
		return Script::helper::DefaultRet<Ret>();

	Script::helper::PushParam(L, arg1);
	Script::helper::PushParam(L, arg2);
	Script::helper::PushParam(L, arg3);
	if (Script::helper::CheckSuccess(L, lua_pcall(L, 3, 1, 0),  szFuncName))
		return Script::helper::PopRet<Ret>(L);		

	return Script::helper::DefaultRet<Ret>();
};

template<typename Ret, typename T1, typename T2, typename T3, typename T4>
inline Ret LuaCall(lua_State* L, const char *szFuncName, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
{
	if (!LuaPushFuncToStack(L, szFuncName))
		return Script::helper::DefaultRet<Ret>();

	Script::helper::PushParam(L, arg1);
	Script::helper::PushParam(L, arg2);
	Script::helper::PushParam(L, arg3);
	Script::helper::PushParam(L, arg4);
	if (Script::helper::CheckSuccess(L, lua_pcall(L, 4, 1, 0), szFile, szFuncName))
		return Script::helper::PopRet<Ret>(L);

	return Script::helper::DefaultRet<Ret>();
};

template<typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5>
inline Ret LuaCall(lua_State* L, const char *szFuncName, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5)
{
	if (!LuaPushFuncToStack(L, szFuncName))
		return Script::helper::DefaultRet<Ret>();

	Script::helper::PushParam(L, arg1);
	Script::helper::PushParam(L, arg2);
	Script::helper::PushParam(L, arg3);
	Script::helper::PushParam(L, arg4);
	Script::helper::PushParam(L, arg5);
	if (Script::helper::CheckSuccess(L, lua_pcall(L, 5, 1, 0), szFile, szFuncName))
		return Script::helper::PopRet<Ret>(L);

	return Script::helper::DefaultRet<Ret>();
};

template<typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
inline Ret LuaCall(lua_State* L, const char *szFuncName, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6)
{
	if (!LuaPushFuncToStack(L, szFuncName))
		return Script::helper::DefaultRet<Ret>();

	Script::helper::PushParam(L, arg1);
	Script::helper::PushParam(L, arg2);
	Script::helper::PushParam(L, arg3);
	Script::helper::PushParam(L, arg4);
	Script::helper::PushParam(L, arg5);
	Script::helper::PushParam(L, arg6);
	if (Script::helper::CheckSuccess(L, lua_pcall(L, 6, 1, 0), szFile, szFuncName))
		return Script::helper::PopRet<Ret>(L);

	return Script::helper::DefaultRet<Ret>();
};

template<typename Ret, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
inline Ret LuaCall(lua_State* L, const char *szFuncName, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7)
{
	if (!LuaPushFuncToStack(L, szFuncName))
		return Script::helper::DefaultRet<Ret>();
	
	Script::helper::PushParam(L, arg1);
	Script::helper::PushParam(L, arg2);
	Script::helper::PushParam(L, arg3);
	Script::helper::PushParam(L, arg4);
	Script::helper::PushParam(L, arg5);
	Script::helper::PushParam(L, arg6);
	Script::helper::PushParam(L, arg7);
	lua_incref(L);
	if (Script::helper::CheckSuccess(L, lua_pcall(L, 7, 1, 0), szFile, szFuncName))
		return Script::helper::PopRet<Ret>(L);

	return Script::helper::DefaultRet<Ret>();
};



inline bool LuaCallF(lua_State* L, const char* szFuncName, const char* sig, ...)
{
	if (!LuaPushFuncToStack(L, szFuncName))
		return false;

    bool bRet = true;
    int argNum = 0;
    va_list vl;
    va_start(vl, sig);
    while (*sig)
    {
        bool bWhile = true;
        switch (*sig++)
        {
        case 'i':
            {
                int i = va_arg(vl, int);
                Script::helper::PushParam(L, i);
                argNum++;
            }
            break;
        case 's':
            {
                const char* s = va_arg(vl, char*);
                Script::helper::PushParam(L, s);
                argNum++;
            }
            break;
        case '>':
            {
                bWhile = false; //after '>' out put args
            }
            break;
        default:
            {
                ASSERT(0);
                bWhile = false;
                bRet = false;
            }
            break;
        }
        if (!bWhile)
            break;

        luaL_checkstack(L, 1, "too many arguments");
    }

    int nResult = (int)strlen(sig);
    if (bRet && Script::helper::CheckSuccess(L, lua_pcall(L, argNum, nResult, 0), szFuncName))
    {
        nResult = -nResult;		/* stack index of first result */
        while (*sig) 
        {	/* get results */
            switch (*sig++) 
            {
            case 'd':	/* double result */
                {
                    if (!lua_isnumber(L, nResult))
                    {
                        bRet = false;
                    }
                    *va_arg(vl, double *) = lua_tonumber(L, nResult);
                }
                break;
            case 'i':	/* int result */
                {
                    if (!lua_isnumber(L, nResult))
                    {
                        bRet = false;
                    }
                    *va_arg(vl, int *) = (int)lua_tonumber(L, nResult);
                }
                break;
            case 's':	/* string result */
                {
                    if (!lua_isstring(L, nResult))
                    {
                        bRet = false;
                    }
                    *va_arg(vl, const char **) = lua_tostring(L, nResult);
                }
                break;
            default:
                break;
            }
            nResult++;
        }
    }while(false);
    va_end(vl);
    return bRet;
}

