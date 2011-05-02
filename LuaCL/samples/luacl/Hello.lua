--~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
--
--	filename	: 	Hello.lua
--	autor		: 	Patrik Polakovic
--
--			26.04.2011
--
--~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

local cl = require ('LuaCL')

local function ERR_CHK(...)
	if table.getn(arg) == 2 then
		if arg[2] ~= cl.CL_SUCCESS then
			print("OpenCL error code:", arg[2])
		end
		return arg[1]
	elseif table.getn(arg) == 3 then
		if arg[3] ~= cl.CL_SUCCESS then
			print("OpenCL error code:", arg[3])
		end
		return arg[1], arg[2]
	else
		if arg[1] ~= cl.CL_SUCCESS then
			print("OpenCL error code:", arg[1])
		end
		return
	end
end

local function run_hello(str)
	src = [[
		#pragma OPENCL EXTENSION cl_khr_byte_addressable_store : enable
		__kernel void hello(__global uchar * output)
		{
			private char hello[] = "]].. str ..[[";
			unsigned int tid = get_global_id(0);
			output[tid] = hello[tid];
		}

	]]
	local platform = ERR_CHK(cl.PlatformIDs(1))
	local device = ERR_CHK(cl.DeviceIDs(platform[1],
										'CL_DEVICE_TYPE_CPU'))

	local context = ERR_CHK(cl.CreateContext({'CL_CONTEXT_PLATFORM',
											platform[1]},
											device))

	local queue = ERR_CHK(cl.CreateQueue(context,
										device[1],
										'CL_QUEUE_PROFILING_ENABLE'))

	local program = ERR_CHK(cl.CreateProgram(context, src))
	ERR_CHK(cl.BuildProgram(program, device))

	local outBuf = ERR_CHK(cl.CreateBuffer(context,
										'CL_MEM_WRITE_ONLY',
										"uchar",
										string.len(str)))

	local kernel = ERR_CHK(cl.CreateKernel(program,
											"hello"))
	ERR_CHK(cl.SetKernelArg(kernel,
							0,
							outBuf))
	ERR_CHK(cl.NDRangeKernel(queue,
							kernel,
							{0},
							{string.len(str)},
							nil))

	local output = ERR_CHK(cl.ReadBuffer(queue,
										outBuf,
										'CL_TRUE',
										"uchar",
										0,
										string.len(str)))
	o = ""
	for i=1, #output do
		o = o .. string.char(output[i])
	end

	print(o)

	cl.ReleaseContext(context)
	cl.ReleaseQueue(queue)
	cl.ReleaseMem(outBuf)
	cl.ReleaseKernel(kernel)
	cl.ReleaseProgram(program)

end

run_hello("Hello World!")

