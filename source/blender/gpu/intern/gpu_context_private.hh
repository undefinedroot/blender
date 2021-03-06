/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2016 by Mike Erwin.
 * All rights reserved.
 */

/** \file
 * \ingroup gpu
 *
 * This interface allow GPU to manage GL objects for multiple context and threads.
 */

#pragma once

#include "MEM_guardedalloc.h"

#include "GPU_context.h"

#include "gpu_state_private.hh"

#include <mutex>
#include <pthread.h>
#include <string.h>
#include <unordered_set>
#include <vector>

struct GPUFrameBuffer;
struct GPUMatrixState;

struct GPUContext {
 public:
  /** State managment */
  GPUShader *shader = NULL;
  GPUFrameBuffer *current_fbo = NULL;
  GPUMatrixState *matrix_state = NULL;
  blender::gpu::GPUStateManager *state_manager = NULL;

 protected:
  /** Thread on which this context is active. */
  pthread_t thread_;
  bool is_active_;

 public:
  GPUContext();
  virtual ~GPUContext();

  virtual void activate(void) = 0;
  virtual void deactivate(void) = 0;

  bool is_active_on_thread(void);

  MEM_CXX_CLASS_ALLOC_FUNCS("GPUContext")
};

GLuint GPU_vao_default(void);
GLuint GPU_framebuffer_default(void);

/* These require a gl ctx bound. */
GLuint GPU_buf_alloc(void);
GLuint GPU_tex_alloc(void);
GLuint GPU_vao_alloc(void);
GLuint GPU_fbo_alloc(void);

/* These can be called any threads even without gl ctx. */
void GPU_buf_free(GLuint buf_id);
void GPU_tex_free(GLuint tex_id);
/* These two need the ctx the id was created with. */
void GPU_vao_free(GLuint vao_id, GPUContext *ctx);
void GPU_fbo_free(GLuint fbo_id, GPUContext *ctx);

void gpu_context_add_framebuffer(GPUContext *ctx, struct GPUFrameBuffer *fb);
void gpu_context_remove_framebuffer(GPUContext *ctx, struct GPUFrameBuffer *fb);

void gpu_context_active_framebuffer_set(GPUContext *ctx, struct GPUFrameBuffer *fb);
struct GPUFrameBuffer *gpu_context_active_framebuffer_get(GPUContext *ctx);

struct GPUMatrixState *gpu_context_active_matrix_state_get(void);
