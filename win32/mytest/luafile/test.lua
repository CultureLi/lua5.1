Util = {}
Util.SubTable = {}

function Util.SetData(value)
	T:Instance():SetData(value)
end

function Util.GetData()
	return T:Instance():GetData()
end

function Util.test0()
	return 10
end

function Util.test1(...)
	print("------------call test1-----------")
	print("input args:")
	for k,v in ipairs(arg) do
		print(v)
	end
end

function Util.test2()
	print("------------call test2-----------")
end

function Util.test3()
	print("------------call test3-----------")
	return 10,"hello",20
end

function Util.test4(...)
	print("------------call test4-----------")
	print("input args:")
	for k,v in ipairs(arg) do
		print(v)
	end

	return 10,"hello",20
end


function Util.SubTable.SubFun(...)
	print("------------call SubFun-----------")
	print("input args:")
	 for i,v in ipairs(arg) do
       print(v)
    end

    return 1,"cc",0
end

function Util.lua_Call_C_StaticFunc()
	print("------------lua_Call_C_StaticFunc-----------")
	
	local a = lua_get()
	print("lua_get first result =",a)
	print("lua_set value")
	lua_set(999)
	a = lua_get()
	print("lua_get second result =",a)
	
end

function globalfun(...)
	print("------------call globalfun-----------")
	 print("input args:")
	 for i,v in ipairs(arg) do
       print(v)
    end

end




