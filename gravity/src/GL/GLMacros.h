#pragma once

#include "GLTypes.h"
#include "GLHelper.h"

#define VARNAME_GL_FUNCTIONS glFunctions
#define VARNAME_GL_FUNCTION_PTRS_ARRAY funcPtrs

#define GETFUNC(T, i) ((T) VARNAME_GL_FUNCTIONS.VARNAME_GL_FUNCTION_PTRS_ARRAY[i])

#define INDEX_FUNC_GL_CLEAR mrko900::gravity::gl::GLHelper::IGL_CLEAR
#define INDEX_FUNC_GL_DRAW_ELEMENTS mrko900::gravity::gl::GLHelper::IGL_DRAW_ELEMENTS
#define INDEX_FUNC_GL_GET_STRING mrko900::gravity::gl::GLHelper::IGL_GET_STRING
#define INDEX_FUNC_GL_CLEAR_BUFFERFV mrko900::gravity::gl::GLHelper::IGL_CLEAR_BUFFERFV
#define INDEX_FUNC_GL_CREATE_SHADER mrko900::gravity::gl::GLHelper::IGL_CREATE_SHADER
#define INDEX_FUNC_GL_SHADER_SOURCE mrko900::gravity::gl::GLHelper::IGL_SHADER_SOURCE
#define INDEX_FUNC_GL_COMPILE_SHADER mrko900::gravity::gl::GLHelper::IGL_COMPILE_SHADER
#define INDEX_FUNC_GL_ATTACH_SHADER mrko900::gravity::gl::GLHelper::IGL_ATTACH_SHADER
#define INDEX_FUNC_GL_LINK_PROGRAM mrko900::gravity::gl::GLHelper::IGL_LINK_PROGRAM
#define INDEX_FUNC_GL_DELETE_SHADER mrko900::gravity::gl::GLHelper::IGL_DELETE_SHADER
#define INDEX_FUNC_GL_CREATE_PROGRAM mrko900::gravity::gl::GLHelper::IGL_CREATE_PROGRAM
#define INDEX_FUNC_GL_CREATE_VERTEX_ARRAYS mrko900::gravity::gl::GLHelper::IGL_CREATE_VERTEX_ARRAYS
#define INDEX_FUNC_GL_BIND_VERTEX_ARRAY mrko900::gravity::gl::GLHelper::IGL_BIND_VERTEX_ARRAY
#define INDEX_FUNC_GL_USE_PROGRAM mrko900::gravity::gl::GLHelper::IGL_USE_PROGRAM
#define INDEX_FUNC_GL_DRAW_ARRAYS mrko900::gravity::gl::GLHelper::IGL_DRAW_ARRAYS
#define INDEX_FUNC_WGL_SWAP_INTERVAL_EXT mrko900::gravity::gl::GLHelper::IWGL_SWAP_INTERVAL_EXT
#define INDEX_FUNC_WGL_GET_SWAP_INTERVAL_EXT mrko900::gravity::gl::GLHelper::IWGL_GET_SWAP_INTERVAL_EXT
#define INDEX_FUNC_GL_POINT_SIZE mrko900::gravity::gl::GLHelper::IGL_POINT_SIZE
#define INDEX_FUNC_GL_GET_ERROR mrko900::gravity::gl::GLHelper::IGL_GET_ERROR
#define INDEX_FUNC_GL_GET_SHADER_INFO_LOG mrko900::gravity::gl::GLHelper::IGL_GET_SHADER_INFO_LOG
#define INDEX_FUNC_GL_VERTEX_ATTRIB4FV mrko900::gravity::gl::GLHelper::IGL_VERTEX_ATTRIB4FV
#define INDEX_FUNC_GL_POLYGON_MODE mrko900::gravity::gl::GLHelper::IGL_POLYGON_MODE
#define INDEX_FUNC_GL_PATCH_PARAMETERI mrko900::gravity::gl::GLHelper::IGL_PATCH_PARAMETERI
#define INDEX_FUNC_GL_GET_SHADERIV mrko900::gravity::gl::GLHelper::IGL_GET_SHADERIV
#define INDEX_FUNC_GL_GET_PROGRAMIV mrko900::gravity::gl::GLHelper::IGL_GET_PROGRAMIV
#define INDEX_FUNC_GL_GET_PROGRAM_INFO_LOG mrko900::gravity::gl::GLHelper::IGL_GET_PROGRAM_INFO_LOG
#define INDEX_FUNC_GL_VIEWPORT mrko900::gravity::gl::GLHelper::IGL_VIEWPORT
#define INDEX_FUNC_GL_DEPTH_RANGE mrko900::gravity::gl::GLHelper::IGL_DEPTH_RANGE
#define INDEX_FUNC_GL_CREATE_BUFFERS mrko900::gravity::gl::GLHelper::IGL_CREATE_BUFFERS
#define INDEX_FUNC_GL_BIND_BUFFER mrko900::gravity::gl::GLHelper::IGL_BIND_BUFFER
#define INDEX_FUNC_GL_BUFFER_STORAGE mrko900::gravity::gl::GLHelper::IGL_BUFFER_STORAGE
#define INDEX_FUNC_GL_BUFFER_SUB_DATA mrko900::gravity::gl::GLHelper::IGL_BUFFER_SUB_DATA
#define INDEX_FUNC_GL_MAP_BUFFER mrko900::gravity::gl::GLHelper::IGL_MAP_BUFFER
#define INDEX_FUNC_GL_UNMAP_BUFFER mrko900::gravity::gl::GLHelper::IGL_UNMAP_BUFFER
#define INDEX_FUNC_GL_MAP_BUFFER_RANGE mrko900::gravity::gl::GLHelper::IGL_MAP_BUFFER_RANGE
#define INDEX_FUNC_GL_CLEAR_BUFFER_SUB_DATA mrko900::gravity::gl::GLHelper::IGL_CLEAR_BUFFER_SUB_DATA
#define INDEX_FUNC_GL_VERTEX_ARRAY_VERTEX_BUFFER mrko900::gravity::gl::GLHelper::IGL_VERTEX_ARRAY_VERTEX_BUFFER
#define INDEX_FUNC_GL_VERTEX_ARRAY_ATTRIB_FORMAT mrko900::gravity::gl::GLHelper::IGL_VERTEX_ARRAY_ATTRIB_FORMAT
#define INDEX_FUNC_GL_ENABLE_VERTEX_ARRAY_ATTRIB mrko900::gravity::gl::GLHelper::IGL_ENABLE_VERTEX_ARRAY_ATTRIB
#define INDEX_FUNC_GL_VERTEX_ARRAY_ATTRIB_BINDING mrko900::gravity::gl::GLHelper::IGL_VERTEX_ARRAY_ATTRIB_BINDING
#define INDEX_FUNC_GL_VERTEX_ATTRIB_FORMAT mrko900::gravity::gl::GLHelper::IGL_VERTEX_ATTRIB_FORMAT
#define INDEX_FUNC_GL_ENABLE_VERTEX_ATTRIB_ARRAY mrko900::gravity::gl::GLHelper::IGL_ENABLE_VERTEX_ATTRIB_ARRAY
#define INDEX_FUNC_GL_BIND_VERTEX_BUFFER mrko900::gravity::gl::GLHelper::IGL_BIND_VERTEX_BUFFER
#define INDEX_FUNC_GL_VERTEX_ATTRIB_POINTER mrko900::gravity::gl::GLHelper::IGL_VERTEX_ATTRIB_POINTER
#define INDEX_FUNC_GL_UNIFORM4F mrko900::gravity::gl::GLHelper::IGL_UNIFORM4F
#define INDEX_FUNC_GL_GET_INTEGERV mrko900::gravity::gl::GLHelper::IGL_GET_INTEGERV
#define INDEX_FUNC_GL_VERTEX_ATTRIB_BINDING mrko900::gravity::gl::GLHelper::IGL_VERTEX_ATTRIB_BINDING
#define INDEX_FUNC_GL_BIND_BUFFER_BASE mrko900::gravity::gl::GLHelper::IGL_BIND_BUFFER_BASE
#define INDEX_FUNC_GL_GET_ACTIVE_UNIFORMSIV mrko900::gravity::gl::GLHelper::IGL_GET_ACTIVE_UNIFORMSIV
#define INDEX_FUNC_GL_GET_UNIFORM_INDICES mrko900::gravity::gl::GLHelper::IGL_GET_UNIFORM_INDICES
#define INDEX_FUNC_GL_UNIFORM_MATRIX4FV mrko900::gravity::gl::GLHelper::IGL_UNIFORM_MATRIX4FV
#define INDEX_FUNC_GL_BUFFER_DATA mrko900::gravity::gl::GLHelper::IGL_BUFFER_DATA
#define INDEX_FUNC_GL_CREATE_TEXTURES mrko900::gravity::gl::GLHelper::IGL_CREATE_TEXTURES
#define INDEX_FUNC_GL_BIND_TEXTURE mrko900::gravity::gl::GLHelper::IGL_BIND_TEXTURE
#define INDEX_FUNC_GL_TEX_STORAGE2D mrko900::gravity::gl::GLHelper::IGL_TEX_STORAGE2D
#define INDEX_FUNC_GL_TEX_SUB_IMAGE2D mrko900::gravity::gl::GLHelper::IGL_TEX_SUB_IMAGE2D
#define INDEX_FUNC_GL_SAMPLER_PARAMETERI mrko900::gravity::gl::GLHelper::IGL_SAMPLER_PARAMETERI
#define INDEX_FUNC_GL_CREATE_SAMPLERS mrko900::gravity::gl::GLHelper::IGL_CREATE_SAMPLERS
#define INDEX_FUNC_GL_BIND_SAMPLER mrko900::gravity::gl::GLHelper::IGL_BIND_SAMPLER
#define INDEX_FUNC_GL_GENERATE_MIPMAP mrko900::gravity::gl::GLHelper::IGL_GENERATE_MIPMAP
#define INDEX_FUNC_GL_TEX_IMAGE2D mrko900::gravity::gl::GLHelper::IGL_TEX_IMAGE2D
#define INDEX_FUNC_GL_TEX_STORAGE3D mrko900::gravity::gl::GLHelper::IGL_TEX_STORAGE3D
#define INDEX_FUNC_GL_TEX_SUB_IMAGE3D mrko900::gravity::gl::GLHelper::IGL_TEX_SUB_IMAGE3D
#define INDEX_FUNC_GL_BIND_TEXTURE_UNIT mrko900::gravity::gl::GLHelper::IGL_BIND_TEXTURE_UNIT
#define INDEX_FUNC_GL_PROGRAM_PARAMETERI mrko900::gravity::gl::GLHelper::IGL_PROGRAM_PARAMETERI
#define INDEX_FUNC_GL_CREATE_PROGRAM_PIPELINES mrko900::gravity::gl::GLHelper::IGL_CREATE_PROGRAM_PIPELINES
#define INDEX_FUNC_GL_USE_PROGRAM_STAGES mrko900::gravity::gl::GLHelper::IGL_USE_PROGRAM_STAGES
#define INDEX_FUNC_GL_BIND_PROGRAM_PIPELINE mrko900::gravity::gl::GLHelper::IGL_BIND_PROGRAM_PIPELINE
#define INDEX_FUNC_GL_DRAW_ARRAYS_INSTANCED mrko900::gravity::gl::GLHelper::IGL_DRAW_ARRAYS_INSTANCED
#define INDEX_FUNC_GL_DRAW_ARRAYS_INDIRECT mrko900::gravity::gl::GLHelper::IGL_DRAW_ARRAYS_INDIRECT
#define INDEX_FUNC_GL_UNIFORM1F mrko900::gravity::gl::GLHelper::IGL_UNIFORM1F
#define INDEX_FUNC_GL_PROGRAM_UNIFORM1F mrko900::gravity::gl::GLHelper::IGL_PROGRAM_UNIFORM1F
#define INDEX_FUNC_GL_PROGRAM_UNIFORM2F mrko900::gravity::gl::GLHelper::IGL_PROGRAM_UNIFORM2F
#define INDEX_FUNC_GL_BLEND_FUNC mrko900::gravity::gl::GLHelper::IGL_BLEND_FUNC
#define INDEX_FUNC_GL_ENABLE mrko900::gravity::gl::GLHelper::IGL_ENABLE
#define INDEX_FUNC_GL_TRANSFORM_FEEDBACK_VARYINGS mrko900::gravity::gl::GLHelper::IGL_TRANSFORM_FEEDBACK_VARYINGS
#define INDEX_FUNC_GL_BEGIN_TRANSFORM_FEEDBACK mrko900::gravity::gl::GLHelper::IGL_BEGIN_TRANSFORM_FEEDBACK
#define INDEX_FUNC_GL_END_TRANSFORM_FEEDBACK mrko900::gravity::gl::GLHelper::IGL_END_TRANSFORM_FEEDBACK
#define INDEX_FUNC_GL_BIND_BUFFER_RANGE mrko900::gravity::gl::GLHelper::IGL_BIND_BUFFER_RANGE
#define INDEX_FUNC_GL_UNIFORM2F mrko900::gravity::gl::GLHelper::IGL_UNIFORM2F
#define INDEX_FUNC_GL_ORTHO mrko900::gravity::gl::GLHelper::IGL_ORTHO
#define INDEX_FUNC_GL_MATRIX_MODE mrko900::gravity::gl::GLHelper::IGL_MATRIX_MODE
#define INDEX_FUNC_GL_LOAD_IDENTITY mrko900::gravity::gl::GLHelper::IGL_LOAD_IDENTITY
#define INDEX_FUNC_GL_DISABLE mrko900::gravity::gl::GLHelper::IGL_DISABLE
#define INDEX_FUNC_GL_DELETE_BUFFERS mrko900::gravity::gl::GLHelper::IGL_DELETE_BUFFERS
#define INDEX_FUNC_GL_UNIFORM1UI mrko900::gravity::gl::GLHelper::IGL_UNIFORM1UI
#define INDEX_FUNC_GL_PROGRAM_UNIFORM1UI mrko900::gravity::gl::GLHelper::IGL_PROGRAM_UNIFORM1UI
#define INDEX_FUNC_GL_PROGRAM_UNIFORM4F mrko900::gravity::gl::GLHelper::IGL_PROGRAM_UNIFORM4F

// function macros
#define glClear GETFUNC(mrko900::gravity::gl::types::FUNC_GL_CLEAR, INDEX_FUNC_GL_CLEAR)
#define glDrawElements GETFUNC(mrko900::gravity::gl::types::FUNC_GL_DRAW_ELEMENTS, INDEX_FUNC_GL_DRAW_ELEMENTS)
#define glGetString GETFUNC(mrko900::gravity::gl::types::FUNC_GL_GET_STRING, INDEX_FUNC_GL_GET_STRING)
#define glClearBufferfv GETFUNC(mrko900::gravity::gl::types::FUNC_GL_CLEAR_BUFFERFV, INDEX_FUNC_GL_CLEAR_BUFFERFV)
#define glCreateShader GETFUNC(mrko900::gravity::gl::types::FUNC_GL_CREATE_SHADER, INDEX_FUNC_GL_CREATE_SHADER)
#define glShaderSource GETFUNC(mrko900::gravity::gl::types::FUNC_GL_SHADER_SOURCE, INDEX_FUNC_GL_SHADER_SOURCE)
#define glCompileShader GETFUNC(mrko900::gravity::gl::types::FUNC_GL_COMPILE_SHADER, INDEX_FUNC_GL_COMPILE_SHADER)
#define glAttachShader GETFUNC(mrko900::gravity::gl::types::FUNC_GL_ATTACH_SHADER, INDEX_FUNC_GL_ATTACH_SHADER)
#define glLinkProgram GETFUNC(mrko900::gravity::gl::types::FUNC_GL_LINK_PROGRAM, INDEX_FUNC_GL_LINK_PROGRAM)
#define glDeleteShader GETFUNC(mrko900::gravity::gl::types::FUNC_GL_DELETE_SHADER, INDEX_FUNC_GL_DELETE_SHADER)
#define glCreateProgram GETFUNC(mrko900::gravity::gl::types::FUNC_GL_CREATE_PROGRAM, INDEX_FUNC_GL_CREATE_PROGRAM)
#define glCreateVertexArrays GETFUNC(mrko900::gravity::gl::types::FUNC_GL_CREATE_VERTEX_ARRAYS,\
                                     INDEX_FUNC_GL_CREATE_VERTEX_ARRAYS)
#define glBindVertexArray GETFUNC(mrko900::gravity::gl::types::FUNC_GL_BIND_VERTEX_ARRAY,\
                                  INDEX_FUNC_GL_BIND_VERTEX_ARRAY)
#define glUseProgram GETFUNC(mrko900::gravity::gl::types::FUNC_GL_USE_PROGRAM, INDEX_FUNC_GL_USE_PROGRAM)
#define glDrawArrays GETFUNC(mrko900::gravity::gl::types::FUNC_GL_DRAW_ARRAYS, INDEX_FUNC_GL_DRAW_ARRAYS)
#define wglSwapIntervalEXT GETFUNC(mrko900::gravity::gl::types::FUNC_WGL_SWAP_INTERVAL_EXT,\
                                   INDEX_FUNC_WGL_SWAP_INTERVAL_EXT)
#define wglGetSwapIntervalEXT GETFUNC(mrko900::gravity::gl::types::FUNC_WGL_GET_SWAP_INTERVAL_EXT,\
                                      INDEX_FUNC_WGL_GET_SWAP_INTERVAL_EXT)
#define glPointSize GETFUNC(mrko900::gravity::gl::types::FUNC_GL_POINT_SIZE,\
                            INDEX_FUNC_GL_POINT_SIZE)
#define glGetError GETFUNC(mrko900::gravity::gl::types::FUNC_GL_GET_ERROR, INDEX_FUNC_GL_GET_ERROR)
#define glGetShaderInfoLog GETFUNC(mrko900::gravity::gl::types::FUNC_GL_GET_SHADER_INFO_LOG,\
                                   INDEX_FUNC_GL_GET_SHADER_INFO_LOG)
#define glVertexAttrib4fv GETFUNC(mrko900::gravity::gl::types::FUNC_GL_VERTEX_ATTRIB4FV,\
                                  INDEX_FUNC_GL_VERTEX_ATTRIB4FV)
#define glPolygonMode GETFUNC(mrko900::gravity::gl::types::FUNC_GL_POLYGON_MODE,\
                              INDEX_FUNC_GL_POLYGON_MODE)
#define glPatchParameteri GETFUNC(mrko900::gravity::gl::types::FUNC_GL_PATCH_PARAMETERI,\
                                  INDEX_FUNC_GL_PATCH_PARAMETERI)
#define glGetShaderiv GETFUNC(mrko900::gravity::gl::types::FUNC_GL_GET_SHADERIV,\
                              INDEX_FUNC_GL_GET_SHADERIV)
#define glGetProgramiv GETFUNC(mrko900::gravity::gl::types::FUNC_GL_GET_PROGRAMIV,\
                              INDEX_FUNC_GL_GET_PROGRAMIV)
#define glGetProgramInfoLog GETFUNC(mrko900::gravity::gl::types::FUNC_GL_GET_PROGRAM_INFO_LOG,\
                                    INDEX_FUNC_GL_GET_PROGRAM_INFO_LOG)
#define glViewport GETFUNC(mrko900::gravity::gl::types::FUNC_GL_VIEWPORT, INDEX_FUNC_GL_VIEWPORT)
#define glDepthRange GETFUNC(mrko900::gravity::gl::types::FUNC_GL_DEPTH_RANGE, INDEX_FUNC_GL_DEPTH_RANGE)
#define glCreateBuffers GETFUNC(mrko900::gravity::gl::types::FUNC_GL_CREATE_BUFFERS, INDEX_FUNC_GL_CREATE_BUFFERS)
#define glBindBuffer GETFUNC(mrko900::gravity::gl::types::FUNC_GL_BIND_BUFFER, INDEX_FUNC_GL_BIND_BUFFER)
#define glBufferStorage GETFUNC(mrko900::gravity::gl::types::FUNC_GL_BUFFER_STORAGE, INDEX_FUNC_GL_BUFFER_STORAGE)
#define glBufferSubData GETFUNC(mrko900::gravity::gl::types::FUNC_GL_BUFFER_SUB_DATA, INDEX_FUNC_GL_BUFFER_SUB_DATA)
#define glMapBuffer GETFUNC(mrko900::gravity::gl::types::FUNC_GL_MAP_BUFFER, INDEX_FUNC_GL_MAP_BUFFER)
#define glUnmapBuffer GETFUNC(mrko900::gravity::gl::types::FUNC_GL_UNMAP_BUFFER, INDEX_FUNC_GL_UNMAP_BUFFER)
#define glMapBufferRange GETFUNC(mrko900::gravity::gl::types::FUNC_GL_MAP_BUFFER_RANGE, INDEX_FUNC_GL_MAP_BUFFER_RANGE)
#define glClearBufferSubData GETFUNC(mrko900::gravity::gl::types::FUNC_GL_CLEAR_BUFFER_SUB_DATA,\
                                     INDEX_FUNC_GL_CLEAR_BUFFER_SUB_DATA)
#define glVertexArrayVertexBuffer GETFUNC(mrko900::gravity::gl::types::FUNC_GL_VERTEX_ARRAY_VERTEX_BUFFER,\
                                          INDEX_FUNC_GL_VERTEX_ARRAY_VERTEX_BUFFER)
#define glVertexArrayAttribFormat GETFUNC(mrko900::gravity::gl::types::FUNC_GL_VERTEX_ARRAY_ATTRIB_FORMAT,\
                                          INDEX_FUNC_GL_VERTEX_ARRAY_ATTRIB_FORMAT)
#define glEnableVertexArrayAttrib GETFUNC(mrko900::gravity::gl::types::FUNC_GL_ENABLE_VERTEX_ARRAY_ATTRIB,\
                                          INDEX_FUNC_GL_ENABLE_VERTEX_ARRAY_ATTRIB)
#define glVertexArrayAttribBinding GETFUNC(mrko900::gravity::gl::types::FUNC_GL_VERTEX_ARRAY_ATTRIB_BINDING,\
                                           INDEX_FUNC_GL_VERTEX_ARRAY_ATTRIB_BINDING)
#define glVertexAttribFormat GETFUNC(mrko900::gravity::gl::types::FUNC_GL_VERTEX_ATTRIB_FORMAT,\
                                     INDEX_FUNC_GL_VERTEX_ATTRIB_FORMAT)
#define glEnableVertexAttribArray GETFUNC(mrko900::gravity::gl::types::FUNC_GL_ENABLE_VERTEX_ATTRIB_ARRAY,\
                                          INDEX_FUNC_GL_ENABLE_VERTEX_ATTRIB_ARRAY)
#define glBindVertexBuffer GETFUNC(mrko900::gravity::gl::types::FUNC_GL_BIND_VERTEX_BUFFER,\
                                   INDEX_FUNC_GL_BIND_VERTEX_BUFFER)
#define glVertexAttribPointer GETFUNC(mrko900::gravity::gl::types::FUNC_GL_VERTEX_ATTRIB_POINTER,\
                                      INDEX_FUNC_GL_VERTEX_ATTRIB_POINTER)
#define glUniform4f GETFUNC(mrko900::gravity::gl::types::FUNC_GL_UNIFORM4F, INDEX_FUNC_GL_UNIFORM4F)
#define glGetIntegerv GETFUNC(mrko900::gravity::gl::types::FUNC_GL_GET_INTEGERV, INDEX_FUNC_GL_GET_INTEGERV)
#define glVertexAttribBinding GETFUNC(mrko900::gravity::gl::types::FUNC_GL_VERTEX_ATTRIB_BINDING,\
                                      INDEX_FUNC_GL_VERTEX_ATTRIB_BINDING)
#define glBindBufferBase GETFUNC(mrko900::gravity::gl::types::FUNC_GL_BIND_BUFFER_BASE, INDEX_FUNC_GL_BIND_BUFFER_BASE)
#define glGetActiveUniformsiv GETFUNC(mrko900::gravity::gl::types::FUNC_GL_GET_ACTIVE_UNIFORMSIV,\
                                      INDEX_FUNC_GL_GET_ACTIVE_UNIFORMSIV)
#define glGetUniformIndices GETFUNC(mrko900::gravity::gl::types::FUNC_GL_GET_UNIFORM_INDICES,\
                                    INDEX_FUNC_GL_GET_UNIFORM_INDICES)
#define glUniformMatrix4fv GETFUNC(mrko900::gravity::gl::types::FUNC_GL_UNIFORM_MATRIX4FV,\
                                   INDEX_FUNC_GL_UNIFORM_MATRIX4FV)
#define glBufferData GETFUNC(mrko900::gravity::gl::types::FUNC_GL_BUFFER_DATA, INDEX_FUNC_GL_BUFFER_DATA)
#define glCreateTextures GETFUNC(mrko900::gravity::gl::types::FUNC_GL_CREATE_TEXTURES, INDEX_FUNC_GL_CREATE_TEXTURES)
#define glBindTexture GETFUNC(mrko900::gravity::gl::types::FUNC_GL_BIND_TEXTURE, INDEX_FUNC_GL_BIND_TEXTURE)
#define glTexStorage2D GETFUNC(mrko900::gravity::gl::types::FUNC_GL_TEX_STORAGE2D, INDEX_FUNC_GL_TEX_STORAGE2D)
#define glTexSubImage2D GETFUNC(mrko900::gravity::gl::types::FUNC_GL_TEX_SUB_IMAGE2D, INDEX_FUNC_GL_TEX_SUB_IMAGE2D)
#define glSamplerParameteri GETFUNC(mrko900::gravity::gl::types::FUNC_GL_SAMPLER_PARAMETERI,\
                                    INDEX_FUNC_GL_SAMPLER_PARAMETERI)
#define glCreateSamplers GETFUNC(mrko900::gravity::gl::types::FUNC_GL_CREATE_SAMPLERS,\
                                 INDEX_FUNC_GL_CREATE_SAMPLERS)
#define glBindSampler GETFUNC(mrko900::gravity::gl::types::FUNC_GL_BIND_SAMPLER,\
                              INDEX_FUNC_GL_BIND_SAMPLER)
#define glGenerateMipmap GETFUNC(mrko900::gravity::gl::types::FUNC_GL_GENERATE_MIPMAP, INDEX_FUNC_GL_GENERATE_MIPMAP)
#define glTexImage2D GETFUNC(mrko900::gravity::gl::types::FUNC_GL_TEX_IMAGE2D, INDEX_FUNC_GL_TEX_IMAGE2D)
#define glTexStorage3D GETFUNC(mrko900::gravity::gl::types::FUNC_GL_TEX_STORAGE3D, INDEX_FUNC_GL_TEX_STORAGE3D)
#define glTexSubImage3D GETFUNC(mrko900::gravity::gl::types::FUNC_GL_TEX_SUB_IMAGE3D, INDEX_FUNC_GL_TEX_SUB_IMAGE3D)
#define glBindTextureUnit GETFUNC(mrko900::gravity::gl::types::FUNC_GL_BIND_TEXTURE_UNIT,\
                                  INDEX_FUNC_GL_BIND_TEXTURE_UNIT)
#define glProgramParameteri GETFUNC(mrko900::gravity::gl::types::FUNC_GL_PROGRAM_PARAMETERI,\
                                    INDEX_FUNC_GL_PROGRAM_PARAMETERI)
#define glCreateProgramPipelines GETFUNC(mrko900::gravity::gl::types::FUNC_GL_CREATE_PROGRAM_PIPELINES,\
                                         INDEX_FUNC_GL_CREATE_PROGRAM_PIPELINES)
#define glUseProgramStages GETFUNC(mrko900::gravity::gl::types::FUNC_GL_USE_PROGRAM_STAGES,\
                                   INDEX_FUNC_GL_USE_PROGRAM_STAGES)
#define glBindProgramPipeline GETFUNC(mrko900::gravity::gl::types::FUNC_GL_BIND_PROGRAM_PIPELINE,\
                                      INDEX_FUNC_GL_BIND_PROGRAM_PIPELINE)
#define glDrawArraysInstanced GETFUNC(mrko900::gravity::gl::types::FUNC_GL_DRAW_ARRAYS_INSTANCED,\
                                      INDEX_FUNC_GL_DRAW_ARRAYS_INSTANCED)
#define glDrawArraysIndirect GETFUNC(mrko900::gravity::gl::types::FUNC_GL_DRAW_ARRAYS_INDIRECT,\
                                     INDEX_FUNC_GL_DRAW_ARRAYS_INDIRECT)
#define glUniform1f GETFUNC(mrko900::gravity::gl::types::FUNC_GL_UNIFORM1F, INDEX_FUNC_GL_UNIFORM1F)
#define glProgramUniform1f GETFUNC(mrko900::gravity::gl::types::FUNC_GL_PROGRAM_UNIFORM1F,\
                                   INDEX_FUNC_GL_PROGRAM_UNIFORM1F)
#define glProgramUniform2f GETFUNC(mrko900::gravity::gl::types::FUNC_GL_PROGRAM_UNIFORM2F,\
                                   INDEX_FUNC_GL_PROGRAM_UNIFORM2F)
#define glBlendFunc GETFUNC(mrko900::gravity::gl::types::FUNC_GL_BLEND_FUNC, INDEX_FUNC_GL_BLEND_FUNC)
#define glEnable GETFUNC(mrko900::gravity::gl::types::FUNC_GL_ENABLE, INDEX_FUNC_GL_ENABLE)
#define glTransformFeedbackVaryings GETFUNC(mrko900::gravity::gl::types::FUNC_GL_TRANSFORM_FEEDBACK_VARYINGS,\
                                            INDEX_FUNC_GL_TRANSFORM_FEEDBACK_VARYINGS)
#define glBeginTransformFeedback GETFUNC(mrko900::gravity::gl::types::FUNC_GL_BEGIN_TRANSFORM_FEEDBACK,\
                                         INDEX_FUNC_GL_BEGIN_TRANSFORM_FEEDBACK)
#define glEndTransformFeedback GETFUNC(mrko900::gravity::gl::types::FUNC_GL_END_TRANSFORM_FEEDBACK,\
                                       INDEX_FUNC_GL_END_TRANSFORM_FEEDBACK)
#define glBindBufferRange GETFUNC(mrko900::gravity::gl::types::FUNC_GL_BIND_BUFFER_RANGE,\
                                  INDEX_FUNC_GL_BIND_BUFFER_RANGE)
#define glUniform2f GETFUNC(mrko900::gravity::gl::types::FUNC_GL_UNIFORM2F, INDEX_FUNC_GL_UNIFORM2F)
#define glOrtho GETFUNC(mrko900::gravity::gl::types::FUNC_GL_ORTHO, INDEX_FUNC_GL_ORTHO)
#define glMatrixMode GETFUNC(mrko900::gravity::gl::types::FUNC_GL_MATRIX_MODE, INDEX_FUNC_GL_MATRIX_MODE)
#define glLoadIdentity GETFUNC(mrko900::gravity::gl::types::FUNC_GL_LOAD_IDENTITY, INDEX_FUNC_GL_LOAD_IDENTITY)
#define glDisable GETFUNC(mrko900::gravity::gl::types::FUNC_GL_DISABLE, INDEX_FUNC_GL_DISABLE)
#define glDeleteBuffers GETFUNC(mrko900::gravity::gl::types::FUNC_GL_DELETE_BUFFERS, INDEX_FUNC_GL_DELETE_BUFFERS)
#define glUniform1ui GETFUNC(mrko900::gravity::gl::types::FUNC_GL_UNIFORM1UI, INDEX_FUNC_GL_UNIFORM1UI)
#define glProgramUniform1ui GETFUNC(mrko900::gravity::gl::types::FUNC_GL_PROGRAM_UNIFORM1UI,\
                                    INDEX_FUNC_GL_PROGRAM_UNIFORM1UI)
#define glProgramUniform4f GETFUNC(mrko900::gravity::gl::types::FUNC_GL_PROGRAM_UNIFORM4F,\
                                   INDEX_FUNC_GL_PROGRAM_UNIFORM4F)

// gl constants
#define GL_TRIANGLES 0x4
#define GL_VERSION 0x1F02
#define GL_ARRAY_BUFFER 0x8892
#define GL_STATIC_DRAW 0x88E4
#define GL_FLOAT 0x1406
#define GL_FALSE 0
#define GL_COMPILE_STATUS 0x8B81
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_BLEND 0x0BE2
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_COLOR 0x1800
#define GL_POINTS 0x0000
#define GL_TESS_EVALUATION_SHADER 0x8E87
#define GL_TESS_CONTROL_SHADER 0x8E88
#define GL_FRONT_AND_BACK 0x0408
#define GL_LINE 0x1B01
#define GL_PATCH_VERTICES 0x8E72
#define GL_SHADER_TYPE 0x8B4F
#define GL_DELETE_STATUS 0x8B80
#define GL_COMPILE_STATUS 0x8B81
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_SHADER_SOURCE_LENGTH 0x8B88
#define GL_LINK_STATUS 0x8B82
#define GL_PATCHES 0xE
#define GL_GEOMETRY_SHADER 0x8DD9
#define GL_MAP_READ_BIT 0x0001
#define GL_MAP_WRITE_BIT 0x0002
#define GL_MAP_PERSISTENT_BIT 0x00000040
#define GL_MAP_COHERENT_BIT 0x00000080
#define GL_DYNAMIC_STORAGE_BIT 0x0100
#define GL_CLIENT_STORAGE_BIT 0x0200
#define GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT 0x00004000
#define GL_BUFFER_IMMUTABLE_STORAGE 0x821F
#define GL_BUFFER_STORAGE_FLAGS 0x8220
#define GL_READ_ONLY 0x88B8
#define GL_WRITE_ONLY 0x88B9
#define GL_READ_WRITE 0x88BA
#define GL_BYTE 0x1400
#define GL_UNSIGNED_BYTE 0x1401
#define GL_SHORT 0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT 0x1404
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406
#define GL_2_BYTES 0x1407
#define GL_3_BYTES 0x1408
#define GL_4_BYTES 0x1409
#define GL_DOUBLE 0x140A
#define GL_MAX_VERTEX_ATTRIB_BINDINGS 0x82DA
#define GL_UNIFORM_BUFFER 0x8A11
#define GL_UNIFORM_OFFSET 0x8A3B
#define GL_DEPTH_STENCIL 0x84F9
#define GL_TEXTURE_2D 0x0DE1
#define GL_RED 0x1903
#define GL_GREEN 0x1904
#define GL_BLUE 0x1905
#define GL_ALPHA 0x1906
#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_RGBA32F 0x8814
#define GL_RGB32F 0x8815
#define GL_RGBA16F 0x881A
#define GL_RGB16F 0x881B
#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_LINEAR_MIPMAP_NEAREST 0x2701
#define GL_NEAREST_MIPMAP_LINEAR 0x2702
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_CLAMP 0x2900
#define GL_REPEAT 0x2901
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_MIRRORED_REPEAT 0x8370
#define GL_MIRROR_CLAMP_TO_EDGE 0x8743
#define GL_CLAMP_TO_BORDER 0x812D
#define GL_TEXTURE_2D_ARRAY 0x8C1A
#define GL_PROGRAM_SEPARABLE 0x8258
#define GL_TRUE 1
#define GL_VERTEX_SHADER_BIT 0x00000001
#define GL_FRAGMENT_SHADER_BIT 0x00000002
#define GL_GEOMETRY_SHADER_BIT 0x00000004
#define GL_TESS_CONTROL_SHADER_BIT 0x00000008
#define GL_TESS_EVALUATION_SHADER_BIT 0x00000010
#define GL_ACTIVE_PROGRAM 0x8259
#define GL_PROGRAM_PIPELINE_BINDING 0x825A
#define GL_ALL_SHADER_BITS 0xFFFFFFFF
#define GL_TRIANGLE_STRIP 0x0005
#define GL_DRAW_INDIRECT_BUFFER 0x8F3F
#define GL_TRANSFORM_FEEDBACK_BUFFER 0x8C8E
#define GL_INTERLEAVED_ATTRIBS 0x8C8C
#define GL_SEPARATE_ATTRIBS 0x8C8D
#define GL_CLIP_PLANE0 0x3000
#define GL_CLIP_DISTANCE0 GL_CLIP_PLANE0
#define GL_PROJECTION 0x1701
#define GL_VIEWPORT 0x0BA2
#define GL_CULL_FACE 0x0B44
