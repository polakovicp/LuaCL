local cl = require("cl")
local ffi = require( "ffi" )
local bit = require( "bit" )

SIZE = 100
ISIZE = {}
ISIZE["x"] = 5
ISIZE["y"] = 10

local function BuffSource(c)
	return	[[
			__kernel void mul(__global const uint* input, __global  uint* output)
			{
				int i = get_global_id(0);
				if (i < ]].. c ..[[)
				output[i] = input[i]+10;
			}
			]]
end

local function ImgSource(c)
	return	[[
			__kernel void mul(__read_only image2d_t input, __write_only image2d_t output)
			{
				const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE |
								CLK_ADDRESS_REPEAT |
								CLK_FILTER_NEAREST;
				int2 coord = (int2)(get_global_id(0), get_global_id(1));
				uint4 val = read_imageui(input, sampler, coord);
				val.x = val.x * ]].. c ..[[;
				write_imageui(output, coord, val);
			}
			]]
end

local function BufferRun(cx, queue)

	local err = ffi.new("cl_int[1]")

	local data = ffi.new("[?]", count)
	for i=0, count-1 do
		in_data[i] = (i+10)
	end

	buf = {}
	buf.read, e = cx:Buffer("CL_MEM_READ_ONLY, CL_MEM_COPY_HOST_PTR", "cl_uint", #data, data)
	buf.write = cx:Buffer("CL_MEM_WRITE_ONLY", "cl_uint", #data)

	prog,e = cx:Build(BuffSource(SIZE))
	kernel,e = prog:CreateKernel("mul")
	kernel:SetArgs(buf.read, buf.write)

	queue:NDRange(kernel, 1, {0,0,0}, {SIZE,0,0}, nil)
	out = queue:Read(buf.write, #data)
	for i=1, SIZE do
		io.write(out[i], "\t")
		if i%10 == 0 then io.write("\n") end
	end

	buf.read:Release()
	buf.write:Release()
	prog:Release()
	kernel:Release()
end

platforms = ffi.new( "cl_platform_id[1]")
cl.clGetPlatformIDs( 1, platforms, nil )
device = ffi.new("cl_device_id[1]")
cl.clGetDeviceIDs(platforms[0], cl.CL_DEVICE_TYPE_CPU, 1, device, nil)
context = ffi.new("cl_context *")
context = cl.clCreateContext(nil, 1, device, nil, nil, ret)

i,e = Context.Info(context)

queue = ffi.new("cl_command_queue")
queue = cl.clCreateCommandQueue(context, device[0], 0, nil)

BufferRun(context, queue)
