--~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
--	This file is part of bachelor project:
--		"OpenCL module for Lua language"
--
--	filename	: 	julia.lua
--
--	description	: 	code for computing RGB colors of Julia fractal
--
--
--	autor		: 	Patrik Polakovic
--
--			26.04.2011
--
--~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

lcl = require('lcl.core')
cl = require('LuaCL')

IMG_WIDTH 	= 256
IMG_HEIGHT 	= 256

local function get_src(xb, xe, yb, ye, cx, cy, i) 
	return [[
	__kernel void fractal(__write_only image2d_t output)
			{								
				int2 coord = (int2)(get_global_id(0), get_global_id(1));
				
				int tid_x = get_global_id(0);
				int tid_y = get_global_id(1);
				
				float xbegin = ]] .. xb .. [[;
				float xend = ]] .. xe .. [[;
				float ybegin = ]] .. yb .. [[;
				float yend = ]] .. ye .. [[;
				float cx = ]] .. cx .. [[, cy = ]] .. cy .. [[;
				float zy0, zx0;
				float zx2, zy2;
				float zx, zy;
				uint iter;
				uint4 tex;
				int IMG_HEIGHT =  get_image_height(output), IMG_WIDTH = get_image_width(output), MAX_ITER = ]] .. i .. [[;
				
				zy0 = ybegin + tid_x*(yend-ybegin) / IMG_HEIGHT;
				zx0 = xbegin + tid_y*(xend-xbegin) / IMG_WIDTH;
				
				zx = zx0;
				zy = zy0;
				for(iter=0; iter<MAX_ITER; iter++)
				{
					zx2 = zx*zx;
					zy2 = zy*zy;
					if(zx2+zy2 > 4) break;
					zy = 2 * zx * zy + cy;
					zx = zx2 - zy2 + cx;
				}
				
				tex.x = iter%255;
				tex.y = iter%255;
				tex.z = iter%255;
				
				write_imageui(output, coord, tex);
			}
]]
end

local function run()
	local cx,e = lcl.NewContext('CL_DEVICE_TYPE_GPU')
		if e~=cl.CL_SUCCESS then print("Context error", e) return end
	local q,e = cx.Queue()
		if e~=cl.CL_SUCCESS then print("Queue error", e) return end
	local out,e = cx.Image2D('CL_MEM_WRITE_ONLY', {"CL_RGBA", "CL_UNSIGNED_INT32"}, IMG_WIDTH, IMG_HEIGHT, 'uint4')
		if e~=cl.CL_SUCCESS then print("Image error", e) return end
	local prog,e = cx.Build(get_src(-1.8, 1.1, -1.2, 1.2, -0.745, 0.113, 2000))
		if e~=cl.CL_SUCCESS then 
			print("Building program error", e) 
			local i = prog.BuildInfo()
			print(i['CL_PROGRAM_BUILD_LOG'])
			return 
		end
	local k,e = prog.Kernel("fractal")
		if e~=cl.CL_SUCCESS then print("Kernel error", e) return end
	k.SetArgs(out)
	evts = {}
	evts[1], e = q.NDRange(k, nil, {IMG_WIDTH, IMG_HEIGHT})
		if e~=cl.CL_SUCCESS then print("Queue NDRangeKernel error", e) return end
	
	q.Finish()
	
	data,ev,e = q.ReadImage(out)
		if e~=cl.CL_SUCCESS then print("Read Image error", e) return end
	
	for i,v in ipairs(data) do
		print(unpack(v))
	end
	
	cx.Release()
	q.Release()
	out.Release()
	prog.Release()
	k.Release()
end

run()