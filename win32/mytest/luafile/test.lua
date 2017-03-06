Util = {}

function Util.SetData(value)
	print("SetData"..value)
	T:Instance():SetData(value)
end

function Util.GetData()
	print("GetData")
	return T:Instance():GetData()
end

function Util.test1()
	print("test1")
	local a = get(2,9,35,6)
	for k,v in pairs(a) do

		print(k,v)
	end
end

Util.SubTable = {}

function Util.SubTable.SubFun(...)
	print("subfun")
	 for i,v in ipairs(arg) do
       print(v)
    end

    return 1,"cc",0
end


function globalfun(...)
	print("globalfun")
	 for i,v in ipairs(arg) do
       print(v)
    end

end

