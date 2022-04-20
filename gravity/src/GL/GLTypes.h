#pragma once

#include <cstddef>
#include <cstdint>

namespace mrko900::gravity::gl::types {
    typedef uint32_t GLenum;
    typedef uint32_t GLbitfield;
    typedef uint32_t GLuint;
    typedef int32_t GLint;
    typedef uint32_t GLsizei;
    typedef bool GLboolean;
    typedef signed char GLbyte;
    typedef int16_t GLshort;
    typedef unsigned char GLubyte;
    typedef uint16_t GLushort;
    typedef float GLfloat;
    typedef float GLclampf;
    typedef double GLdouble;
    typedef double GLclampd;
    typedef void GLvoid;
    typedef char GLchar;
    typedef ptrdiff_t GLintptr;
    typedef ptrdiff_t GLsizeiptr;   

    // todo argument names

    typedef GLubyte* (*FUNC_GL_GET_STRING) (GLenum);
    typedef void (*FUNC_GL_CLEAR) (GLbitfield);
    typedef void (*FUNC_GL_VERTEX2F) (GLfloat, GLfloat);
    typedef void (*FUNC_GL_ATTACH_SHADER) (GLuint program, GLuint shader);
    typedef void (*FUNC_GL_LINK_PROGRAM) (GLuint program);
    typedef void (*FUNC_GL_VALIDATE_PROGRAM) (GLuint);
    typedef void (*FUNC_GL_DELETE_SHADER) (GLuint shader);
    typedef void (*FUNC_GL_GEN_BUFFERS) (GLsizei n, GLuint* buffers);
    typedef void (*FUNC_GL_BIND_BUFFER) (GLenum target, GLuint buffer);
    typedef void (*FUNC_GL_BUFFER_DATA) (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
    typedef void (*FUNC_GL_VERTEX_ATTRIB_POINTER) (GLuint index, GLint size, GLenum type, GLboolean normalized, 
                                                   GLsizei stride, const GLvoid* pointer);
    typedef void (*FUNC_GL_USE_PROGRAM) (GLuint program);
    typedef void (*FUNC_GL_DRAW_ARRAYS) (GLenum mode, GLint first, GLsizei count);
    typedef void (*FUNC_GL_ENABLE_VERTEX_ATTRIB_ARRAY) (GLuint index);
    typedef void (*FUNC_GL_SHADER_SOURCE) (GLuint shader, GLsizei count, const GLchar** string, const GLint* length);
    typedef void (*FUNC_GL_COMPILE_SHADER) (GLuint shader);
    typedef void (*FUNC_GL_GET_SHADERIV) (GLuint shader, GLenum pname, GLint* params);
    typedef void (*FUNC_GL_GET_SHADER_INFO_LOG) (GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
    typedef GLuint (*FUNC_GL_CREATE_PROGRAM) ();
    typedef GLuint (*FUNC_GL_CREATE_SHADER) (GLenum shaderType);
    typedef void (*FUNC_GL_DRAW_ELEMENTS) (GLenum mode, GLsizei count, GLenum type, const GLvoid* indices);
    typedef void (*FUNC_GL_UNIFORM4F) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
    typedef GLint (*FUNC_GL_GET_UNIFORM_LOCATION) (GLuint program, const GLchar* name);
    typedef GLenum (*FUNC_GL_GET_ERROR) ();
    typedef void (*FUNC_GL_BEGIN) (GLenum);
    typedef void (*FUNC_GL_END) ();
    typedef void (*FUNC_GL_VERTEX2F) (GLfloat x, GLfloat y);
    typedef void (*FUNC_GL_GEN_VERTEX_ARRAYS) (GLsizei n, GLuint* arrays);
    typedef void (*FUNC_GL_BIND_VERTEX_ARRAY) (GLuint array);
    typedef void (*FUNC_GL_BLEND_FUNC) (GLenum sfactor, GLenum dfactor);
    typedef void (*FUNC_GL_ENABLE) (GLenum cap);
    typedef void (*FUNC_GL_DETACH_SHADER) (GLuint program, GLuint shader);
    typedef void (*FUNC_GL_CLEAR_BUFFERFV) (GLenum buffer, GLint drawbuffer, const GLfloat* value);
    typedef void (*FUNC_GL_CREATE_VERTEX_ARRAYS) (GLsizei n, GLuint* arrays);
    typedef void (*FUNC_WGL_SWAP_INTERVAL_EXT) (int interval);
    typedef int (*FUNC_WGL_GET_SWAP_INTERVAL_EXT) ();
    typedef void (*FUNC_GL_POINT_SIZE) (GLfloat size);
    typedef void (*FUNC_GL_VERTEX_ATTRIB4FV) (GLuint index, const GLfloat* v);
    typedef void (*FUNC_GL_POLYGON_MODE) (GLenum face, GLenum mode);
    typedef void (*FUNC_GL_PATCH_PARAMETERI) (GLenum pname, GLint value);
    typedef void (*FUNC_GL_GET_PROGRAMIV) (GLuint program, GLenum pname, GLint* params);
    typedef void (*FUNC_GL_GET_PROGRAM_INFO_LOG) (GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
    typedef void (*FUNC_GL_VIEWPORT) (GLint x, GLint y, GLsizei width, GLsizei height);
    typedef void (*FUNC_GL_DEPTH_RANGE) (GLdouble nearVal, GLdouble farVal);
    typedef void (*FUNC_GL_CREATE_BUFFERS) (GLsizei n, GLuint* buffers);
    typedef void (*FUNC_GL_BUFFER_STORAGE) (GLenum target, GLsizeiptr size, const GLvoid* data, GLbitfield flags);
    typedef void (*FUNC_GL_BUFFER_SUB_DATA) (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data);
    typedef void* (*FUNC_GL_MAP_BUFFER) (GLenum target, GLenum access);
    typedef GLboolean (*FUNC_GL_UNMAP_BUFFER) (GLenum target);
    typedef void* (*FUNC_GL_MAP_BUFFER_RANGE) (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
    typedef void (*FUNC_GL_CLEAR_BUFFER_SUB_DATA) (GLenum target, GLenum internalformat, GLintptr offset,
                                                   GLsizeiptr size, GLenum format, GLenum type, const void* data);
    typedef void (*FUNC_GL_VERTEX_ARRAY_VERTEX_BUFFER) (GLuint vaobj, GLuint bindingindex, GLuint buffer,
                                                        GLintptr offset, GLsizei stride);
    typedef void (*FUNC_GL_VERTEX_ARRAY_ATTRIB_FORMAT) (GLuint vaobj, GLuint attribindex, GLint size,
                                                        GLenum type, GLboolean normalized, GLuint relativeoffset);
    typedef void (*FUNC_GL_ENABLE_VERTEX_ARRAY_ATTRIB) (GLuint vaobj, GLuint index);
    typedef void (*FUNC_GL_VERTEX_ARRAY_ATTRIB_BINDING) (GLuint vaobj, GLuint attribindex, GLuint bindingindex);
    typedef void (*FUNC_GL_VERTEX_ATTRIB_FORMAT) (GLuint attribindex, GLint size, GLenum type, 
                                                  GLboolean normalized, GLuint relativeoffset);
    typedef void (*FUNC_GL_BIND_VERTEX_BUFFER) (GLuint bindingindex, GLuint buffer, GLintptr offset, GLintptr stride);
    typedef void (*FUNC_GL_GET_INTEGERV) (GLenum pname, GLint* data);
    typedef void (*FUNC_GL_VERTEX_ATTRIB_BINDING) (GLuint attribindex, GLuint bindingindex);
    typedef void (*FUNC_GL_BIND_BUFFER_BASE) (GLenum target, GLuint index, GLuint buffer);
    typedef void (*FUNC_GL_GET_ACTIVE_UNIFORMSIV) (GLuint program, GLsizei uniformCount, const GLuint* uniformIndices,
                                                   GLenum pname, GLint* params);
    typedef void (*FUNC_GL_GET_UNIFORM_INDICES) (GLuint program, GLsizei uniformCount,
                                                 const GLchar** uniformNames, GLuint* uniformIndices);
    typedef void (*FUNC_GL_UNIFORM_MATRIX4FV) (GLint location, GLsizei count, 
                                               GLboolean transpose, const GLfloat* value);
    typedef void (*FUNC_GL_CREATE_TEXTURES) (GLenum target, GLsizei n, GLuint* textures);
    typedef void (*FUNC_GL_BIND_TEXTURE) (GLenum target, GLuint texture);
    typedef void (*FUNC_GL_TEX_STORAGE2D) (GLenum target, GLsizei levels, GLenum internalformat, 
                                           GLsizei width, GLsizei height);
    typedef void (*FUNC_GL_TEX_SUB_IMAGE2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width,
                                             GLsizei height, GLenum format, GLenum type, const GLvoid* pixels);
    typedef void (*FUNC_GL_SAMPLER_PARAMETERI) (GLuint sampler, GLenum pname, GLint param);
    typedef void (*FUNC_GL_CREATE_SAMPLERS) (GLsizei n, GLuint* samplers);
    typedef void (*FUNC_GL_BIND_SAMPLER) (GLuint unit, GLuint sampler);
    typedef void (*FUNC_GL_GENERATE_MIPMAP) (GLenum target);
    typedef void (*FUNC_GL_TEX_IMAGE2D) (GLenum target, GLint level, GLint internalformat, GLsizei width, 
                                         GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data);
    typedef void (*FUNC_GL_TEX_STORAGE3D) (GLenum target, GLsizei levels, GLenum internalformat,
                                           GLsizei width, GLsizei height, GLsizei depth);
    typedef void (*FUNC_GL_TEX_SUB_IMAGE3D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset,
                                             GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type,
                                             const GLvoid* pixels);
    typedef void (*FUNC_GL_BIND_TEXTURE_UNIT) (GLuint unit, GLuint texture);
    typedef void (*FUNC_GL_PROGRAM_PARAMETERI) (GLuint program, GLenum pname, GLint value);
    typedef void (*FUNC_GL_CREATE_PROGRAM_PIPELINES) (GLsizei n, GLuint* pipelines);
    typedef void (*FUNC_GL_USE_PROGRAM_STAGES) (GLuint pipeline, GLbitfield stages, GLuint program);
    typedef void (*FUNC_GL_BIND_PROGRAM_PIPELINE) (GLuint pipeline);
    typedef void (*FUNC_GL_DRAW_ARRAYS_INSTANCED) (GLenum mode, GLint first, GLsizei count, GLsizei primcount);
    typedef void (*FUNC_GL_DRAW_ARRAYS_INDIRECT) (GLenum mode, const void* indirect);
    typedef void (*FUNC_GL_UNIFORM1F) (GLint location, GLfloat v0);
    typedef void (*FUNC_GL_PROGRAM_UNIFORM1F) (GLuint program, GLint location, GLfloat v0);
    typedef void (*FUNC_GL_PROGRAM_UNIFORM2F) (GLuint program, GLint location, GLfloat v0, GLfloat v1);
    typedef void (*FUNC_GL_TRANSFORM_FEEDBACK_VARYINGS) (GLuint program, GLsizei count, const char** varyings, 
                                                         GLenum bufferMode);
    typedef void (*FUNC_GL_BEGIN_TRANSFORM_FEEDBACK) (GLenum primitiveMode);
    typedef void (*FUNC_GL_END_TRANSFORM_FEEDBACK) ();
    typedef void (*FUNC_GL_BIND_BUFFER_RANGE) (GLenum target, GLuint index, GLuint buffer,
                                               GLintptr offset, GLsizeiptr size);
    typedef void (*FUNC_GL_UNIFORM2F) (GLint location, GLfloat v0, GLfloat v1);
}
