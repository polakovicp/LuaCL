require "LuaCl"

function rozbal(co, odkial, pokial)
	odkial = odkial or 1
	if co[odkial] and odkial <= pokial then return co[odkial], rozbal(co, odkial + 1, pokial) end
end

local ROW = 600 --pocet riadkov matice
local COL = ROW --pocet stlpcov matice

sample = [[
	__kernel void matrix_multiply_slow( __constant uint *vectorIn1, __constant uint *vectorIn2, __global uint *vectorOut )
	{
		int rows =]].. ROW ..[[ ;
		int j;
		int myid = (int) get_global_id(0);

		int myrow = myid/rows;

		vectorOut[myid] = 0;

		for(j = 0; j<rows; j++)
		{
			vectorOut[myid] +=  vectorIn1[rows*myrow+j] * vectorIn2[myid%rows+(j*rows)];
		}
	}

	__kernel void matrix_multiply_fast( __constant uint *vectorIn1, __constant uint *vectorIn2, __global uint *vectorOut )
	{
		int rows =]].. ROW ..[[ ;
		int j;
		uint mul = 0;
		int myid = (int) get_global_id(0);

		int myrow = myid/rows;

		for(j = 0; j<rows; j++)
		{
			mul +=  vectorIn1[rows*myrow+j] * vectorIn2[myid%rows+(j*rows)];
		}

		vectorOut[myid] = mul;
	}
]]
--ziskanie zariadeni
platform = cl.GetPlatformIDs(1)

CPUdevice = cl.GetDeviceIDs(platform[1], 'CL_DEVICE_TYPE_CPU', 1)
print("CPU",cl.GetDeviceInfo(CPUdevice[1], 'CL_DEVICE_NAME', 100))
GPUdevice = cl.GetDeviceIDs(platform[1], 'CL_DEVICE_TYPE_GPU', 1)
print("GPU",cl.GetDeviceInfo(GPUdevice[1], 'CL_DEVICE_NAME', 100))

devices = {CPUdevice[1], GPUdevice[1]}
--vytvorenie kontextu
props = {}
props[1] = cl.CL_CONTEXT_PLATFORM
props[2] = platform[1]

context = cl.CreateContext(props, devices)
--vytvorenie queues na CPU a GPU
CPUqueue = cl.CreateCommandQueue(context, devices[1])
GPUqueue = cl.CreateCommandQueue(context, devices[2])

--inicializovanie a kopirovanie buffrov
nums1 = {}; nums2 = {}
for i=1,ROW*COL do
	nums1[i] = i - 1;
	nums2[i] = ROW*COL - i;
end

startTime = cl.Timer()
matrix1 = cl.BufferUINT(context, 'CL_MEM_READ_ONLY, CL_MEM_COPY_HOST_PTR', ROW*COL, nums1)
if not matrix1 then print("Buffer init Failed") end

--cl.EnqueueWriteBuffer(CPUqueue, matrix1, cl.CL_TRUE, 0, cl.CL_UINT, ROW*COL, nums1)
--cl.EnqueueWriteBuffer(GPUqueue, matrix1, cl.CL_TRUE, 0, cl.CL_UINT, ROW*COL, nums1)

matrix2 = cl.BufferUINT(context, 'CL_MEM_READ_ONLY, CL_MEM_COPY_HOST_PTR', ROW*COL, nums2)
if not matrix1 then print("Buffer init Failed") end

--cl.EnqueueWriteBuffer(CPUqueue, matrix2, cl.CL_TRUE, 0, cl.CL_UINT, ROW*COL, nums2)
--cl.EnqueueWriteBuffer(GPUqueue, matrix2, cl.CL_TRUE, 0, cl.CL_UINT, ROW*COL, nums2)

matrixOut = cl.BufferUINT(context, 'CL_MEM_WRITE_ONLY', ROW*COL)
if not matrix1 then print("Buffer init Failed") end
endTime = cl.Timer()
print("Querying data Done. Time:", ((endTime - startTime)), "ms")

program = cl.CreateProgramWithSource(context, sample)
cl.BuildProgram(program, devices)
kernel_slow = cl.CreateKernel(program, "matrix_multiply_slow")
kernel_fast = cl.CreateKernel(program, "matrix_multiply_fast")
--nastavenie argumentov
cl.SetKernelArg(kernel_slow, 0, matrix1)
cl.SetKernelArg(kernel_slow, 1, matrix2)
cl.SetKernelArg(kernel_slow, 2, matrixOut)

cl.SetKernelArg(kernel_fast, 0, matrix1)
cl.SetKernelArg(kernel_fast, 1, matrix2)
cl.SetKernelArg(kernel_fast, 2, matrixOut)
--pocitanie na CPU

startTime = cl.Timer()

cl.EnqueueNDRangeKernel(CPUqueue, kernel_slow, 1, ROW*COL)
cl.EnqueueNDRangeKernel(CPUqueue, kernel_slow, 1, ROW*COL)
cl.EnqueueNDRangeKernel(CPUqueue, kernel_slow, 1, ROW*COL)
cl.EnqueueNDRangeKernel(CPUqueue, kernel_slow, 1, ROW*COL)
cl.EnqueueNDRangeKernel(CPUqueue, kernel_slow, 1, ROW*COL)
--ziskanie vysledku
cl.Finish(CPUqueue)
--output = cl.EnqueueMapBuffer(CPUqueue, matrixOut, cl.CL_TRUE, cl.CL_MAP_READ, 0, cl.CL_UINT, ROW*COL)
endTime = cl.Timer()
--[[
for i=0, ROW - 1 do
	print(rozbal(output, 1+(i*ROW), ROW+(i*ROW)))
end
--]]

print("CPU Done. Time:", ((endTime - startTime)/5), "ms")
----------------------------------------------------------------------------------
--pocitanie na GPU
startTime = cl.Timer()
cl.EnqueueNDRangeKernel(GPUqueue, kernel_slow, 1, ROW*COL)
cl.EnqueueNDRangeKernel(GPUqueue, kernel_slow, 1, ROW*COL)
cl.EnqueueNDRangeKernel(GPUqueue, kernel_slow, 1, ROW*COL)
cl.EnqueueNDRangeKernel(GPUqueue, kernel_slow, 1, ROW*COL)
cl.EnqueueNDRangeKernel(GPUqueue, kernel_slow, 1, ROW*COL)
--print(cl.GetCLError())

cl.Finish(GPUqueue)
--output = cl.EnqueueMapBuffer(GPUqueue, matrixOut, cl.CL_TRUE, cl.CL_MAP_READ, 0, cl.CL_UINT, ROW*COL)
endTime = cl.Timer()
--[[
for i=0, ROW - 1 do
	print(rozbal(output, 1+(i*ROW), ROW+(i*ROW)))
end
--]]

print("GPU Done. Time:", ((endTime - startTime)/5), "ms")

----------------------------------------------------------------------------------
print("---------------------------------")
----------------------------------------------------------------------------------

--pocitanie na CPU

startTime = cl.Timer()

cl.EnqueueNDRangeKernel(CPUqueue, kernel_fast, 1, ROW*COL)
cl.EnqueueNDRangeKernel(CPUqueue, kernel_fast, 1, ROW*COL)
cl.EnqueueNDRangeKernel(CPUqueue, kernel_fast, 1, ROW*COL)
cl.EnqueueNDRangeKernel(CPUqueue, kernel_fast, 1, ROW*COL)
cl.EnqueueNDRangeKernel(CPUqueue, kernel_fast, 1, ROW*COL)
--ziskanie vysledku
cl.Finish(CPUqueue)
--output = cl.EnqueueMapBuffer(CPUqueue, matrixOut, cl.CL_TRUE, cl.CL_MAP_READ, 0, cl.CL_UINT, ROW*COL)
endTime = cl.Timer()
--[[
for i=0, ROW - 1 do
	print(rozbal(output, 1+(i*ROW), ROW+(i*ROW)))
end
--]]

print("CPU Done. Time:", ((endTime - startTime)/5), "ms")
----------------------------------------------------------------------------------
--pocitanie na GPU
startTime = cl.Timer()
cl.EnqueueNDRangeKernel(GPUqueue, kernel_fast, 1, ROW*COL)
cl.EnqueueNDRangeKernel(GPUqueue, kernel_fast, 1, ROW*COL)
cl.EnqueueNDRangeKernel(GPUqueue, kernel_fast, 1, ROW*COL)
cl.EnqueueNDRangeKernel(GPUqueue, kernel_fast, 1, ROW*COL)
cl.EnqueueNDRangeKernel(GPUqueue, kernel_fast, 1, ROW*COL)
--print(cl.GetCLError())

cl.Finish(GPUqueue)
--output = cl.EnqueueMapBuffer(GPUqueue, matrixOut, cl.CL_TRUE, cl.CL_MAP_READ, 0, cl.CL_UINT, ROW*COL)
endTime = cl.Timer()
--[[
for i=0, ROW - 1 do
	print(rozbal(output, 1+(i*ROW), ROW+(i*ROW)))
end
--]]

print("GPU Done. Time:", ((endTime - startTime)/5), "ms")

--release
cl.ReleaseContext(context)
cl.ReleaseCommandQueue(CPUqueue)
cl.ReleaseCommandQueue(GPUqueue)
cl.ReleaseKernel(kernel_slow)
cl.ReleaseKernel(kernel_fast)
cl.ReleaseProgram(program)
cl.ReleaseMemObject(matrix1)
cl.ReleaseMemObject(matrix2)
cl.ReleaseMemObject(matrixOut)
