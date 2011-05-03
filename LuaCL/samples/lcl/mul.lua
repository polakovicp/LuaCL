--~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
--	This file is part of bachelor project:
--		"OpenCL module for Lua language"
--
--	filename	: 	mul.lua
--
--	description	: 	matrix multiply
--
--
--	autor		: 	Patrik Polakovic
--
--			26.04.2011
--
--~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

lcl = require('lcl.core')
cl = require('LuaCL')

local W = 10
local H = 10

sample = [[
	__kernel void mm( __const sampler_t smp, __read_only image2d_t in1, __read_only image2d_t in2, __write_only image2d_t out )
	{
		int j;

		int width = get_image_width(in1);

		int2 coords1 = (int2) {get_global_id(0), 0};
		int2 coords2 = (int2) {0, get_global_id(1)};

		uint4 mul = {0,0,0,0};
		uint4 pom1, pom2;

		for(j = 0; j<width; j++)
		{
			pom1 = read_imageui(in1, smp, coords2);
			pom2 = read_imageui(in2, smp, coords1);

			mul += pom1.x*pom2.x;

			++coords1.y;
			++coords2.x;
		}

		int2 coord = (int2) {get_global_id(0), get_global_id(1)};
		write_imageui(out, coord, mul);
	}
]]

local cx = lcl.NewContext('CL_DEVICE_TYPE_GPU')

local queue = cx.Queue()

nums1 = {}; nums2 = {}
for i=1,W*H do
	nums1[i] = i - 1;
	nums2[i] = W*H - i;
end

for i=1, W*H do
	io.write(nums1[i], '\t')
	if i%W == 0 then
		io.write('\n')
	end
end
io.write('\n')
for i=1, W*H do
	io.write(nums2[i], '\t')
	if i%H == 0 then
		io.write('\n')
	end
end

local matrix1 = cx.Image2D('CL_MEM_READ_ONLY, CL_MEM_COPY_HOST_PTR', {'CL_R', 'CL_UNSIGNED_INT32'}, W, H, 'uint', nums1)
local matrix2 = cx.Image2D('CL_MEM_READ_ONLY, CL_MEM_COPY_HOST_PTR', {'CL_R', 'CL_UNSIGNED_INT32'}, H, W, 'uint', nums2)
local matrix3 = cx.Image2D('CL_MEM_READ_ONLY', {'CL_R', 'CL_UNSIGNED_INT32'},H,H, 'uint')

local smp = cx.Sampler('CL_FALSE', 'CL_ADDRESS_NONE', 'CL_FILTER_NEAREST')

prog = cx.Build(sample)

local kernel = prog.Kernel('mm')
kernel.SetArgs(smp, matrix1, matrix2, matrix3)

queue.NDRange(kernel, nil, {H, H})

queue.Finish()

local o = queue.ReadImage(matrix3)

io.write('\n')
for i=1, H*H do
	io.write(o[i], '\t')
	if i%H == 0 then
		io.write('\n')
	end
end

prog.Release()
kernel.Release()
cx.Release()
queue.Release()
matrix1.Release()
matrix2.Release()
matrix3.Release()
