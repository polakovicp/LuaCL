local cl = require("cl")
local ffi = require( "ffi" )

function run(d, t)

	pl = ffi.new("cl_platform_id[1]")
	cl.clGetPlatformIDs(1, pl, NULL)

	de = ffi.new("cl_device_id[1]")
	cl.clGetDeviceIDs(pl[0], cl.CL_DEVICE_TYPE_CPU, 1, de, nil)

	cx = cl.clCreateContext(nil, 1, de, nil, nil, nil)

	qe = cl.clCreateCommandQueue(cx, de[0], 0, nil)

	b1 = cl.clCreateBuffer(cx, cl.CL_MEM_READ_WRITE, #data*ffi.sizeof(t), NULL, NULL)
	b2 = cl.clCreateBuffer(cx, cl.CL_MEM_READ_WRITE, #data*ffi.sizeof(t), NULL, NULL)

	indata = ffi.new(t.."[?]", #d, d)
	cl.clEnqueueWriteBuffer(qe, b1, cl.CL_TRUE, 0, #data*ffi.sizeof(t), indata, 0, nil, nil)

	cl.clEnqueueCopyBuffer(qe, b1, b2, 0, 0, #data*ffi.sizeof(t), 0, nil, nil)

	outdata = ffi.new(t.."[?]", #d, d)
	cl.clEnqueueReadBuffer(qe, b2, cl.CL_TRUE, 0, #data*ffi.sizeof(t), outdata, 0, nil, nil)

	for i=0, #d-1 do
		print(outdata[i])
	end

	cl.clReleaseContext(cx)
	cl.clReleaseCommandQueue(qe)
	cl.clReleaseMemObject(b1)
	cl.clReleaseMemObject(b2)

end

data = {}
for i=1, 100 do
	data[i] = i*(-1.1)
end

run(data, "cl_char")
run(data, "cl_uchar")
run(data, "cl_short")
run(data, "cl_ushort")
run(data, "cl_int")
run(data, "cl_uint")
run(data, "cl_long")
run(data, "cl_ulong")
run(data, "cl_half")
run(data, "cl_float")
run(data, "cl_double")
