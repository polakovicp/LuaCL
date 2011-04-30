local cl = require('LuaCL')

--~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~context
function NewContext(p)
	local self = {_data=0, platform={}, device={}}, e
	self.platform, e = cl.PlatformIDs(1)
	self.device, e = cl.DeviceIDs(self.platform[1], p or "CL_DEVICE_TYPE_CPU")
	self._data, e = cl.CreateContextFromType({"CL_CONTEXT_PLATFORM", self.platform[1]}, p or "CL_DEVICE_TYPE_CPU")
	local Queue =
		function(p)
			return NewQueue(self._data, self.device[1], p)
		end

	local Sampler =
		function(normalized, addresing, filter)
			return NewSampler(self._data, normalized, addresing, filter)
		end

	local GetDeviceInfo =
		function()

		end

	local GetPlatformInfo =
		function()

		end

	local Info =
		function()

		end

	local Buffer =
		function(p, t, d)
			return NewBuffer(self._data, p, t, d)
		end

	local Image2D =
		function(p, f, w, h, t, d)
			return NewImage2D(self._data, p, f, w, h, t, d)
		end

	local Image3D =
		function(p, f, w, h, d, t, dt)
			return NewImage3D(self._data, p, f, w, h, d, t, dt)
		end

	local Build =
		function(source)
			local program, e = NewProgram(self._data, source)
			if e ~= cl.CL_SUCCESS then return nil, e end
			return program, program.Build(self.device)
		end

	local Release =
		function()
			return cl.ReleaseContext(self._data)
		end

	local GetData =
		function()
			return self._data
		end

	return {
		Queue = Queue,
		Sampler = Sampler,
		GetDeviceInfo = GetDeviceInfo,
		GetPlatformInfo = GetPlatformInfo,
		Info = Info,
		Buffer = Buffer,
		Image2D = Image2D,
		Image3D = Image3D,
		Build = Build,
		Release = Release,
		GetData = GetData
	}, e
end

--~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~queue
function NewQueue(context, device, param)
	local self = {_data=0}, e
	self._data, e = cl.CreateQueue(context, device, param or 0)

	local GetData =
		function()
			return self._data
		end

	local Info =
		function()
			local i = {}, e
			i["CL_QUEUE_CONTEXT"],e 		= cl.QueueInfo(self._data, "CL_QUEUE_CONTEXT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_QUEUE_DEVICE"],e 			= cl.QueueInfo(self._data, "CL_QUEUE_DEVICE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_QUEUE_REFERENCE_COUNT"],e = cl.QueueInfo(self._data, "CL_QUEUE_REFERENCE_COUNT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_QUEUE_PROPERTIES"],e		= cl.QueueInfo(self._data, "CL_QUEUE_PROPERTIES") if e ~= cl.CL_SUCCESS then return nil, e end
			return i, e
		end

	local Release =
		function()
			return cl.ReleaseQueue(self._data)
		end

	local NDRange =
		function(kernel, offset, global, loc)
			return cl.NDRangeKernel(self._data, kernel.GetData(), offset, global, loc)
		end

	local Finish =
		function()
			return cl.Finish(self._data)
		end

	local ReadBuffer =
		function(buffer)
			return cl.ReadBuffer(self._data, buffer.GetData(), "CL_TRUE", buffer.GetType(), 0, buffer.GetSize())
		end

	local ReadImage =
		function(img)
			local image_size = img.GetSize()
			local slice_pitch=0
			if image_size[3] == 0 then
				slice_pitch = 0
				image_size[3] = 1
			else
				slice_pitch = image_size[1]*image_size[2]
			end
			return cl.ReadImage(self._data, img.GetData(), 'CL_TRUE', {0,0,0}, image_size, image_size[1], slice_pitch, img.GetType())
		end

	local WriteBuffer =
		function(buffer, data)
			return cl.WriteBuffer(self._data, buffer.GetData(), 'CL_TRUE', buffer.GetType(), offset or 0, data)
		end

	local WriteImage =
		function(image, data)
			local image_size = image.GetSize()
			local slice_pitch = 0
			if image_size[3] == 0 then
				image_size[3] = 1
			else
				slice_pitch = image_size[1]*image_size[2]
			end
			return cl.WriteImage(self._data, image.GetData(), 'CL_TRUE', {0,0,0}, image_size, image_size[1], slice_pitch, image.GetType(), data)
		end

	local CopyImage =
		function(src_image, dst_image, src_origin, dst_origin, src_region)
			src_size = src_image.GetSize()
			if src_size[3] == 0 then
				src_size[3] = 1
			end
			return cl.CopyImage(self._data, src_image.GetData(), dst_image.GetData(), src_origin or {0,0,0}, dst_origin or {0,0,0}, src_region or src_size)
		end
		
	return {
		CopyImage = CopyImage,
		WriteBuffer = WriteBuffer,
		WriteImage = WriteImage,
		ReadBuffer = ReadBuffer,
		ReadImage = ReadImage,
		Finish = Finish,
		NDRange = NDRange,
		GetData=GetData,
		Info=Info,
		Release=Release
		}, e
end



--~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~sampler
function NewSampler(context, normalized_coords, addresing_mode, filter_mode)
	local self = {_data=0}, e
	self._data,e = cl.CreateSampler(context, normalized_coords, addresing_mode, filter_mode)

	local GetData =
		function()
			return self._data
		end

	local Info =
		function()
			local i = {}, e
			i["CL_SAMPLER_CONTEXT"],e = cl.SamplerInfo(self._data, "CL_SAMPLER_CONTEXT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_SAMPLER_REFERENCE_COUNT"],e = cl.SamplerInfo(self._data, "CL_SAMPLER_REFERENCE_COUNT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_SAMPLER_ADDRESSING_MODE"],e = cl.SamplerInfo(self._data, "CL_SAMPLER_ADDRESSING_MODE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_SAMPLER_FILTER_MODE"],e = cl.SamplerInfo(self._data, "CL_SAMPLER_FILTER_MODE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_SAMPLER_NORMALIZED_COORDS"],e = cl.SamplerInfo(self._data, "CL_SAMPLER_NORMALIZED_COORDS") if e ~= cl.CL_SUCCESS then return nil, e end
			return i,e
		end

	local Release =
		function()
			return cl.ReleaseSampler(self._data)
		end

	return {GetData=GetData, Info=Info, Release=Release}, e
end
--~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~buffer
function NewBuffer(context, param, typ, data)
	local self = {_data=0, t=typ, s=0},e
	if type(data) == "table" then
		self.s = #data
	else
		self.s = data
	end

	self._data,e = cl.CreateBuffer(context, param, typ, data)

	local GetData =
		function()
			return self._data
		end

	local Info =
		function()
			local i={}, e
			i["CL_MEM_TYPE"],e = cl.MemInfo(self._data, "CL_MEM_TYPE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_MEM_FLAGS"],e = cl.MemInfo(self._data, "CL_MEM_FLAGS") if e ~= cl.CL_SUCCESS then  return nil, e end
			i["CL_MEM_SIZE"],e = cl.MemInfo(self._data, "CL_MEM_SIZE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_MEM_REFERENCE_COUNT"],e = cl.MemInfo(self._data, "CL_MEM_REFERENCE_COUNT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_MEM_CONTEXT"],e = cl.MemInfo(self._data, "CL_MEM_CONTEXT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_MEM_ASSOCIATED_MEMOBJECT"],e = cl.MemInfo(self._data, "CL_MEM_ASSOCIATED_MEMOBJECT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_MEM_OFFSET"],e = cl.MemInfo(self._data, "CL_MEM_OFFSET") if e ~= cl.CL_SUCCESS then return nil, e end
			return i, e
		end

	local Release =
		function()
			return cl.ReleaseMem(self._data)
		end

	local GetType =
		function()
			return self.t
		end

	local GetSize =
		function()
			return self.s
		end

	return {GetSize = GetSize, GetType=GetType, GetData=GetData, Info=Info, Release=Release}, e
end
--~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~image2D
function NewImage2D(context, param, iformat, width, height, typ, data)
	local self={_data=0, t=typ, w=width, h=height}, e
	if not typ then return nil, -30 end

	self._data,e = cl.CreateImage2D(context, param, iformat, width, height, typ, data)

	local GetData =
		function()
			return self._data
		end

	local Info =
		function()
			local i={}, e
			i["CL_MEM_TYPE"],e = cl.MemInfo(self._data, "CL_MEM_TYPE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_MEM_FLAGS"],e = cl.MemInfo(self._data, "CL_MEM_FLAGS") if e ~= cl.CL_SUCCESS then  return nil, e end
			i["CL_MEM_SIZE"],e = cl.MemInfo(self._data, "CL_MEM_SIZE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_MEM_REFERENCE_COUNT"],e = cl.MemInfo(self._data, "CL_MEM_REFERENCE_COUNT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_MEM_CONTEXT"],e = cl.MemInfo(self._data, "CL_MEM_CONTEXT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_MEM_ASSOCIATED_MEMOBJECT"],e = cl.MemInfo(self._data, "CL_MEM_ASSOCIATED_MEMOBJECT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_MEM_OFFSET"],e = cl.MemInfo(self._data, "CL_MEM_OFFSET") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_IMAGE_FORMAT"],e = cl.ImageInfo(self._data, "CL_IMAGE_FORMAT") if e ~= cl.CL_SUCCESS then return nil,e end
			i["CL_IMAGE_ELEMENT_SIZE"],e = cl.ImageInfo(self._data, "CL_IMAGE_ELEMENT_SIZE") if e ~= cl.CL_SUCCESS then return nil,e end
			i["CL_IMAGE_ROW_PITCH"],e = cl.ImageInfo(self._data, "CL_IMAGE_ROW_PITCH") if e ~= cl.CL_SUCCESS then return nil,e end
			i["CL_IMAGE_SLICE_PITCH"],e = cl.ImageInfo(self._data, "CL_IMAGE_SLICE_PITCH") if e ~= cl.CL_SUCCESS then return nil,e end
			i["CL_IMAGE_WIDTH"],e = cl.ImageInfo(self._data, "CL_IMAGE_WIDTH") if e ~= cl.CL_SUCCESS then return nil,e end
			i["CL_IMAGE_HEIGHT"],e = cl.ImageInfo(self._data, "CL_IMAGE_HEIGHT") if e ~= cl.CL_SUCCESS then return nil,e end
			i["CL_IMAGE_DEPTH"],e = cl.ImageInfo(self._data, "CL_IMAGE_DEPTH") if e ~= cl.CL_SUCCESS then return nil,e end
			return i, e
		end

	local GetType =
		function()
			return self.t
		end

	local GetSize =
		function()
			return {self.w, self.h, 0}
		end

	local Release =
		function()
			return cl.ReleaseMem(self._data)
		end

	return {GetType = GetType, GetSize = GetSize, GetData = GetData, Info=Info, Release=Release},e
end
--~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~image3D
function NewImage3D(context, param, iformat, width, height, depth, typ, data)
	local self = {_data=0, t=typ, w=width, h=height, d=depth}, e
	if not typ then return nil, -30 end
	self._data, e = cl.CreateImage3D(context, param, iformat, width, height, depth, typ, data)

	local GetData =
		function()
			return self._data
		end

	local Info =
		function()
			local i={}, e
			i["CL_MEM_TYPE"],e = cl.MemInfo(self._data, "CL_MEM_TYPE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_MEM_FLAGS"],e = cl.MemInfo(self._data, "CL_MEM_FLAGS") if e ~= cl.CL_SUCCESS then  return nil, e end
			i["CL_MEM_SIZE"],e = cl.MemInfo(self._data, "CL_MEM_SIZE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_MEM_REFERENCE_COUNT"],e = cl.MemInfo(self._data, "CL_MEM_REFERENCE_COUNT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_MEM_CONTEXT"],e = cl.MemInfo(self._data, "CL_MEM_CONTEXT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_MEM_ASSOCIATED_MEMOBJECT"],e = cl.MemInfo(self._data, "CL_MEM_ASSOCIATED_MEMOBJECT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_MEM_OFFSET"],e = cl.MemInfo(self._data, "CL_MEM_OFFSET") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_IMAGE_FORMAT"],e = cl.ImageInfo(self._data, "CL_IMAGE_FORMAT") if e ~= cl.CL_SUCCESS then return nil,e end
			i["CL_IMAGE_ELEMENT_SIZE"],e = cl.ImageInfo(self._data, "CL_IMAGE_ELEMENT_SIZE") if e ~= cl.CL_SUCCESS then return nil,e end
			i["CL_IMAGE_ROW_PITCH"],e = cl.ImageInfo(self._data, "CL_IMAGE_ROW_PITCH") if e ~= cl.CL_SUCCESS then return nil,e end
			i["CL_IMAGE_SLICE_PITCH"],e = cl.ImageInfo(self._data, "CL_IMAGE_SLICE_PITCH") if e ~= cl.CL_SUCCESS then return nil,e end
			i["CL_IMAGE_WIDTH"],e = cl.ImageInfo(self._data, "CL_IMAGE_WIDTH") if e ~= cl.CL_SUCCESS then return nil,e end
			i["CL_IMAGE_HEIGHT"],e = cl.ImageInfo(self._data, "CL_IMAGE_HEIGHT") if e ~= cl.CL_SUCCESS then return nil,e end
			i["CL_IMAGE_DEPTH"],e = cl.ImageInfo(self._data, "CL_IMAGE_DEPTH") if e ~= cl.CL_SUCCESS then return nil,e end
			return i, e
		end

	local GetType =
		function()
			return self.t
		end

	local GetSize =
		function()
			return {self.w, self.h, self.d}
		end

	local Release =
		function()
			return cl.ReleaseMem(self._data)
		end

	return {GetType = GetType, GetSize = GetSize, GetData = GetData, Info=Info, Release=Release},e
end
--~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~program
function NewProgram(context, source)
	local self = {_data=0, device=0}, e
	self._data, e = cl.CreateProgram(context, source)

	local GetData =
		function()
			return self._data
		end

	local Build =
		function(devices)
			self.device = devices[1]
			return cl.BuildProgram(self._data, devices)
		end

	local Kernel =
		function(kernel)
			return NewKernel(self._data, kernel)
		end

	local BuildInfo =
		function()
			local i={}, e
			i["CL_PROGRAM_BUILD_STATUS"],e = cl.BuildInfo(self._data, self.device, "CL_PROGRAM_BUILD_STATUS") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_PROGRAM_BUILD_OPTIONS"],e = cl.BuildInfo(self._data, self.device, "CL_PROGRAM_BUILD_OPTIONS") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_PROGRAM_BUILD_LOG"],e = cl.BuildInfo(self._data, self.device, "CL_PROGRAM_BUILD_LOG") if e ~= cl.CL_SUCCESS then return nil, e end
			return i,e
		end

	local Info =
		function()
			local i={}, e
			i["CL_PROGRAM_REFERENCE_COUNT"],e = cl.ProgramInfo(self._data, "CL_PROGRAM_REFERENCE_COUNT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_PROGRAM_NUM_DEVICES"],e = cl.ProgramInfo(self._data, "CL_PROGRAM_NUM_DEVICES") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_PROGRAM_CONTEXT"],e = cl.ProgramInfo(self._data, "CL_PROGRAM_CONTEXT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_PROGRAM_DEVICES"],e = cl.ProgramInfo(self._data, "CL_PROGRAM_DEVICES") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_PROGRAM_SOURCE"],e = cl.ProgramInfo(self._data, "CL_PROGRAM_SOURCE") if e ~= cl.CL_SUCCESS then return nil, e end
			return i,e
		end

	local Release =
		function()
			return cl.ReleaseProgram(self._data)
		end

	return {GetData=GetData, Build=Build, Kernel=Kernel, BuildInfo=BuildInfo, Info=Info, Release=Release}, e
end
--~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~kernel
function NewKernel(program, kernel)
	local self={_data=0}, e

	self._data, e = cl.CreateKernel(program, kernel)

	local GetData =
		function()
			return self._data
		end

	local SetArgs =
		function(...)
			for index, value in ipairs(arg) do
				local data = value.GetData()
				e = cl.SetKernelArg(self._data, index-1, data)
				if e ~= cl.CL_SUCCESS then return e end
			end
			return e
		end

	local Release =
		function()
			return cl.ReleaseKernel(self._data)
		end

	local Info =
		function()
			local i={}
			i["CL_KERNEL_FUNCTION_NAME"],e = cl.KernelInfo(self._data, "CL_KERNEL_FUNCTION_NAME") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_KERNEL_NUM_ARGS"],e = cl.KernelInfo(self._data, "CL_KERNEL_NUM_ARGS") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_KERNEL_REFERENCE_COUNT"],e = cl.KernelInfo(self._data, "CL_KERNEL_REFERENCE_COUNT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_KERNEL_CONTEXT"],e = cl.KernelInfo(self._data, "CL_KERNEL_CONTEXT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_KERNEL_PROGRAM"],e = cl.KernelInfo(self._data, "CL_KERNEL_PROGRAM") if e ~= cl.CL_SUCCESS then return nil, e end
			return i,e
		end

	return {GetData=GetData, SetArgs=SetArgs, Release=Release, Info=Info}, e
end

--~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~end

return {
	NewContext = NewContext
}
