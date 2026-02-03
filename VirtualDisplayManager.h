#ifndef VIRTUAL_DISPLAY_MANAGER_H
#define VIRTUAL_DISPLAY_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Opaque handle to hide platform-specific structs
typedef struct VirtualDisplayManager VirtualDisplayManager;

typedef struct
{
  const uint8_t *data;
  int width;
  int height;
  size_t size;
} ScreenFrame;

// Callback type for frame updates
typedef void (*FrameCallback)(const ScreenFrame *Frame, void *user_data);

// --- Interface Functions ---

VirtualDisplayManager *vdm_create(void);

bool vdm_initialize(VirtualDisplayManager *vdm);

bool vdm_create_display(VirtualDisplayManager *vdm, const char *name, int w, int h);

bool vdm_set_frame_callback(VirtualDisplayManager *vdm, FrameCallback callback, void *user_data);

bool vdm_destroy(VirtualDisplayManager *vdm);

#endif
