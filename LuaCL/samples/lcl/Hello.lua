--~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
--
--	filename	: 	Hello.lua
--	autor		: 	Patrik Polakovic
--
--			26.04.2011
--
--~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

local wrapper = require('lcl.core')

local function run_hello(str)
	local src = [[
		#pragma OPENCL EXTENSION cl_khr_byte_addressable_store : enable
		__kernel void hello(__global uchar * output)
		{
			private char hello[] = "]].. str ..[[";
			unsigned int tid = get_global_id(0);
			output[tid] = hello[tid];
		}

		]]

	local cx = wrapper.NewContext()

	local qe = cx.Queue()

	local bu = cx.Buffer('CL_MEM_READ_WRITE', 'uchar', string.len(str))

	local prog = cx.Build(src)

	local krnl = prog.Kernel("hello")

	krnl.SetArgs(bu)

	qe.NDRange(krnl, {0}, {string.len(str)})

	qe.Finish()

	local output = qe.ReadBuffer(bu)

	local o = ""
	for i=1, #output do
		o = o .. string.char(output[i])
	end

	print(o)

	cx.Release()
	qe.Release()
	bu.Release()
	prog.Release()
	krnl.Release()
end

run_hello("Hello World!")
