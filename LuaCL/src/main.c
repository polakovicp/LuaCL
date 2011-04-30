//--~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//--	This file is part of bachelor project:
//--		"OpenCL module for Lua language"
//--
//--	filename	: 	main.c
//--
//--	description	: 	file binds OpenCL library with
//--                    lua language
//--
//--	autor		: 	Patrik Polakovic
//--
//--			26.04.2011
//--
//--~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// gcc -lOpenCL -c -fpic main.c
// gcc -shared -lOpenCL -o libLuaCL.so main.o

#include "main.h"

#define mr(t) \
    if(csize==1)\
        {\
	*((t    *) buff + i) = (t) luaL_checknumber(L, -1);\
	}\
    else\
    {\
        luaL_checktype(L, -1, LUA_TTABLE);\
        for(j=0;j<csize;j++)\
        {\
            lua_rawgeti(L, -1, j+1);\
            *((t    *) buff + j + i) = (t) luaL_checknumber(L, -1);\
            lua_remove(L,-1);\
        }\
    }

#define mw(t)\
    if(csize==1)\
        lua_pushnumber(L, *((t    *) buff + i));\
    else\
    {\
        lua_newtable(L);\
        for(j=0; j<csize;j++)\
        {\
            lua_pushnumber(L, *((t    *) buff + j + i));\
            lua_rawseti(L, -2, j+1);\
        }\
    }

void write_host_buffer(lua_State * L, size_t sz, TYPE t, unsigned int csize, void* buff)
{
    cl_uint i,p,j;

    lua_newtable(L);
    for(i = 0, p = 0; p < sz; i+=csize, p++)
    {
        switch(t)
        {
            case type_cl_char   : mw(cl_char); break;
            case type_cl_uchar  : mw(cl_uchar); break;
            case type_cl_short  : mw(cl_char); break;
            case type_cl_ushort : mw(cl_ushort); break;
            case type_cl_int    : mw(cl_int); break;
            case type_cl_uint   : mw(cl_uint); break;
            case type_cl_long   : mw(cl_long); break;
            case type_cl_ulong  : mw(cl_ulong); break;
            case type_cl_half   : mw(cl_half); break;
            case type_cl_float  : mw(cl_float); break;
            case type_cl_double : mw(cl_double); break;
        }
        lua_rawseti(L, -2, p+1);
    }
}
void read_host_buffer(lua_State * L, int pos, TYPE t, unsigned int csize, void* buff)
{
    cl_uint i,p,j;
    size_t size = lua_objlen(L, pos);
    for(i = 0, p=0; i < size*csize; p++, i+=csize)
    {
        lua_rawgeti(L, pos, p+1);
        switch(t)
        {
            case type_cl_char   : mr(cl_char); break;
            case type_cl_uchar  : mr(cl_uchar); break;
            case type_cl_short  : mr(cl_short); break;
            case type_cl_ushort : mr(cl_ushort); break;
            case type_cl_int    : mr(cl_int); break;
            case type_cl_uint   : mr(cl_uint); break;
            case type_cl_long   : mr(cl_long); break;
            case type_cl_ulong  : mr(cl_ulong); break;
            case type_cl_half   : mr(cl_half); break;
            case type_cl_float  : mr(cl_float); break;
            case type_cl_double : mr(cl_double); break;
        }
        lua_remove(L, -1);
    }
}

size_t get_events(lua_State ** L, cl_event * in_event_list, int pos)
{

    size_t size, i;
    if(lua_istable(*L, pos))
    {
        if((size = lua_objlen(*L, pos)) > 0)
        {
            in_event_list = (cl_event *) malloc(size * sizeof(cl_event));
            if(in_event_list == NULL) return 0;
            for(i = 0; i < size; i++)
            {
                lua_rawgeti(*L, pos, i+1);
                in_event_list[i] = (cl_event) lua_touserdata(*L, -1);
                lua_remove(*L, -1);
            }
            return size;
        }
    }
    return 0;
}
size_t sizeof_t(TYPE t)
{
    switch(t)
    {
        case type_cl_char   : return sizeof(cl_char);
        case type_cl_uchar  : return sizeof(cl_uchar);
        case type_cl_short  : return sizeof(cl_short);
        case type_cl_ushort : return sizeof(cl_ushort);
        case type_cl_int    : return sizeof(cl_int);
        case type_cl_uint   : return sizeof(cl_uint);
        case type_cl_long   : return sizeof(cl_long);
        case type_cl_ulong  : return sizeof(cl_ulong);
        case type_cl_half   : return sizeof(cl_half);
        case type_cl_float  : return sizeof(cl_float);
        case type_cl_double : return sizeof(cl_double);
        case type_cl_char2	: return sizeof(cl_char2);
		case type_cl_char3	: return sizeof(cl_char3);
		case type_cl_char4	: return sizeof(cl_char4);
		case type_cl_char8	: return sizeof(cl_char8);
		case type_cl_char16	: return sizeof(cl_char16);
		case type_cl_uchar2	: return sizeof(cl_uchar2);
		case type_cl_uchar3	: return sizeof(cl_uchar3);
		case type_cl_uchar4	: return sizeof(cl_uchar4);
		case type_cl_uchar8	: return sizeof(cl_uchar8);
		case type_cl_uchar16: return sizeof(cl_uchar16);
		case type_cl_short2	: return sizeof(cl_short2);
		case type_cl_short3	: return sizeof(cl_short3);
		case type_cl_short4	: return sizeof(cl_short4);
		case type_cl_short8	: return sizeof(cl_short8);
		case type_cl_short16: return sizeof(cl_short16);
		case type_cl_ushort2: return sizeof(cl_ushort2);
		case type_cl_ushort3: return sizeof(cl_ushort3);
		case type_cl_ushort4: return sizeof(cl_ushort4);
		case type_cl_ushort8: return sizeof(cl_ushort8);
		case type_cl_ushort16: return sizeof(cl_ushort16);
		case type_cl_int2	: return sizeof(cl_int2);
		case type_cl_int3	: return sizeof(cl_int3);
		case type_cl_int4	: return sizeof(cl_int4);
		case type_cl_int8	: return sizeof(cl_int8);
		case type_cl_int16	: return sizeof(cl_int16);
		case type_cl_uint2	: return sizeof(cl_uint2);
		case type_cl_uint3	: return sizeof(cl_uint3);
		case type_cl_uint4	: return sizeof(cl_uint4);
		case type_cl_uint8	: return sizeof(cl_uint8);
		case type_cl_uint16	: return sizeof(cl_uint16);
		case type_cl_long2	: return sizeof(cl_long2);
		case type_cl_long3	: return sizeof(cl_long3);
		case type_cl_long4	: return sizeof(cl_long4);
		case type_cl_long8	: return sizeof(cl_long8);
		case type_cl_long16	: return sizeof(cl_long16);
		case type_cl_ulong2	: return sizeof(cl_ulong2);
		case type_cl_ulong3	: return sizeof(cl_ulong3);
		case type_cl_ulong4	: return sizeof(cl_ulong4);
		case type_cl_ulong8	: return sizeof(cl_ulong8);
		case type_cl_ulong16: return sizeof(cl_ulong16);
		case type_cl_float2	: return sizeof(cl_float2);
		case type_cl_float3	: return sizeof(cl_float3);
		case type_cl_float4	: return sizeof(cl_float4);
		case type_cl_float8	: return sizeof(cl_float8);
		case type_cl_float16: return sizeof(cl_float16);
		case type_cl_double2: return sizeof(cl_double2);
		case type_cl_double3: return sizeof(cl_double3);
		case type_cl_double4: return sizeof(cl_double4);
		case type_cl_double8: return sizeof(cl_double8);
		case type_cl_double16: return sizeof(cl_double16);
        default: return 0;
    }

}
void * host_alloc(TYPE *t, size_t size, unsigned int * csize)
{
    cl_char * buff_char = NULL;
    cl_uchar * buff_uchar = NULL;
    cl_short * buff_short = NULL;
    cl_ushort * buff_ushort = NULL;
    cl_int * buff_int = NULL;
    cl_uint * buff_uint = NULL;
    cl_long * buff_long = NULL;
    cl_ulong * buff_ulong = NULL;
    cl_half * buff_half = NULL;
    cl_float * buff_float = NULL;
    cl_double * buff_double = NULL;

    switch(*t)
    {
        case type_cl_char2		: *csize=2; *t = type_cl_char; break;
		case type_cl_char3		: *csize=4; *t = type_cl_char; break;
		case type_cl_char4		: *csize=4; *t = type_cl_char; break;
		case type_cl_char8		: *csize=8; *t = type_cl_char; break;
		case type_cl_char16		: *csize=16; *t = type_cl_char; break;
		case type_cl_uchar2		: *csize=2; *t = type_cl_uchar; break;
		case type_cl_uchar3		: *csize=4; *t = type_cl_uchar; break;
		case type_cl_uchar4		: *csize=4; *t = type_cl_uchar; break;
		case type_cl_uchar8		: *csize=8; *t = type_cl_uchar; break;
		case type_cl_uchar16	: *csize=16; *t = type_cl_uchar; break;
		case type_cl_short2		: *csize=2; *t = type_cl_short; break;
		case type_cl_short3		: *csize=4; *t = type_cl_short; break;
		case type_cl_short4		: *csize=4; *t = type_cl_short; break;
		case type_cl_short8		: *csize=8; *t = type_cl_short; break;
		case type_cl_short16	: *csize=16; *t = type_cl_short; break;
		case type_cl_ushort2	: *csize=2; *t = type_cl_ushort; break;
		case type_cl_ushort3	: *csize=4; *t = type_cl_ushort; break;
		case type_cl_ushort4	: *csize=4; *t = type_cl_ushort; break;
		case type_cl_ushort8	: *csize=8; *t = type_cl_ushort; break;
		case type_cl_ushort16	: *csize=16; *t = type_cl_ushort; break;
		case type_cl_int2		: *csize=2; *t = type_cl_int; break;
		case type_cl_int3		: *csize=4; *t = type_cl_int; break;
		case type_cl_int4		: *csize=4; *t = type_cl_int; break;
		case type_cl_int8		: *csize=8; *t = type_cl_int; break;
		case type_cl_int16		: *csize=16; *t = type_cl_int; break;
		case type_cl_uint2		: *csize=2; *t = type_cl_uint; break;
		case type_cl_uint3		: *csize=4; *t = type_cl_uint; break;
		case type_cl_uint4		: *csize=4; *t = type_cl_uint; break;
		case type_cl_uint8		: *csize=8; *t = type_cl_uint; break;
		case type_cl_uint16		: *csize=16; *t = type_cl_uint; break;
		case type_cl_long2		: *csize=2; *t = type_cl_long; break;
		case type_cl_long3		: *csize=4; *t = type_cl_long; break;
		case type_cl_long4		: *csize=4; *t = type_cl_long; break;
		case type_cl_long8		: *csize=8; *t = type_cl_long; break;
		case type_cl_long16		: *csize=16; *t = type_cl_long; break;
		case type_cl_ulong2		: *csize=2; *t = type_cl_ulong; break;
		case type_cl_ulong3		: *csize=4; *t = type_cl_ulong; break;
		case type_cl_ulong4		: *csize=4; *t = type_cl_ulong; break;
		case type_cl_ulong8		: *csize=8; *t = type_cl_ulong; break;
		case type_cl_ulong16	: *csize=16; *t = type_cl_ulong; break;
		case type_cl_float2		: *csize=2; *t = type_cl_float; break;
		case type_cl_float3		: *csize=4; *t = type_cl_float; break;
		case type_cl_float4		: *csize=4; *t = type_cl_float; break;
		case type_cl_float8		: *csize=8; *t = type_cl_float; break;
		case type_cl_float16	: *csize=16; *t = type_cl_float; break;
		case type_cl_double2	: *csize=2; *t = type_cl_double; break;
		case type_cl_double3	: *csize=4; *t = type_cl_double; break;
		case type_cl_double4	: *csize=4; *t = type_cl_double; break;
		case type_cl_double8	: *csize=8; *t = type_cl_double; break;
		case type_cl_double16	: *csize=16; *t = type_cl_double; break;
        default : *csize = 1;
    }

    switch(*t)
    {
        case type_cl_char   : buff_char = (cl_char *) malloc(size*sizeof(cl_char)*(*csize));
            return ((void *) buff_char);
        case type_cl_uchar  : buff_uchar = (cl_uchar *) malloc(size*sizeof(cl_uchar) *(*csize));
            return ((void *) buff_uchar);
        case type_cl_short  : buff_short = (cl_short *) malloc(size*sizeof(cl_short) *(*csize));
            return ((void *) buff_short);
        case type_cl_ushort : buff_ushort = (cl_ushort *) malloc(size*sizeof(cl_ushort) *(*csize));
            return ((void *) buff_ushort);
        case type_cl_int    : buff_int = (cl_int *) malloc(size*sizeof(cl_int) *(*csize));
            return ((void *) buff_int);
        case type_cl_uint   : buff_uint = (cl_uint *) malloc(size*sizeof(cl_uint) *(*csize));
            return ((void *) buff_uint);
        case type_cl_long   : buff_long = (cl_long *) malloc(size*sizeof(cl_long) *(*csize));
            return ((void *) buff_long);
        case type_cl_ulong  : buff_ulong = (cl_ulong *) malloc(size*sizeof(cl_ulong) *(*csize));
            return ((void *) buff_ulong);
        case type_cl_half   : buff_half = (cl_half *) malloc(size*sizeof(cl_half) *(*csize));
            return ((void *) buff_half);
        case type_cl_float  : buff_float = (cl_float *) malloc(size*sizeof(cl_float) *(*csize));
            return ((void *) buff_float);
        case type_cl_double : buff_double = (cl_double *) malloc(size*sizeof(cl_double) *(*csize));
            return ((void *) buff_double);
    }
    return NULL;
}

static int getPlatformIDs(lua_State *L)
{
    cl_int err;
    unsigned int        i;
    cl_platform_id *    platforms;
    cl_uint             num_platforms_ret;

    if(lua_gettop(L) != 0)
        num_platforms_ret = luaL_checkint(L, 1);
    else
        clGetPlatformIDs(0, NULL, &num_platforms_ret);

    platforms = (cl_platform_id *) malloc(num_platforms_ret * sizeof(cl_platform_id));
	err = clGetPlatformIDs(num_platforms_ret, platforms, &num_platforms_ret);

	if(err != CL_SUCCESS)
	    lua_pushnil(L);
	else
	{
        lua_newtable(L);
        for(i = 0; i < num_platforms_ret; i++)
        {
            lua_pushlightuserdata(L, platforms[i]);
            lua_rawseti(L, -2, i+1);
        }
	}
	free(platforms);
	lua_pushnumber(L, err);
	return 2;
}
//----------------------------------------------------------------------
static int getPlatformInfo(lua_State *L)
{
    cl_int err;
    cl_platform_info    info;
    size_t              param_value_size_ret;
    char *              param_value;

    info = (cl_platform_info) get_enum(L, 2, cl_const);

    clGetPlatformInfo((cl_platform_id) lua_touserdata(L, 1), info, 0, NULL, &param_value_size_ret);

    param_value = (char *) malloc(sizeof(char) * param_value_size_ret);
    err = clGetPlatformInfo((cl_platform_id) lua_touserdata(L, 1), info, param_value_size_ret, param_value, NULL);

    (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushstring(L, param_value);
    lua_pushnumber(L, err);
    free(param_value);
    return 2;
}
//----------------------------------------------------------------------
static int getDeviceIDs(lua_State *L)
{
    cl_int err;
    unsigned int    i;
    cl_uint         num_devices_ret;
    cl_uint         num_devices;
    cl_device_id *  devices;
    cl_device_type  device_type = get_enum(L, 2, cl_const);

    if(lua_gettop(L) >= 3)
    {
        num_devices = (cl_uint) luaL_checkint(L, 3);
        devices = (cl_device_id *) malloc(sizeof(cl_device_id) * num_devices);
        err = clGetDeviceIDs((cl_platform_id) lua_touserdata(L, 1), device_type, num_devices, devices, &num_devices_ret);
        num_devices_ret = (num_devices_ret > num_devices) ? num_devices : num_devices_ret;
    }

    else
    {
        clGetDeviceIDs((cl_platform_id) lua_touserdata(L, 1), device_type, 0, NULL, &num_devices_ret);
        devices = (cl_device_id *) malloc(sizeof(cl_device_id) * num_devices_ret);
        err = clGetDeviceIDs((cl_platform_id) lua_touserdata(L, 1), device_type, num_devices_ret, devices, &num_devices_ret);
    }

    if(err != CL_SUCCESS)
	    lua_pushnil(L);
	else
	{
        lua_newtable(L);
        for(i = 0; i < num_devices_ret; i++)
        {
            lua_pushlightuserdata(L, devices[i]);
            lua_rawseti(L, -2, i+1);
        }
	}
	free(devices);
	lua_pushnumber(L, err);
	return 2;
}

static int getDeviceInfo(lua_State *L)
{
    cl_int err = -30;
    unsigned int    i;
    char *          param_value_char;
    cl_platform_id  param_value_platform = 0;
    size_t *        param_value_sizet = NULL;
    cl_ulong        param_value_ulong = 0;
    size_t          param_value_size_ret;

    cl_device_info param_type = get_enum(L, 2, cl_const);

    clGetDeviceInfo((cl_device_id) lua_touserdata(L, 1), param_type, 0, NULL, &param_value_size_ret);

    switch(param_type)
    {
        case CL_DEVICE_TYPE                             :   // cl_device_type
        case CL_DEVICE_VENDOR_ID                        :   // cl_uint
        case CL_DEVICE_MAX_COMPUTE_UNITS                :
        case CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS         :
        case CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR      :
        case CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT     :
        case CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT       :
        case CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG      :
        case CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT     :
        case CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE    :
#ifdef CL_VERSION_1_1
        case CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF      :
        case CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR         :
        case CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT        :
        case CL_DEVICE_NATIVE_VECTOR_WIDTH_INT          :
        case CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG         :
        case CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT        :
        case CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE       :
        case CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF         :
#endif
        case CL_DEVICE_MAX_CLOCK_FREQUENCY              :
        case CL_DEVICE_ADDRESS_BITS                     :
        case CL_DEVICE_MAX_READ_IMAGE_ARGS              :
        case CL_DEVICE_MAX_WRITE_IMAGE_ARGS             :
        case CL_DEVICE_MAX_SAMPLERS                     :
        case CL_DEVICE_MEM_BASE_ADDR_ALIGN              :
        case CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE         :
        case CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE        :
        case CL_DEVICE_MAX_CONSTANT_ARGS                :
        case CL_DEVICE_MAX_WORK_GROUP_SIZE              :   // size_t
        case CL_DEVICE_IMAGE2D_MAX_WIDTH                :
        case CL_DEVICE_IMAGE2D_MAX_HEIGHT               :
        case CL_DEVICE_IMAGE3D_MAX_WIDTH                :
        case CL_DEVICE_IMAGE3D_MAX_HEIGHT               :
        case CL_DEVICE_IMAGE3D_MAX_DEPTH                :
        case CL_DEVICE_MAX_PARAMETER_SIZE               :
        case CL_DEVICE_PROFILING_TIMER_RESOLUTION       :
        case CL_DEVICE_MAX_MEM_ALLOC_SIZE               :   // cl_ulong
        case CL_DEVICE_GLOBAL_MEM_CACHE_SIZE            :
        case CL_DEVICE_GLOBAL_MEM_SIZE                  :
        case CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE         :
        case CL_DEVICE_LOCAL_MEM_SIZE                   :
        case CL_DEVICE_IMAGE_SUPPORT                    :   // cl_bool
#ifdef CL_VERSION_1_1
        case CL_DEVICE_HOST_UNIFIED_MEMORY              :
#endif
        case CL_DEVICE_ERROR_CORRECTION_SUPPORT         :
        case CL_DEVICE_ENDIAN_LITTLE                    :
        case CL_DEVICE_AVAILABLE                        :
        case CL_DEVICE_COMPILER_AVAILABLE               :
        case CL_DEVICE_SINGLE_FP_CONFIG                 :   // cl_device_fp_config
        case CL_DEVICE_GLOBAL_MEM_CACHE_TYPE            :   // cl_device_mem_cache_type
        case CL_DEVICE_LOCAL_MEM_TYPE                   :   // cl_device_local_mem_type
        case CL_DEVICE_EXECUTION_CAPABILITIES           :   // cl_device_exec_capabilities
        case CL_DEVICE_QUEUE_PROPERTIES                 :   // cl_command_queue_properties
            err = clGetDeviceInfo((cl_device_id) lua_touserdata(L, 1), param_type, param_value_size_ret, (void *) &param_value_ulong, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushnumber(L, param_value_ulong);
            lua_pushnumber(L, err);
            return 2;
        case CL_DEVICE_MAX_WORK_ITEM_SIZES              :   // size_t[]
            param_value_sizet = (size_t *) malloc(param_value_size_ret);
            err = clGetDeviceInfo((cl_device_id) lua_touserdata(L, 1), param_type, param_value_size_ret, (void *) param_value_sizet, NULL);

            if(err != CL_SUCCESS)
                lua_pushnil(L);
            else
            {
                lua_newtable(L);
                for(i = 0; i < param_value_size_ret/sizeof(size_t); i++)
                {
                    lua_pushnumber(L, param_value_sizet[i]);
                    lua_rawseti(L, -2, i+1);
                }
            }
            free(param_value_sizet);
            lua_pushnumber(L, err);
            return 2;

        case CL_DEVICE_PLATFORM                         :   // cl_platform_id
            err = clGetDeviceInfo((cl_device_id) lua_touserdata(L, 1), param_type, param_value_size_ret, (void *) &param_value_platform, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushlightuserdata(L, param_value_platform);
            lua_pushnumber(L, err);
            return 2;
        case CL_DEVICE_NAME                             :   // char[]
        case CL_DEVICE_VENDOR                           :
        case CL_DRIVER_VERSION                          :
        case CL_DEVICE_PROFILE                          :
        case CL_DEVICE_VERSION                          :
#ifdef CL_VERSION_1_1
        case CL_DEVICE_OPENCL_C_VERSION                 :
#endif
        case CL_DEVICE_EXTENSIONS                       :
            param_value_char = (char *) malloc(param_value_size_ret);
            err = clGetDeviceInfo((cl_device_id) lua_touserdata(L, 1), param_type, param_value_size_ret, (void *) param_value_char, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushstring(L, (const char *) param_value_char);
            free(param_value_char);
            lua_pushnumber(L, err);
            return 2;
        default                                         :
            lua_pushnil(L);
            lua_pushnumber(L, err);
            return 2;
    }
}
//--------------------------------------------------------------------CONTEXT
static int createContext(lua_State * L)
{
    cl_int err;
    unsigned int i;
    size_t num_properties = lua_objlen(L, 1);
    size_t num_devices = lua_objlen(L, 2);
    cl_context_properties * properties;
    cl_device_id * devices;

    if(num_properties % 2 == 0) num_properties++;
    properties = (cl_context_properties *) malloc(num_properties * sizeof(cl_context_properties));

    for(i = 0; i < num_properties - 1; i++)
    {
        lua_rawgeti(L, 1, i+1);
        if(i % 2 == 0)
        {
            properties[i] = (cl_context_properties) get_enum(L, -1, cl_const);
        }
        else
            properties[i] = (cl_context_properties)(cl_platform_id) lua_touserdata(L, -1);
        lua_remove(L, -1);

    }
    properties[i] = 0;

    devices = (cl_device_id *) malloc(num_devices * sizeof(cl_device_id));
    for(i = 0; i < num_devices; i++)
    {
        lua_rawgeti(L, 2, i+1);
        devices[i] = (cl_device_id) lua_touserdata(L, -1);
        lua_remove(L, -1);
    }

    cl_context context = clCreateContext(properties, num_devices, devices, NULL, NULL, &err);

    (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushlightuserdata(L, context);
    free(devices); free(properties);
    lua_pushnumber(L, err);
    return 2;
}

static int retainContext(lua_State * L)
{
    cl_int err;
    err = clRetainContext((cl_context)lua_touserdata(L, 1));
    lua_pushnumber(L, err);
    return 1;
}

static int releaseContext(lua_State * L)
{
    cl_int err;
    err = clReleaseContext((cl_context)lua_touserdata(L, 1));
    lua_pushnumber(L, err);
    return 1;
}
//----------------------------------------------------------------COMMAND QUEUES
static int createCommandQueue(lua_State * L)
{
    cl_int err;
	cl_command_queue_properties props = 0;
    cl_command_queue command_queue;

	if(lua_gettop(L) == 3)
		props = get_enum(L, 3, cl_const);
	command_queue  = clCreateCommandQueue((cl_context)lua_touserdata(L, 1), (cl_device_id) lua_touserdata(L, 2), props, &err);

	(err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushlightuserdata(L, command_queue);
    lua_pushnumber(L, err);
    return 2;
}

static int retainCommandQueue(lua_State * L)
{
    cl_int err;
    err = clRetainCommandQueue((cl_command_queue) lua_touserdata(L, 1));
    lua_pushnumber(L, err);
    return 1;
}

static int releaseCommandQueue(lua_State * L)
{
    cl_int err;
    err = clReleaseCommandQueue((cl_command_queue) lua_touserdata(L, 1));
    lua_pushnumber(L, err);
    return 1;
}

static int releaseMemObject(lua_State * L)
{
    cl_int err;
    err = clReleaseMemObject((cl_mem) lua_touserdata(L, 1));
    lua_pushnumber(L, err);
    return 1;
}
static int newBuffer(lua_State * L)
{
    cl_int err;
    cl_mem clbuffer;
    cl_uint len = 0, component_size=1;
    void * buffer = NULL;
    cl_mem_flags flags = get_enum(L, 2, cl_const);
    TYPE type = (TYPE) get_enum(L, 3, cl_const);

    if((flags & CL_MEM_USE_HOST_PTR) || (flags & CL_MEM_COPY_HOST_PTR))
    {
        buffer = host_alloc(&type, lua_objlen(L, 4), &component_size);
        read_host_buffer(L, 4, type, component_size, buffer);
    }
    else
    {
        len = luaL_checknumber(L, 4);
    }
    clbuffer = clCreateBuffer((cl_context) lua_touserdata(L, 1), flags, len * sizeof_t(type)*component_size, buffer, &err);

    (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushlightuserdata(L, clbuffer);
    lua_pushnumber(L, err);
    if(buffer != NULL) free(buffer);
    return 2;
}

static int readBuffer(lua_State * L)
{
    cl_int err;
    cl_event event_ret;
    size_t buff_size = (size_t) luaL_checknumber(L, 6);
    unsigned int componenet_size;
    void * buffer = NULL;
    TYPE type = (TYPE) get_enum(L, 4, cl_const);

    cl_uint         events_in_list = 0;
    cl_event *      event_list = NULL;
    cl_bool         param_name = (cl_bool)  get_enum(L, 3, cl_const);
    events_in_list = get_events(&L, event_list, 7);

    buffer = host_alloc(&type, buff_size, &componenet_size);

    err = clEnqueueReadBuffer((cl_command_queue) lua_touserdata(L, 1), (cl_mem) lua_touserdata(L, 2), param_name,
                            (size_t) luaL_checknumber(L, 5) * sizeof_t(type) * componenet_size,
                             buff_size * sizeof_t(type) * componenet_size,
                             buffer, events_in_list, event_list, &event_ret);

    if(err != CL_SUCCESS)
    {
        lua_pushnil(L);
        lua_pushnil(L);
        lua_pushnumber(L, err);
        return 3;
    }

    write_host_buffer(L, buff_size, type, componenet_size, buffer);

    lua_pushlightuserdata(L, event_ret);
    lua_pushnumber(L, err);
    free(buffer);
    return 3;
}

static int writeBuffer(lua_State * L)
{
    cl_event        event;
    cl_int          err;
    unsigned int    component_size;
    cl_uint         events_in_list = 0;
    cl_event *      event_list = NULL;
    void *          buffer = NULL;
    TYPE            type = (TYPE) get_enum(L, 4, cl_const);
    cl_bool         param_name = (cl_bool)  get_enum(L, 3, cl_const);
    events_in_list = get_events(&L, event_list, 7);
    size_t buff_size = (size_t) lua_objlen(L, 6);

    buffer = host_alloc(&type, buff_size, &component_size);

    read_host_buffer(L, 6, type, component_size, buffer);

    err = clEnqueueWriteBuffer((cl_command_queue) lua_touserdata(L, 1), (cl_mem) lua_touserdata(L, 2), param_name,
                                (size_t) luaL_checknumber(L, 5) * sizeof_t(type) * component_size,
                               buff_size * sizeof_t(type) * component_size,
                               buffer, events_in_list, event_list, &event);

    lua_pushlightuserdata(L, event);
    lua_pushnumber(L, err);
    free(buffer);
    return 2;
}

static int copyBuffer(lua_State * L)
{
    cl_int err;
    cl_event event;
    cl_uint         events_in_list = 0;
    cl_event *      event_list = NULL;
    TYPE            type = (TYPE) get_enum(L, 4, cl_const);
    events_in_list = get_events(&L, event_list, 7);

    err = clEnqueueCopyBuffer((cl_command_queue) lua_touserdata(L, 1),
                              (cl_mem) lua_touserdata(L, 2),
                              (cl_mem) lua_touserdata(L, 3),
                              (size_t) luaL_checknumber(L, 5) * sizeof_t(type),
                              (size_t) luaL_checknumber(L, 6) * sizeof_t(type),
                              (size_t) luaL_checknumber(L, 7) * sizeof_t(type),
                              events_in_list,
                              event_list,
                              &event);

    (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushlightuserdata(L, event);
    lua_pushnumber(L, err);
    return 2;
}

static int createProgramWithSource(lua_State * L)
{
    cl_int err;
    const char * source = luaL_checkstring(L, 2);
    cl_program program = clCreateProgramWithSource((cl_context) lua_touserdata(L, 1),
                                                   1,
                                                   &source,
                                                   NULL,
                                                   &err
                                                   );
    if(err != CL_SUCCESS)
        lua_pushnil(L);
    else
        lua_pushlightuserdata(L, program);
    lua_pushnumber(L, err);
    return 2;
}

static int buildProgram(lua_State * L)
{
    unsigned int i;
    cl_int err;
    size_t num = lua_objlen(L, 2);
    cl_device_id * devices = (cl_device_id *) malloc(num * sizeof(cl_device_id));

    for(i = 0; i < num; i++)
    {
        lua_rawgeti(L, 2, i+1);
        devices[i] = (cl_device_id) lua_touserdata(L, -1);
    }

    err = clBuildProgram((cl_program) lua_touserdata(L, 1),
                                num,
                                devices,
                                NULL, NULL, NULL
                                );
    free(devices);
    lua_pushnumber(L, err);
    return 1;
}
static int releaseProgram(lua_State * L)
{
    cl_int err;
    err = clReleaseProgram((cl_program) lua_touserdata(L, 1));
    lua_pushnumber(L, err);
    return 1;
}
//----------------------------------------------------------------PROGRAM OBJECT QUERIES
static int getProgramBuildInfo(lua_State * L)
{
    cl_int err;
    size_t                  size_ret;
    char *                  param_value;
    cl_int                  param_value_status;
    cl_program_build_info   param_name = (cl_program_build_info)  get_enum(L, 3, cl_const);

    switch(param_name)
    {
        case CL_PROGRAM_BUILD_STATUS    :
            err = clGetProgramBuildInfo((cl_program) lua_touserdata(L, 1), (cl_device_id) lua_touserdata(L, 2), param_name, sizeof(cl_int), &param_value_status, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushnumber(L, param_value_status);
            break;
        case CL_PROGRAM_BUILD_OPTIONS   :
        case CL_PROGRAM_BUILD_LOG       :
            clGetProgramBuildInfo((cl_program) lua_touserdata(L, 1), (cl_device_id) lua_touserdata(L, 2), param_name, 0, NULL, &size_ret);
            param_value = (char *) malloc(sizeof(char) * size_ret);
            err = clGetProgramBuildInfo((cl_program) lua_touserdata(L, 1), (cl_device_id) lua_touserdata(L, 2), param_name, size_ret, param_value, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushstring(L, param_value);
            break;
        default                         :
            err = CL_INVALID_VALUE;
            lua_pushnil(L);
            return 1;
    }
    lua_pushnumber(L, err);
    return 2;
}
//----------------------------------------------------------------KERNEL OBJECTS
static int createKernel(lua_State * L)
{
    cl_int err;
    cl_kernel kernel = clCreateKernel((cl_program) lua_touserdata(L, 1), luaL_checkstring(L, 2), &err);
    (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushlightuserdata(L, kernel);
    lua_pushnumber(L, err);
    return 2;
}

static int setKernelArg(lua_State * L)
{
    cl_int err;
    void * arg_value = lua_touserdata(L, 3);

    err = clSetKernelArg((cl_kernel) lua_touserdata(L, 1), (cl_uint) luaL_checknumber(L, 2), sizeof(arg_value), &arg_value);
    lua_pushnumber(L, err);
    return 1;
}

static int releaseKernel(lua_State * L)
{
    cl_int err;
    err = clReleaseKernel((cl_kernel) lua_touserdata(L, 1));
    lua_pushnumber(L, err);
    return 1;
}
//----------------------------------------------------------------EXECUTING KERNELS
static int enqueueNDRangeKernel(lua_State * L)
{
    unsigned int    i;
    cl_int          err;
    cl_event        event;
    cl_uint         events_in_list = 0;
    cl_event *      event_list = NULL;
    cl_uint         work_dim = 0;
    size_t *        global_work_offset = NULL;
    size_t *        global_work_size = NULL;
    size_t *        local_work_size = NULL;

    events_in_list = get_events(&L, event_list, 6);

    if(lua_istable(L, 4))
    {
        work_dim = lua_objlen(L, 4);
        global_work_size = (size_t *) malloc(sizeof(size_t)*work_dim);

        for(i = 0; i < work_dim; i++)
        {
            lua_rawgeti(L, 4, i+1);
            global_work_size[i] = (size_t) luaL_checknumber(L, -1);
            lua_remove(L, -1);
        }
    }
    else
    {
        luaL_typerror(L, 4, "table");
    }
    if(lua_istable(L, 3))
    {
        global_work_offset = (size_t *) malloc(sizeof(size_t)*work_dim);
        for(i = 0; i < work_dim; i++)
        {
            lua_rawgeti(L, 3, i+1);
            global_work_offset[i] = (size_t) luaL_checknumber(L, -1);
            lua_remove(L, -1);
        }
    }
    if(lua_istable(L, 5))
    {
        local_work_size = (size_t *) malloc(sizeof(size_t)*work_dim);
        for(i = 0; i < work_dim; i++)
        {
            lua_rawgeti(L, 5, i+1);
            local_work_size[i] = (size_t) luaL_checknumber(L, -1);
            lua_remove(L, -1);
        }
    }
    err = clEnqueueNDRangeKernel((cl_command_queue) lua_touserdata(L, 1),
                                (cl_kernel) lua_touserdata(L, 2),
                                work_dim,
                                global_work_offset,
                                global_work_size,
                                local_work_size,
                                events_in_list, event_list, &event
                                );
    if(global_work_offset != NULL) free(global_work_offset);
    if(global_work_size != NULL) free(global_work_size);
    if(local_work_size != NULL) free(local_work_size);
    lua_pushlightuserdata(L, event);
    lua_pushnumber(L, err);
    return 2;
}

//----------------------------------------------------------------FLUSH AND FINISH
static int finish(lua_State * L)
{
    cl_int err;
    err = clFinish((cl_command_queue) lua_touserdata(L, 1));
    lua_pushnumber(L, err);
    return 1;
}
//----------------------------------------------------------------

static int getContextInfo(lua_State * L)
{
    cl_int err = -30;
    unsigned int i;
    size_t size_ret;
    cl_uint param_value_uint;
    cl_device_id * param_value_dev;
    //cl_context_properties * param_value_props;                                                                    // !

    cl_context_info param_name = get_enum(L, 2, cl_const);

    switch(param_name)
    {
#ifdef CL_VERSION_1_1
        case CL_CONTEXT_NUM_DEVICES     :
#endif
        case CL_CONTEXT_REFERENCE_COUNT : //cl_uint
            err = clGetContextInfo((cl_context) lua_touserdata(L, 1),
                                   param_name,
                                   (size_t) sizeof(param_value_uint),
                                   &param_value_uint,
                                   &size_ret
                                   );
            if(err != CL_SUCCESS)
                lua_pushnil(L);
            else
                lua_pushnumber(L, param_value_uint);
            break;

        case CL_CONTEXT_DEVICES         : // cl_device_id[]
            clGetContextInfo((cl_context) lua_touserdata(L, 1), param_name, 0, NULL, &size_ret);
            param_value_dev = (cl_device_id *) malloc(size_ret);
            err = clGetContextInfo((cl_context) lua_touserdata(L, 1), param_name, size_ret, param_value_dev, NULL);
            if(err != CL_SUCCESS)
                lua_pushnil(L);
            else
            {
                lua_newtable(L);
                for(i = 0; i < size_ret/sizeof(cl_device_id); i++)
                {
                    lua_pushlightuserdata(L, param_value_dev[i]);
                    lua_rawseti(L, -2, i+1);
                }
            }
            break;
        case CL_CONTEXT_PROPERTIES      : // cl_context_properties[]
            clGetContextInfo((cl_context) lua_touserdata(L, 1), param_name, 0, NULL, &size_ret); // TODO
            break;

        default :
            lua_pushnil(L);
    }
    lua_pushnumber(L, err);
    return 2;
}
//----------------------------------------------------------------
static int getCommandQueueInfo(lua_State * L)
{
    cl_int err;
    cl_context                  param_value_context;
    cl_device_id                param_value_device;
    cl_uint                     param_value_ref_count;
    cl_command_queue_properties param_value_props;

    cl_command_queue_info param_name = (cl_command_queue_info) get_enum(L, 2, cl_const);

    switch(param_name)
    {
        case CL_QUEUE_CONTEXT           : // cl_context
            err = clGetCommandQueueInfo((cl_command_queue) lua_touserdata(L, 1), param_name, sizeof(cl_context), &param_value_context, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushlightuserdata(L, param_value_context);
            break;

        case CL_QUEUE_DEVICE            : // cl_device_id
            err = clGetCommandQueueInfo((cl_command_queue) lua_touserdata(L, 1), param_name, sizeof(cl_device_id), &param_value_device, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushlightuserdata(L, param_value_device);
            break;
        case CL_QUEUE_REFERENCE_COUNT   : // cl_uint
            err = clGetCommandQueueInfo((cl_command_queue) lua_touserdata(L, 1), param_name, sizeof(cl_uint), &param_value_ref_count, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushnumber(L, param_value_ref_count);
            break;
        case CL_QUEUE_PROPERTIES        : // cl_command_queue_properties
            err = clGetCommandQueueInfo((cl_command_queue) lua_touserdata(L, 1), param_name, sizeof(cl_command_queue_properties), &param_value_props, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushnumber(L, param_value_props);
            break;
        default                         :
            lua_pushnil(L);
            break;
    }
    lua_pushnumber(L, err);
    return 2;
}
//----------------------------------------------------------------
/******************************************************************************************************************DEPRECATED*/
#ifndef CL_VERSION_1_1
static int setCommandQueueProperty(lua_State * L)
{
    cl_int err;
    cl_command_queue_properties old_properties = 0;
    cl_command_queue_properties new_properties = (cl_command_queue_properties) get_enum(L, 2, cl_const);

    err = clSetCommandQueueProperty((cl_command_queue) lua_touserdata(L, 1), new_properties, (cl_bool) luaL_checknumber(L, 3), &old_properties );
    (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushnumber(L, old_properties);
    lua_pushnumber(L, err);
    return 2;
}
#endif
/****************************************************************************************************************************/
static int createContextFromType(lua_State * L)
{
    cl_int err;
    unsigned int            i;
    cl_context              context;
    size_t                  num_properties;
    cl_device_type          device_type;
    cl_context_properties * properties;

    device_type     = (cl_device_type) get_enum(L, 2, cl_const);
    num_properties  = lua_objlen(L, 1);

    if(num_properties % 2 == 0) num_properties++;
    properties = (cl_context_properties *) malloc(num_properties * sizeof(cl_context_properties));

    for(i = 0; i < num_properties - 1; i++)
    {
        lua_rawgeti(L, 1, i+1);
        if(i % 2 == 0)
        {
            properties[i] = (cl_context_properties) get_enum(L, -1, cl_const);
        }
        else
            properties[i] = (cl_context_properties)(cl_platform_id) lua_touserdata(L, -1);
        lua_remove(L, -1);
    }
    properties[i] = 0;

    context = clCreateContextFromType(properties, device_type, NULL, NULL, &err);

    (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushlightuserdata(L, context);
    lua_pushnumber(L, err);
    return 2;
}
static int createKernelsInProgram(lua_State * L)
{
    cl_int err;
    unsigned int    i;
    cl_kernel *     kernels;
    cl_uint         num_kernels;

    clCreateKernelsInProgram((cl_program) lua_touserdata(L, 1), 0, NULL, &num_kernels);
    kernels = (cl_kernel *) malloc(num_kernels * sizeof(cl_kernel));
    err = clCreateKernelsInProgram((cl_program)lua_touserdata(L, 1), num_kernels * sizeof(cl_kernel), kernels, NULL);
    if(err != CL_SUCCESS)
        lua_pushnil(L);
    else
    {
        lua_newtable(L);
        for(i = 0; i < num_kernels; i++)
        {
            lua_pushlightuserdata(L, kernels[i]);
            lua_rawseti(L, -2, i+1);
        }
    }

    lua_pushnumber(L, err);
    return 2;
}
//----------------------------------------------------------------
static int newImage2D(lua_State * L)
{
    cl_int err;
    unsigned int    component_size=1;
    cl_mem_flags    flags;
    cl_image_format image_format;
    cl_mem          img;
    void *          buffer = NULL;
    TYPE            type;
    size_t          image_width, image_height, image_row_pitch = 0;

    lua_rawgeti(L, 3, 1);
    image_format.image_channel_order        = (cl_channel_order) get_enum(L, -1, cl_const);
    lua_remove(L, -1);
    lua_rawgeti(L, 3, 2);
    image_format.image_channel_data_type    = (cl_channel_type) get_enum(L, -1, cl_const);
    lua_remove(L, -1);

    flags = get_enum(L, 2, cl_const);

    image_width     = (size_t) lua_tonumber(L, 4);
    image_height    = (size_t) lua_tonumber(L, 5);
    image_row_pitch = image_width;

    if((flags & CL_MEM_USE_HOST_PTR) || (flags & CL_MEM_COPY_HOST_PTR))
    {
	type = (TYPE) get_enum(L, 6, cl_const);
        buffer = host_alloc(&type, lua_objlen(L, 7), &component_size);
	read_host_buffer(L, 7, type, component_size, buffer);
    }
    else
    {
	    image_row_pitch = 0;
	    type = (TYPE) 0;
    }

    img = clCreateImage2D ((cl_context) lua_touserdata(L, 1), flags, &image_format,
                            image_width, image_height, image_row_pitch*sizeof_t(type)*component_size, buffer, &err);

    (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushlightuserdata(L, img);
    lua_pushnumber(L, err);
    if(buffer != NULL)
        free(buffer);
    return 2;
}
static int readImage(lua_State * L)														// TODO BUGGY
{
    unsigned int i, component_size;
    cl_int err;
    cl_event event_ret;

    size_t origin[3];
    size_t region[3];
    size_t buff_size = 1;
    void * buffer = NULL;

    TYPE type = (TYPE) get_enum(L, 8, cl_const);
    size_t row_pitch = luaL_checknumber(L, 6)*sizeof_t(type);
    size_t slice_pitch = luaL_checknumber(L, 7)*sizeof_t(type);
    cl_uint         events_in_list = 0;
    cl_event *      event_list = NULL;
    cl_bool         param_name = (cl_bool)  get_enum(L, 3, cl_const);
    events_in_list = get_events(&L, event_list, 9);

    for(i=0; i < 3; i++)
    {
        lua_rawgeti(L, 4, i+1);
        origin[i] = (size_t) luaL_checknumber(L, -1);
        lua_remove(L, -1);
        lua_rawgeti(L, 5, i+1);
        region[i] = (size_t) luaL_checknumber(L, -1);
        lua_remove(L, -1);
	buff_size  *=  region[i] - origin[i];
    }
    buffer = host_alloc(&type, buff_size, &component_size);

    err = clEnqueueReadImage((cl_command_queue) lua_touserdata(L, 1),
						(cl_mem) lua_touserdata(L, 2), param_name,
						origin, region,
						row_pitch, slice_pitch, buffer,
						events_in_list, event_list, &event_ret);

    if(err != CL_SUCCESS)
    {
        lua_pushnil(L);
        lua_pushnil(L);
        lua_pushnumber(L, err);
        return 3;
    }

    write_host_buffer(L, buff_size, type, component_size, buffer);

    lua_pushlightuserdata(L, event_ret);
    lua_pushnumber(L, err);
    free(buffer);
    return 3;
}
static int writeImage(lua_State * L)
{
    unsigned int i, component_size;
    cl_int err;
    cl_event event_ret;

    size_t origin[3];
    size_t region[3];
    size_t buff_size = 1;
    void * buffer = NULL;

    TYPE type = (TYPE) get_enum(L, 9, cl_const);
    size_t row_pitch = luaL_checknumber(L, 6)*sizeof_t(type);
    size_t slice_pitch = luaL_checknumber(L, 7)*sizeof_t(type);
    cl_uint         events_in_list = 0;
    cl_event *      event_list = NULL;
    cl_bool         param_name = (cl_bool)  get_enum(L, 3, cl_const);
    events_in_list = get_events(&L, event_list, 7);

    for(i=0; i < 3; i++)
    {
        lua_rawgeti(L, 4, i+1);
        origin[i] = (size_t) lua_tonumber(L, -1);
        lua_remove(L, -1);
        lua_rawgeti(L, 5, i+1);
        region[i] = (size_t) lua_tonumber(L, -1);
        lua_remove(L, -1);
	buff_size  *=  region[i] - origin[i];
    }
    buffer = host_alloc(&type, buff_size, &component_size);
    if(buffer == NULL) return 0;

    read_host_buffer(L, 8, type, component_size, buffer);

    err = clEnqueueWriteImage((cl_command_queue) lua_touserdata(L, 1),
						(cl_mem) lua_touserdata(L, 2), param_name,
						origin, region,
						row_pitch, slice_pitch, buffer,
						events_in_list, event_list, &event_ret);

    if(err != CL_SUCCESS)
    {
        lua_pushnil(L);
        lua_pushnumber(L, err);
        return 2;
    }

    lua_pushlightuserdata(L, event_ret);
    lua_pushnumber(L, err);
    free(buffer);
    return 2;
}

static int copyImage(lua_State * L)
{
    unsigned int i;
    cl_int err;
    cl_event event_ret;

    size_t src_origin[3];
    size_t dst_origin[3];
    size_t region[3];

    cl_uint         events_in_list = 0;
    cl_event *      event_list = NULL;
    events_in_list = get_events(&L, event_list, 7);

    for(i=0; i < 3; i++)
    {
        lua_rawgeti(L, 4, i+1);
        src_origin[i] = (size_t) luaL_checknumber(L, -1);
        lua_remove(L, -1);
        lua_rawgeti(L, 5, i+1);
        dst_origin[i] = (size_t) luaL_checknumber(L, -1);
        lua_remove(L, -1);
        lua_rawgeti(L, 6, i+1);
        region[i] = (size_t) luaL_checknumber(L, -1);
        lua_remove(L, -1);
    }
    err = clEnqueueCopyImage((cl_command_queue) lua_touserdata(L, 1),
						(cl_mem) lua_touserdata(L, 2), (cl_mem) lua_touserdata(L, 3),
						src_origin, dst_origin, region,
						events_in_list, event_list, &event_ret);

    if(err != CL_SUCCESS)
    {
        lua_pushnil(L);
        lua_pushnumber(L, err);
        return 2;
    }

    lua_pushlightuserdata(L, event_ret);
    lua_pushnumber(L, err);
    return 2;
}
static int newImage3D(lua_State * L)
{
    cl_int err;
    unsigned int    component_size = 1;
    cl_mem_flags    flags;
    cl_image_format image_format;
    cl_mem          img;
    void *          buffer = NULL;
    TYPE            type;
    size_t          image_width, image_height, image_depth, image_row_pitch = 0, image_slice_pitch = 0;

    lua_rawgeti(L, 3, 1);
    image_format.image_channel_order        = (cl_channel_order) get_enum(L, -1, cl_const);
    lua_remove(L, -1);
    lua_rawgeti(L, 3, 2);
    image_format.image_channel_data_type    = (cl_channel_type) get_enum(L, -1, cl_const);
    lua_remove(L, -1);

    flags = get_enum(L, 2, cl_const);

    image_width     		= (size_t) luaL_checknumber(L, 4);
    image_height    		= (size_t) luaL_checknumber(L, 5);
    image_depth		= (size_t) luaL_checknumber(L, 6);
    image_row_pitch    	= image_width;
    image_slice_pitch 	= image_row_pitch*image_height;

    if((flags & CL_MEM_USE_HOST_PTR) || (flags & CL_MEM_COPY_HOST_PTR))
    {
	type = (TYPE) get_enum(L, 7, cl_const);
        buffer = host_alloc(&type, lua_objlen(L, 8), &component_size);
	read_host_buffer(L, 8, type, component_size, buffer);
    }
    else
    {
	        image_row_pitch    	= 0;
		image_slice_pitch 	= 0;
	}

    img = clCreateImage3D ((cl_context) lua_touserdata(L, 1), flags, &image_format,
                            image_width, image_height, image_depth, image_row_pitch*sizeof_t(type) * component_size,
				image_slice_pitch*sizeof_t(type) * component_size,
				buffer, &err);

    (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushlightuserdata(L, img);
    lua_pushnumber(L, err);
    free(buffer);
    return 2;
}

static int getSupportedImageFormats(lua_State * L)
{
    cl_int err;
    unsigned int        i;
    cl_mem_object_type  image_type;
    cl_mem_flags        flags;
    cl_uint             num_image_formats;
    cl_image_format *   image_formats;

    flags       = (cl_mem_flags) get_enum(L, 2, cl_const);
    image_type  = (cl_mem_object_type) get_enum(L, 3, cl_const);

    clGetSupportedImageFormats((cl_context) lua_touserdata(L, 1), flags, image_type, 0, NULL, &num_image_formats);
    image_formats = (cl_image_format *) malloc(sizeof(cl_image_format) * num_image_formats);

    err = clGetSupportedImageFormats((cl_context) lua_touserdata(L, 1), flags, image_type, num_image_formats, image_formats, NULL);
    if(err != CL_SUCCESS)
    {
        lua_pushnil(L);
    }
    else
    {
        lua_newtable(L);
        for(i = 0; i < num_image_formats; i++)
        {
	    lua_newtable(L);
            lua_pushnumber(L, image_formats[i].image_channel_order);
            lua_rawseti(L, -2, 1);
            lua_pushnumber(L, image_formats[i].image_channel_data_type);
            lua_rawseti(L, -2, 2);
	    lua_rawseti(L, -2, i+1);
        }
    }
    free(image_formats);
    lua_pushnumber(L, err);
    return 2;
}
static int createSampler(lua_State * L)
{
    cl_int err;
    cl_sampler          sampler;
    cl_filter_mode      filter_mode;
    cl_addressing_mode  addressing_mode;
    cl_bool             normalized_coords;

    normalized_coords   = (cl_bool) get_enum(L, 2, cl_const);
    addressing_mode     = (cl_addressing_mode) get_enum(L, 3, cl_const);
    filter_mode         = (cl_filter_mode) get_enum(L, 4, cl_const);

    sampler = clCreateSampler((cl_context) lua_touserdata(L, 1), normalized_coords, addressing_mode, filter_mode, &err);
    (err != CL_SUCCESS) ? (lua_pushnil(L)) : (lua_pushlightuserdata(L, sampler));
    lua_pushnumber(L, err);
    return 2;
}
static int retainMemObject(lua_State * L)
{
    cl_int err;
    err = clRetainMemObject((cl_mem) lua_touserdata(L, 1));
    lua_pushnumber(L, err);
    return 1;
}
static int getMemObjectInfo(lua_State * L)
{
    cl_int err=-30;
    cl_mem_object_type  param_name_object_type;
    cl_mem_flags        param_name_flags;
    size_t              param_name_size;
    cl_uint             param_name_uint;
    cl_context          param_name_context;
    cl_mem              param_name_mem;
    cl_mem_info param_name = (cl_mem_info) get_enum(L, 2, cl_const);

    switch(param_name)
    {
        case CL_MEM_TYPE            : // cl_mem_object_type
            err = clGetMemObjectInfo((cl_mem) lua_touserdata(L, 1), param_name, sizeof(cl_mem_object_type), &param_name_object_type, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushnumber(L, param_name_object_type);
            break;
        case CL_MEM_FLAGS           : // cl_mem_flags
            err = clGetMemObjectInfo((cl_mem) lua_touserdata(L, 1), param_name, sizeof(cl_mem_flags), &param_name_flags, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushnumber(L, param_name_flags);
            break;
        case CL_MEM_SIZE            : // size_t
#ifdef CL_VERSION_1_1
        case CL_MEM_OFFSET          :
#endif
            err = clGetMemObjectInfo((cl_mem) lua_touserdata(L, 1), param_name, sizeof(size_t), &param_name_size, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushnumber(L, param_name_size);
            break;
        //case CL_MEM_HOST_PTR        : // void *
        //~ case CL_MEM_MAP_COUNT       : // cl_uint
        case CL_MEM_REFERENCE_COUNT :
            err = clGetMemObjectInfo((cl_mem) lua_touserdata(L, 1), param_name, sizeof(cl_uint), &param_name_uint, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushnumber(L, param_name_uint);
            break;
        case CL_MEM_CONTEXT         : // cl_context
            err = clGetMemObjectInfo((cl_mem) lua_touserdata(L, 1), param_name, sizeof(cl_context), &param_name_context, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushlightuserdata(L, param_name_context);
            break;
#ifdef CL_VERSION_1_1
        case CL_MEM_ASSOCIATED_MEMOBJECT    :
            err = clGetMemObjectInfo((cl_mem) lua_touserdata(L, 1), param_name, sizeof(cl_mem), &param_name_mem, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushlightuserdata(L, param_name_mem);
            break;
#endif
        default                     :
            lua_pushnil(L);
    }
    lua_pushnumber(L, err);
    return 2;
}
static int getImageInfo(lua_State * L)
{
    cl_int err;
    cl_image_format param_value_format;
    size_t          param_value_size;

    cl_image_info param_name = (cl_image_info) get_enum(L, 2, cl_const);

    switch(param_name)
    {
        case CL_IMAGE_FORMAT        : // cl_image_format                                                        // !
            err = clGetImageInfo((cl_mem) lua_touserdata(L, 1), param_name, sizeof(cl_image_format), &param_value_format, NULL);
            if(err != CL_SUCCESS) {lua_pushnil(L); return 1;}
            lua_newtable(L);
            lua_pushnumber(L, param_value_format.image_channel_order); lua_rawseti(L, -2, 1);
            lua_pushnumber(L, param_value_format.image_channel_data_type); lua_rawseti(L, -2, 2);
            break;
        case CL_IMAGE_ELEMENT_SIZE  : // size_t
        case CL_IMAGE_ROW_PITCH     :
        case CL_IMAGE_SLICE_PITCH   :
        case CL_IMAGE_WIDTH         :
        case CL_IMAGE_HEIGHT        :
        case CL_IMAGE_DEPTH         :
            err = clGetImageInfo((cl_mem) lua_touserdata(L, 1), param_name, sizeof(size_t), &param_value_size, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushnumber(L, param_value_size);
            break;
        default                     :
            lua_pushnil(L);

    }
    lua_pushnumber(L, err);
    return 2;
}
static int retainSampler(lua_State * L)
{
    cl_int err;
    err = clRetainSampler((cl_sampler) lua_touserdata(L, 1));
    lua_pushnumber(L, err);
    return 1;
}
static int releaseSampler(lua_State * L)
{
    cl_int err;
    err = clReleaseSampler((cl_sampler) lua_touserdata(L, 1));
    lua_pushnumber(L, err);
    return 1;
}
static int getSamplerInfo(lua_State * L)
{
    cl_int err;
    cl_context  param_value_context;
    cl_uint     param_value_uint;

    cl_sampler_info param_name = (cl_sampler_info) get_enum(L, 2, cl_const);

    switch(param_name)
    {
        case CL_SAMPLER_CONTEXT             :   // cl_context
            err = clGetSamplerInfo((cl_sampler) lua_touserdata(L, 1), param_name, sizeof(cl_context), &param_value_context, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushlightuserdata(L, param_value_context);
            break;
        case CL_SAMPLER_REFERENCE_COUNT     :   // cl_uint
        case CL_SAMPLER_ADDRESSING_MODE     :
        case CL_SAMPLER_FILTER_MODE         :
        case CL_SAMPLER_NORMALIZED_COORDS   :
            err = clGetSamplerInfo((cl_sampler) lua_touserdata(L, 1), param_name, sizeof(cl_uint), &param_value_uint, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushnumber(L, param_value_uint);
            break;
        default                             :
            lua_pushnil(L);
    }
    lua_pushnumber(L, err);
    return 2;
}
static int retainProgram(lua_State * L)
{
    cl_int err;
    err = clRetainProgram ((cl_program) lua_touserdata(L, 1));
    lua_pushnumber(L, err);
    return 1;
}
static int unloadCompiler(lua_State * L)
{
    cl_int err;
    err = clUnloadCompiler();
    lua_pushnumber(L, err);
    return 1;
}
static int getProgramInfo(lua_State * L)
{
    cl_int err;
    unsigned int    i;
    size_t          size_ret;
    cl_uint         param_value_uint;
    cl_context      param_value_context;
    cl_device_id*   param_value_device;
    char*           param_value_char;

    cl_program_info param_name = (cl_program_info) get_enum(L, 2, cl_const);

    switch(param_name)
    {
        case CL_PROGRAM_REFERENCE_COUNT : // cl_uint
        case CL_PROGRAM_NUM_DEVICES     :
            err = clGetProgramInfo((cl_program) lua_touserdata(L, 1), param_name, sizeof(cl_uint), &param_value_uint, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushnumber(L, param_value_uint);
            break;
        case CL_PROGRAM_CONTEXT         : // cl_context
            err = clGetProgramInfo((cl_program) lua_touserdata(L, 1), param_name, sizeof(cl_context), &param_value_context, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushlightuserdata(L, param_value_context);
            break;
        case CL_PROGRAM_DEVICES         : // cl_device_id[]
            clGetProgramInfo((cl_program) lua_touserdata(L, 1), param_name, 0, NULL, &size_ret);
            param_value_device = (cl_device_id *) malloc(sizeof(cl_device_id) * size_ret);
            err = clGetProgramInfo((cl_program) lua_touserdata(L, 1), param_name, sizeof(cl_device_id) * size_ret, param_value_device, NULL);
            if(err != CL_SUCCESS)
                lua_pushnil(L);
            else
            {
                lua_newtable(L);
                for(i = 0; i < size_ret/(sizeof(cl_device_id)); i++)
                {
                    lua_pushlightuserdata(L, param_value_device[i]);
                    lua_rawseti(L, -2, i+1);
                }
            }
            free(param_value_device);
            break;
        case CL_PROGRAM_SOURCE          : // char[]
            clGetProgramInfo((cl_program) lua_touserdata(L, 1), param_name, 0, NULL, &size_ret);
            param_value_char = (char *) malloc(sizeof(char) * size_ret);
            err = clGetProgramInfo((cl_program) lua_touserdata(L, 1), param_name, sizeof(char) * size_ret, param_value_char, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushstring(L, param_value_char);
            free(param_value_char);
            break;
//        case CL_PROGRAM_BINARY_SIZES    : // TODO, not support for clCreateProgramWithBinary
//            break;
//        case CL_PROGRAM_BINARIES        :
//            break;
        default                         :
            lua_pushnil(L);
    }
    lua_pushnumber(L, err);
    return 2;
}
static int retainKernel(lua_State * L)
{
    cl_int err;
    err = clRetainKernel((cl_kernel) lua_touserdata(L, 1));
    lua_pushnumber(L, err);
    return 1;
}
static int getKernelInfo(lua_State * L)
{
    cl_int err;
    size_t          size_ret;
    char*           param_value_char;
    cl_uint         param_value_uint;
    cl_context      param_value_context;
    cl_program      param_value_program;
    cl_kernel_info  param_name = (cl_kernel_info) get_enum(L, 2, cl_const);

    switch(param_name)
    {
        case CL_KERNEL_FUNCTION_NAME    :
            clGetKernelInfo((cl_kernel) lua_touserdata(L, 1), param_name, 0, NULL, &size_ret);
            param_value_char = (char *) malloc(sizeof(char) * size_ret);
            err = clGetKernelInfo((cl_kernel) lua_touserdata(L, 1), param_name, sizeof(char) * size_ret, param_value_char, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushstring(L, param_value_char);
            free(param_value_char);
            break;
        case CL_KERNEL_NUM_ARGS         :
        case CL_KERNEL_REFERENCE_COUNT  :
            err = clGetKernelInfo((cl_kernel) lua_touserdata(L, 1), param_name, sizeof(cl_uint), &param_value_uint, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushnumber(L, param_value_uint);
            break;
        case CL_KERNEL_CONTEXT          :
            err = clGetKernelInfo((cl_kernel) lua_touserdata(L, 1), param_name, sizeof(cl_context), &param_value_context, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushlightuserdata(L, param_value_context);
            break;
        case CL_KERNEL_PROGRAM          :
            err = clGetKernelInfo((cl_kernel) lua_touserdata(L, 1), param_name, sizeof(cl_program), &param_value_program, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushlightuserdata(L, param_value_context);
            break;
        default                         :
            lua_pushnil(L);
    }
    lua_pushnumber(L, err);
    return 2;
}
static int getKernelWorkGroupInfo(lua_State * L)
{
    cl_int err;
    unsigned int                i;
    size_t                      param_value_size[3];
    cl_ulong                    param_value_ulong;
    cl_kernel_work_group_info   param_name = (cl_kernel_work_group_info) get_enum(L, 3, cl_const);
    switch(param_name)
    {
#ifdef CL_VERSION_1_1
        case CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE :
#endif
        case CL_KERNEL_WORK_GROUP_SIZE          : // size_t
            err = clGetKernelWorkGroupInfo((cl_kernel) lua_touserdata(L, 1), (cl_device_id) lua_touserdata(L, 2),
                                            param_name, sizeof(size_t), &param_value_size[0], NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushnumber(L, param_value_size[0]);
            break;
        case CL_KERNEL_COMPILE_WORK_GROUP_SIZE  : // size_t[3]
            err = clGetKernelWorkGroupInfo((cl_kernel) lua_touserdata(L, 1), (cl_device_id) lua_touserdata(L, 2),
                                            param_name, sizeof(size_t) * 3, param_value_size, NULL);
            if(err != CL_SUCCESS)
                lua_pushnil(L);
            else
            {
                lua_newtable(L);
                for(i = 0; i < 3; i++)
                {
                    lua_pushnumber(L, param_value_size[i]);
                    lua_rawseti(L, -2, i+1);
                }
            }
            break;
#ifdef CL_VERSION_1_1
        case CL_KERNEL_PRIVATE_MEM_SIZE         :
#endif
        case CL_KERNEL_LOCAL_MEM_SIZE           : // cl_ulong
            err = clGetKernelWorkGroupInfo((cl_kernel) lua_touserdata(L, 1), (cl_device_id) lua_touserdata(L, 2),
                                            param_name, sizeof(cl_ulong), &param_value_ulong, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushnumber(L, param_value_ulong);
            break;
        default                                 :
            lua_pushnil(L);
    }
    lua_pushnumber(L, err);
    return 2;
}
static int waitForEvents(lua_State * L)
{
    cl_int err;
    unsigned int    i;
    cl_uint         num_events;
    cl_event*       event_list;

	if(!lua_istable(L, 1))
		luaL_typerror(L, 1, "table");

    num_events = lua_objlen(L, 1);
    event_list = (cl_event *) malloc(num_events * sizeof(cl_event));

    for(i = 0; i < num_events; i++)
    {
        lua_rawgeti(L, 1, i+1);
        event_list[i] = (cl_event) lua_touserdata(L, -1);
    }

    err = clWaitForEvents(num_events, (const cl_event *) event_list);
    lua_pushnumber(L, err);
    return 1;
}
static int getEventInfo(lua_State * L)
{
    cl_int err;
    cl_int              param_value_int;
    cl_uint             param_value_uint;
    cl_command_queue    param_value_queue;
    cl_context          param_value_context;
    cl_event_info       param_name = (cl_event_info) get_enum(L, 2, cl_const);

    switch(param_name)
    {
        case CL_EVENT_COMMAND_QUEUE             :
            err = clGetEventInfo((cl_event) lua_touserdata(L, 1), param_name, sizeof(cl_command_queue), &param_value_queue, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushlightuserdata(L, param_value_queue);
            break;
        case CL_EVENT_COMMAND_EXECUTION_STATUS  :
            err = clGetEventInfo((cl_event) lua_touserdata(L, 1), param_name, sizeof(cl_int), &param_value_int, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushnumber(L, param_value_int);
            break;
#ifdef CL_VERSION_1_1
        case CL_EVENT_CONTEXT                   :
            err = clGetEventInfo((cl_event) lua_touserdata(L, 1), param_name, sizeof(cl_context), &param_value_context, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushlightuserdata(L, param_value_queue);
            break;
#endif
        case CL_EVENT_COMMAND_TYPE              :
        case CL_EVENT_REFERENCE_COUNT           :
            err = clGetEventInfo((cl_event) lua_touserdata(L, 1), param_name, sizeof(cl_uint), &param_value_uint, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushnumber(L, param_value_uint);
            break;
        default                                 :
            lua_pushnil(L);
    }
    lua_pushnumber(L, err);
    return 2;
}
static int retainEvent(lua_State * L)
{
    cl_int err;
    err = clRetainEvent((cl_event) lua_touserdata(L, 1));
    lua_pushnumber(L, err);
    return 1;
}
static int releaseEvent(lua_State * L)
{
    cl_int err;
    err = clReleaseEvent((cl_event) lua_touserdata(L, 1));
    lua_pushnumber(L, err);
    return 1;
}
static int getEventProfilingInfo(lua_State * L)
{
    cl_int err;
    cl_ulong            param_value_ulong;
    cl_profiling_info   param_name = (cl_profiling_info) get_enum(L, 2, cl_const);

    switch(param_name)
    {
        case CL_PROFILING_COMMAND_QUEUED    :
        case CL_PROFILING_COMMAND_SUBMIT    :
        case CL_PROFILING_COMMAND_START     :
        case CL_PROFILING_COMMAND_END       :
            err = clGetEventProfilingInfo((cl_event) lua_touserdata(L, 1), param_name, sizeof(cl_ulong), &param_value_ulong, NULL);
            (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushnumber(L, param_value_ulong);
            break;
        default                             :
            lua_pushnil(L);
    }
    lua_pushnumber(L, err);
    return 2;
}
static int flush(lua_State * L)
{
    cl_int err;
    err = clFlush((cl_command_queue) lua_touserdata(L, 1));
    lua_pushnumber(L, err);
    return 1;
}
#ifdef CL_VERSION_1_1
static int createSubBuffer(lua_State * L)
{
	cl_int err;
	cl_mem sub;
	cl_buffer_region buffer_create_info;
	cl_buffer_create_type param = get_enum(L, 3, cl_const);
	cl_mem_flags flags = get_enum(L, 2, cl_const);
	TYPE type = (TYPE) get_enum(L, 5, cl_const);

	lua_rawgeti(L, 4, 1);
	buffer_create_info.origin = luaL_checknumber(L, -1) * sizeof_t(type);
	lua_remove(L, -1);
	lua_rawgeti(L, 4, 2);
	buffer_create_info.size = luaL_checknumber(L, -1) * sizeof_t(type);
	lua_remove(L, -1);

	sub = clCreateSubBuffer((cl_mem) lua_touserdata(L, 1),
						flags,  param, (const void *) &buffer_create_info,
						&err
						);
	(err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushlightuserdata(L, sub);
	lua_pushnumber(L, err);
	return 2;
}
#endif
static int enqueueCopyImageToBuffer(lua_State * L)
{
    cl_int err;
    unsigned int i;
    size_t      src_origin[3];
    size_t      region[3];
    cl_event    event;
    cl_uint     events_in_list = 0;
    cl_event *  event_list = NULL;
    TYPE type = (TYPE) get_enum(L, 4, cl_const);
    for(i = 0; i < 3; i++)
    {
        lua_rawgeti(L, 5, i+1); src_origin[i]   = (size_t) luaL_checknumber(L, -1); lua_remove(L, -1);
        lua_rawgeti(L, 6, i+1); region[i]       = (size_t) luaL_checknumber(L, -1); lua_remove(L, -1);
    }

    events_in_list = get_events(&L, event_list, 8);
    err = clEnqueueCopyImageToBuffer((cl_command_queue) lua_touserdata(L, 1), (cl_mem) lua_touserdata(L, 2), (cl_mem) lua_touserdata(L, 3),
                                     src_origin, region, (size_t) luaL_checknumber(L, 7) * sizeof_t(type),
                                     events_in_list, event_list, &event
                                     );

    (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushlightuserdata(L, event);
    lua_pushnumber(L, err);
    return 2;
}
static int enqueueCopyBufferToImage(lua_State * L)
{
    cl_int err;
    unsigned int i;
    size_t      dst_origin[3];
    size_t      region[3];
    cl_event    event;
    cl_uint     events_in_list = 0;
    cl_event *  event_list = NULL;
    TYPE type = (TYPE) get_enum(L, 4, cl_const);

    for(i = 0; i < 3; i++)
    {
        lua_rawgeti(L, 6, i+1); dst_origin[i]   = (size_t) luaL_checknumber(L, -1); lua_remove(L, -1);
        lua_rawgeti(L, 7, i+1); region[i]       = (size_t) luaL_checknumber(L, -1); lua_remove(L, -1);
    }

    events_in_list = get_events(&L, event_list, 8);

    err = clEnqueueCopyBufferToImage((cl_command_queue) lua_touserdata(L, 1), (cl_mem) lua_touserdata(L, 2), (cl_mem) lua_touserdata(L, 3),
                                     (size_t) luaL_checknumber(L, 5)*sizeof_t(type), dst_origin, region,
                                     events_in_list, event_list, &event
                                     );

    (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushlightuserdata(L, event);
    lua_pushnumber(L, err);
    return 2;
}

static int enqueueTask(lua_State * L)
{
    cl_int err;
    cl_event event;
    cl_uint         events_in_list = 0;
    cl_event *      event_list = NULL;

    events_in_list = get_events(&L, event_list, 3);

    err = clEnqueueTask((cl_command_queue) lua_touserdata(L, 1), (cl_kernel) lua_touserdata(L, 2), events_in_list, event_list, &event);
    (err != CL_SUCCESS) ? lua_pushnil(L) : lua_pushlightuserdata(L, event);
    lua_pushnumber(L, err);
    return 2;
}
//----------------------------------------------------------------
static int enqueueMarker(lua_State * L)
{
    cl_int err;
    cl_event event;

    err = clEnqueueMarker((cl_command_queue) lua_touserdata(L, 1), &event);

    if(err != CL_SUCCESS)
        lua_pushnil(L);
    else
        lua_pushlightuserdata(L, event);
    lua_pushnumber(L, err);
    return 2;
}
//----------------------------------------------------------------
static int enqueueWaitForEvents(lua_State * L)
{
    cl_int err;
    unsigned int i;
    cl_event * event_list;
    cl_uint num_events = (cl_uint) lua_objlen(L, 2);

	if(!lua_istable(L, 2))
		luaL_typerror(L, 2, "table");

    event_list = (cl_event *) malloc(num_events * sizeof(cl_event));

    for(i = 0; i < num_events; i++)
    {
        lua_rawgeti(L, 2, i+1);
        event_list[i] = (cl_event) lua_touserdata(L, -1);
        lua_remove(L, -1);
    }

    err = clEnqueueWaitForEvents((cl_command_queue) lua_touserdata(L, 1), num_events, event_list);
    lua_pushnumber(L, err);
    return 2;
}
//----------------------------------------------------------------
static int enqueueBarrier(lua_State * L)
{
    cl_int err;
    err = clEnqueueBarrier((cl_command_queue) lua_touserdata(L, 1));
    lua_pushnumber(L, err);
    return 1;
}

//---------------------------------------------------------
static const struct luaL_Reg LuaCL [] = {
//    #ifdef _WIN32
//    {"Time"                         , performaceTime},
//    #endif
    {"PlatformIDs"                  , getPlatformIDs},
    {"PlatformInfo"                 , getPlatformInfo},
    {"DeviceIDs"                    , getDeviceIDs},
    {"DeviceInfo"                   , getDeviceInfo},
    {"CreateContext"                , createContext},
    {"CreateContextFromType"        , createContextFromType},
    {"RetainContext"                , retainContext},
    {"ReleaseContext"               , releaseContext},
    {"CreateQueue"                  , createCommandQueue},
    {"RetainQueue"                  , retainCommandQueue},
    {"ReleaseQueue"                 , releaseCommandQueue},

    {"CreateBuffer"                 , newBuffer},
    {"ReadBuffer"                   , readBuffer},
    {"WriteBuffer"                  , writeBuffer},
    {"CopyBuffer"                   , copyBuffer},

    {"CreateProgram"                , createProgramWithSource},
    {"BuildProgram"                 , buildProgram},
    {"BuildInfo"                    , getProgramBuildInfo},
    {"CreateKernel"                 , createKernel},
    {"SetKernelArg"                 , setKernelArg},
    {"NDRangeKernel"                , enqueueNDRangeKernel},
    {"Finish"                       , finish},

    {"ReleaseKernel"                , releaseKernel},
    {"ReleaseProgram"               , releaseProgram},
    {"ReleaseMem"                   , releaseMemObject},
    {"ContextInfo"                  , getContextInfo},
    {"QueueInfo"                    , getCommandQueueInfo},

    {"CreateImage3D"                      , newImage3D},

    {"CreateImage2D"                      , newImage2D},
    {"ReadImage"                    , readImage},
    {"WriteImage"                   , writeImage},
    {"CopyImage"			        , copyImage},

    {"RetainMem"                    , retainMemObject},
    {"SupportedImageFormats"        , getSupportedImageFormats},
    {"MemInfo"                      , getMemObjectInfo},
    {"ImageInfo"                    , getImageInfo},
    {"CreateSampler"                , createSampler},
    {"RetainSampler"                , retainSampler},
    {"ReleaseSampler"               , releaseSampler},
    {"SamplerInfo"                  , getSamplerInfo},
    {"RetainProgram"                , retainProgram},
    {"UnloadCompiler"               , unloadCompiler},
    {"ProgramInfo"                  , getProgramInfo},
    {"CreateKernels"                , createKernelsInProgram},
    {"RetainKernel"                 , retainKernel},
    {"KernelInfo"                   , getKernelInfo},
    {"KernelWorkGroupInfo"          , getKernelWorkGroupInfo},
    {"EventInfo"                    , getEventInfo},
    {"RetainEvent"                  , retainEvent},
    {"ReleaseEvent"                 , releaseEvent},
    {"EventProfilingInfo"           , getEventProfilingInfo},
    {"Flush"                        , flush},

    {"CopyImageToBuffer"            , enqueueCopyImageToBuffer},
    {"CopyBufferToImage"            , enqueueCopyBufferToImage},

    {"Task"                         , enqueueTask},
    {"Marker"                       , enqueueMarker},
    {"Wait"                         , waitForEvents},
    {"WaitForEvents"                , enqueueWaitForEvents},
    {"Barrier"                      , enqueueBarrier},
#ifdef CL_VERSION_1_1
    {"SubBuffer"                    , createSubBuffer},
#endif
#ifndef CL_VERSION_1_1
    {"QueueProperty"                , setCommandQueueProperty},
#endif
    {NULL                           , NULL}
};
int get_enum_val(const char * str, int n, const cl_enum * enums)
{
    int i = 0;

    while(enums[i].cl != 0)
    {
        if(strncmp(str, enums[i].cl, n) == 0 && enums[i].cl[n] == 0)
            return enums[i].value;
        i++;
    }
  return -1;
}
int get_enum(lua_State * L, int position, const cl_enum * enums)
{
    const char * str;
    int retval = 0;
    int enum_value;
    int length = 0;
    int i;

    if(lua_isnumber(L, position))
        return luaL_checknumber(L, position);

    str = luaL_checkstring(L, position);

    length = strlen(str);

    for(i = 0; i <= length; i++)
    {
        if((str[i] == ' ') || (str[i] == ',') || (str[i] == '\0'))
        {
            if(i != 0)
            {
                if((enum_value = get_enum_val(str, i, enums)) == -1)
                   return -1;
                else
                    retval |= enum_value;

				str += i;
				length -= i;
				i = -1;
				if(i == length) break;
            }
            else
			{
				str++;
                i = -1;
				length--;
			}
        }
    }
    return retval;
}

static int luacl_register_enum(lua_State * L, const cl_enum * enums)
{
    int i;
    for (i = 0; ; i++)
    {
        if(enums[i].cl == NULL) return 1;
        lua_pushnumber(L, enums[i].value);
        lua_setfield(L, -2, enums[i].cl);
    }
}
/*
** Open library
*/
int luaopen_LuaCL (lua_State *L) {
  lua_newtable(L);
  luaL_register(L, NULL, LuaCL);
  luacl_register_enum(L, cl_const);
  return 1;
}
