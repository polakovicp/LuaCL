lcl = require('lcl.core')
cl = require('LuaCL')

function _print(t)
	for i, d in ipairs(t) do
		if(type(d) == "table") then
			print(unpack(d))
		else
			print(d)
		end
	end
end

function CHK(...)
	print(arg[1]..': Error '.. arg[#arg])
	if #arg > 2 then
		return arg[2], arg[3]
	else
		return arg[2]
	end
end

function run(d, t)
	evnt = {}
	cx,e = CHK("Context", lcl.NewContext())
	qe = cx.Queue()

	b1 = CHK("1. Buffer", cx.Buffer('CL_MEM_READ_WRITE', t, #d))
	evnt[#evnt+1] = CHK("WriteBuffer", qe.WriteBuffer(b1, d))
	b2 = CHK("2. Buffer", cx.Buffer('CL_MEM_READ_WRITE', t, #d))

	evnt[#evnt+1] = CHK("CopyBuffer", qe.CopyBuffer(b1, b2))

	out, evnt[#evnt+1] = CHK("ReadBuffer", qe.ReadBuffer(b2))

	CHK("WaitForEvents", cl.WaitForEvents(qe.GetData(), evnt))
	_print(out)

end


data = {}
for i=1, 100 do
	data[i] = i
end

data2 = {}
for i=1, 100 do
	data2[i] = {i,i,i,i,i,i,i,i}
end

run(data, "int")
run(data2, "uint8")
