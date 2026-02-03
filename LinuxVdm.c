#include "VirtualDisplayManager.h"
#include "edid_data.h"
#include "evdi_lib.h"
#include <stdlib.h>
#include <stdio.h>

struct VirtualDisplayManager
{
    evdi_handle handle;
    FrameCallback callback;
    void *user_data;
    uint8_t width;
    uint8_t height;
    uint8_t device;

    struct evdi_buffer *buffer;
};

int find_evdi_device()
{
    for (int i = 0; i < 64; i++)
    { // Check cards 0 to 63
        enum evdi_device_status status = evdi_check_device(i);

        if (status == AVAILABLE)
        {
            printf("Found available EVDI device at /dev/dri/card%d\n", i);
            return i;
        }
        else if (status == UNRECOGNIZED)
        {
            // This is a normal GPU (like Intel/Nvidia), skip it
            continue;
        }
    }
    return -1; // No EVDI device found
};

bool vdm_initialize(VirtualDisplayManager *vdm)
{
    vdm->device = find_evdi_device();
    if (vdm->device < 0)
        return false;

    // Check if the kernel module is loaded
    if (evdi_check_device(vdm->device) < 0)
    {
        fprintf(stderr, "EVDI kernel module not found. Run: sudo modprobe evdi\n");
        return false;
    }
    return true;
}

bool vdm_create_display(VirtualDisplayManager *vdm, const char *name, int w, int h)
{
    vdm->width = w;
    vdm->height = h;
    vdm->handle = evdi_open(vdm->device);

    if (!vdm->handle)
        return false;

    // Connect with the defined EDID
    evdi_connect(vdm->handle, display_edid_1080p, display_edid_1080p_len, 0);

    // Setup buffer: 4 bytes per pixel (RGBA/BGRA)
    const size_t bufferSize = w * h * 4;
    vdm->buffer->id = 123;
    vdm->buffer->width = w;
    vdm->buffer->height = h;
    vdm->buffer->stride = 4;
    vdm->buffer->buffer = malloc(bufferSize);
    evdi_register_buffer(vdm->handle, *vdm->buffer);

    return true;
}

bool vdm_set_frame_callback(VirtualDisplayManager *vdm, FrameCallback callback, void *user_data)
{
}

bool vdm_destroy(VirtualDisplayManager *vdm)
{
    if (!vdm)
        return;

    // 1. Tell the OS the monitor is "unplugged"
    if (vdm->handle)
    {
        evdi_disconnect(vdm->handle);

        // 2. Unregister buffers before closing handle
        evdi_unregister_buffer(vdm->handle, vdm->buffer->id);

        // 3. Close the EVDI control node handle
        evdi_close(vdm->handle);
    }

    // 4. Free the pixel buffer we malloc'd in vdm_create_display
    if (vdm->buffer)
    {
        free(vdm->buffer->buffer);
        vdm->buffer->buffer = NULL;
        free(vdm->buffer);
        vdm->buffer = NULL;
    }

    // 5. Free the manager struct itself
    free(vdm);

    printf("Virtual display destroyed and resources freed.\n");
}