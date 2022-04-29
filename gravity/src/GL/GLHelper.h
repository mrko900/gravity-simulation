#pragma once

#include <functional>

namespace mrko900::gravity::gl {
    class GLHelper {
    private:
        static constexpr unsigned int LEN = 88;

    public:
        enum Function {
            IGL_CLEAR = 0,
            IGL_DRAW_ELEMENTS = 1,
            IGL_CLEAR_BUFFERFV = 2,
            IGL_GET_STRING = 3,
            IGL_CREATE_SHADER = 4,
            IGL_SHADER_SOURCE = 5,
            IGL_COMPILE_SHADER = 6,
            IGL_ATTACH_SHADER = 7,
            IGL_LINK_PROGRAM = 8,
            IGL_DELETE_SHADER = 9,
            IGL_CREATE_PROGRAM = 10,
            IGL_CREATE_VERTEX_ARRAYS = 11,
            IGL_BIND_VERTEX_ARRAY = 12,
            IGL_USE_PROGRAM = 13,
            IGL_DRAW_ARRAYS = 14,
            IWGL_SWAP_INTERVAL_EXT = 15,
            IWGL_GET_SWAP_INTERVAL_EXT = 16,
            IGL_POINT_SIZE = 17,
            IGL_GET_ERROR = 18,
            IGL_GET_SHADER_INFO_LOG = 19,
            IGL_VERTEX_ATTRIB4FV = 20,
            IGL_POLYGON_MODE = 21,
            IGL_PATCH_PARAMETERI = 22,
            IGL_GET_SHADERIV = 23,
            IGL_GET_PROGRAMIV = 24,
            IGL_GET_PROGRAM_INFO_LOG = 25,
            IGL_VIEWPORT = 26,
            IGL_DEPTH_RANGE = 27,
            IGL_CREATE_BUFFERS = 28,
            IGL_BIND_BUFFER = 29,
            IGL_BUFFER_STORAGE = 30,
            IGL_BUFFER_SUB_DATA = 31,
            IGL_MAP_BUFFER = 32,
            IGL_UNMAP_BUFFER = 33,
            IGL_MAP_BUFFER_RANGE = 34,
            IGL_CLEAR_BUFFER_SUB_DATA = 35,
            IGL_VERTEX_ARRAY_VERTEX_BUFFER = 36,
            IGL_VERTEX_ARRAY_ATTRIB_FORMAT = 37,
            IGL_ENABLE_VERTEX_ARRAY_ATTRIB = 38,
            IGL_VERTEX_ARRAY_ATTRIB_BINDING = 39,
            IGL_VERTEX_ATTRIB_FORMAT = 40,
            IGL_ENABLE_VERTEX_ATTRIB_ARRAY = 41,
            IGL_BIND_VERTEX_BUFFER = 42,
            IGL_VERTEX_ATTRIB_POINTER = 43,
            IGL_UNIFORM4F = 44,
            IGL_GET_INTEGERV = 45,
            IGL_VERTEX_ATTRIB_BINDING = 46,
            IGL_BIND_BUFFER_BASE = 47,
            IGL_GET_ACTIVE_UNIFORMSIV = 48,
            IGL_GET_UNIFORM_INDICES = 49,
            IGL_UNIFORM_MATRIX4FV = 50,
            IGL_BUFFER_DATA = 51,
            IGL_CREATE_TEXTURES = 52,
            IGL_BIND_TEXTURE = 53,
            IGL_TEX_STORAGE2D = 54,
            IGL_TEX_SUB_IMAGE2D = 55,
            IGL_SAMPLER_PARAMETERI = 56,
            IGL_CREATE_SAMPLERS = 57,
            IGL_BIND_SAMPLER = 58,
            IGL_GENERATE_MIPMAP = 59,
            IGL_TEX_IMAGE2D = 60,
            IGL_TEX_STORAGE3D = 61,
            IGL_TEX_SUB_IMAGE3D = 62,
            IGL_BIND_TEXTURE_UNIT = 63,
            IGL_PROGRAM_PARAMETERI = 64,
            IGL_CREATE_PROGRAM_PIPELINES = 65,
            IGL_USE_PROGRAM_STAGES = 66,
            IGL_BIND_PROGRAM_PIPELINE = 67,
            IGL_DRAW_ARRAYS_INSTANCED = 68,
            IGL_DRAW_ARRAYS_INDIRECT = 69,
            IGL_UNIFORM1F = 70,
            IGL_PROGRAM_UNIFORM1F = 71,
            IGL_PROGRAM_UNIFORM2F = 72,
            IGL_BLEND_FUNC = 73,
            IGL_ENABLE = 74,
            IGL_TRANSFORM_FEEDBACK_VARYINGS = 75,
            IGL_BEGIN_TRANSFORM_FEEDBACK = 76,
            IGL_END_TRANSFORM_FEEDBACK = 77,
            IGL_BIND_BUFFER_RANGE = 78,
            IGL_UNIFORM2F = 79,
            IGL_ORTHO = 80,
            IGL_MATRIX_MODE = 81,
            IGL_LOAD_IDENTITY = 82,
            IGL_DISABLE = 83,
            IGL_DELETE_BUFFERS = 84,
            IGL_UNIFORM1UI = 85,
            IGL_PROGRAM_UNIFORM1UI = 86,
            IGL_PROGRAM_UNIFORM4F = 87,
        };

        typedef void (*FuncPtr) ();

        FuncPtr funcPtrs[LEN];

        GLHelper(std::function<FuncPtr(Function)> funcSrc);
        void initFunction(Function function);

    private:
        std::function<FuncPtr(Function)> m_FuncLoader;
    };
}
