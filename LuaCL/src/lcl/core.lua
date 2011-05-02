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
			local i={},e
			i["CL_DEVICE_TYPE"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_TYPE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_VENDOR_ID"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_VENDOR_ID") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_MAX_COMPUTE_UNITS"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_MAX_COMPUTE_UNITS") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_MAX_CLOCK_FREQUENCY"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_MAX_CLOCK_FREQUENCY") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_ADDRESS_BITS"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_ADDRESS_BITS") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_MAX_READ_IMAGE_ARGS"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_MAX_READ_IMAGE_ARGS") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_MAX_WRITE_IMAGE_ARGS"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_MAX_WRITE_IMAGE_ARGS") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_MAX_SAMPLERS"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_MAX_SAMPLERS") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_MEM_BASE_ADDR_ALIGN"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_MEM_BASE_ADDR_ALIGN") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_MAX_CONSTANT_ARGS"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_MAX_CONSTANT_ARGS") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_MAX_WORK_GROUP_SIZE"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_MAX_WORK_GROUP_SIZE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_IMAGE2D_MAX_WIDTH"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_IMAGE2D_MAX_WIDTH") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_IMAGE2D_MAX_HEIGHT"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_IMAGE2D_MAX_HEIGHT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_IMAGE3D_MAX_WIDTH"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_IMAGE3D_MAX_WIDTH") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_IMAGE3D_MAX_HEIGHT"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_IMAGE3D_MAX_HEIGHT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_IMAGE3D_MAX_DEPTH"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_IMAGE3D_MAX_DEPTH") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_MAX_PARAMETER_SIZE"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_MAX_PARAMETER_SIZE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_PROFILING_TIMER_RESOLUTION"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_PROFILING_TIMER_RESOLUTION") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_MAX_MEM_ALLOC_SIZE"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_MAX_MEM_ALLOC_SIZE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_GLOBAL_MEM_CACHE_SIZE"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_GLOBAL_MEM_CACHE_SIZE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_GLOBAL_MEM_SIZE"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_GLOBAL_MEM_SIZE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_LOCAL_MEM_SIZE"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_LOCAL_MEM_SIZE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_IMAGE_SUPPORT"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_IMAGE_SUPPORT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_ERROR_CORRECTION_SUPPORT"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_ERROR_CORRECTION_SUPPORT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_ENDIAN_LITTLE"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_ENDIAN_LITTLE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_AVAILABLE"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_AVAILABLE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_COMPILER_AVAILABLE"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_COMPILER_AVAILABLE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_SINGLE_FP_CONFIG"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_SINGLE_FP_CONFIG") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_GLOBAL_MEM_CACHE_TYPE"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_GLOBAL_MEM_CACHE_TYPE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_LOCAL_MEM_TYPE"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_LOCAL_MEM_TYPE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_EXECUTION_CAPABILITIES"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_EXECUTION_CAPABILITIES") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_QUEUE_PROPERTIES"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_QUEUE_PROPERTIES") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_MAX_WORK_ITEM_SIZES"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_MAX_WORK_ITEM_SIZES") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_PLATFORM"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_PLATFORM") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_NAME"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_NAME") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_VENDOR"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_VENDOR") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DRIVER_VERSION"],e = cl.DeviceInfo(self.devices[1], "CL_DRIVER_VERSION") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_PROFILE"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_PROFILE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_VERSION"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_VERSION") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_DEVICE_EXTENSIONS"],e = cl.DeviceInfo(self.devices[1], "CL_DEVICE_EXTENSIONS") if e ~= cl.CL_SUCCESS then return nil, e end
			return i,e
		end

	local GetPlatformInfo =
		function()
			local i = {},e
			i["CL_PLATFORM_PROFILE"],e 	= cl.PlatformInfo(self.platform[1], "CL_PLATFORM_PROFILE") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_PLATFORM_VERSION"],e 	= cl.PlatformInfo(self.platform[1], "CL_PLATFORM_VERSION") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_PLATFORM_NAME"],e 	= cl.PlatformInfo(self.platform[1], "CL_PLATFORM_NAME") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_PLATFORM_VENDOR"],e	= cl.PlatformInfo(self.platform[1], "CL_PLATFORM_VENDOR") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_PLATFORM_EXTENSIONS"],e = cl.PlatformInfo(self.platform[1], "CL_PLATFORM_EXTENSIONS") if e ~= cl.CL_SUCCESS then return nil, e end
			return i, e
		end

	local Info =
		function()
			local i={}, e
			i["CL_CONTEXT_REFERENCE_COUNT"],e = cl.ContextInfo(self._data, "CL_CONTEXT_REFERENCE_COUNT") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_CONTEXT_NUM_DEVICES"],e = cl.ContextInfo(self._data, "CL_CONTEXT_NUM_DEVICES") if e ~= cl.CL_SUCCESS then return nil, e end
			i["CL_CONTEXT_DEVICES"],e = cl.ContextInfo(self._data, "CL_CONTEXT_DEVICES") if e ~= cl.CL_SUCCESS then return nil, e end
			--~ i["CL_CONTEXT_PROPERTIES"],e = cl.ContextInfo(self._data, "CL_CONTEXT_PROPERTIES") if e ~= cl.CL_SUCCESS then return nil, e end
			return i, e
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
			return cl.WriteBuffer(self._data, buffer.GetData(), 'CL_TRUE', buffer.GetType(), 0, data)
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

	local CopyBuffer =
		function(src_buffer, dst_buffer, src_offset, dst_offset, size)
			return cl.CopyBuffer(self._data, src_buffer.GetData(), dst_buffer.GetData(), src_buffer.GetType(), src_offset or 0, dst_offset or 0, size or src_buffer.GetSize())
		end

	return {
		CopyBuffer = CopyBuffer,
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
	NewContext = NewContext,
	NewQueue = NewQueue,
	NewKernel = NewKernel,
	NewProgram = NewProgram,
	NewImage3D = NewImage3D,
	NewImage2D = NewImage2D,
	NewSampler = NewSampler,
	NewBuffer = NewBuffer,
	_VERSION = "1.0.0"
}
