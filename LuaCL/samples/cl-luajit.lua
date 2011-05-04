-- require cl.lua binding module
local cl = require( "cl" )
local ffi = require( "ffi" )

local count = string.len("Hello World!")

local source = [[
		#pragma OPENCL EXTENSION cl_khr_byte_addressable_store : enable
		__kernel void hello(__global uchar * output)
		{
			private char hello[] = "Hello World!";
			unsigned int tid = get_global_id(0);
			output[tid] = hello[tid];
		}
		]]

local platform = ffi.new("cl_platform_id[1]")
cl.clGetPlatformIDs(1, platform, NULL)
local device = ffi.new("cl_device_id[1]")
cl.clGetDeviceIDs(platform[0], cl.CL_DEVICE_TYPE_CPU, 1, device, nil)
local context = cl.clCreateContext(nil, 1, device, nil, nil, nil)
local queue = cl.clCreateCommandQueue(context, device[0], 0, nil)
local buffer = cl.clCreateBuffer(context,
								cl.CL_MEM_WRITE_ONLY,
								count,
								NULL, NULL)
local src = ffi.new("char[?]", #source+1, source)
local src2 = ffi.new("const char*[1]", src)
local program = cl.clCreateProgramWithSource(context, 1, src2, nil, nil)
cl.clBuildProgram(program, 1, device, nil, nil, nil)
local kernel = cl.clCreateKernel(program, "hello", nil)
local out = ffi.new("cl_mem[1]", buffer)
cl.clSetKernelArg(kernel, 0, ffi.sizeof("cl_mem"), out)
local global_size = ffi.new("size_t[1]", count)
cl.clEnqueueNDRangeKernel(queue, kernel, 1, nil, global_size,
						nil, 0, nil, nil)
local output = ffi.new("char[?]", count)
cl.clEnqueueReadBuffer(queue, buffer, cl.CL_TRUE,
						0, count, output,
						0, nil, nil)

local o = ""
for i=0,count-1 do
	o = o .. string.char(output[i])
end

print(o)

--> output>
-->Hello World!
