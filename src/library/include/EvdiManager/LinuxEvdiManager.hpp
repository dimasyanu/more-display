#pragma once

#include "../Constants/edid_data.hpp"
#include "../VirtualDisplayManager/VirtualDisplayManager.hpp"
#include <atomic>
#include <cstddef>
#include <cstring>
#include <evdi_lib.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <xf86drm.h>
#include <iostream>

class LinuxEvdiManager : public VirtualDisplayManager
{
private:
    evdi_handle handle;
    std::vector<uint8_t> _buffer;
    evdi_buffer *evdiBuffer;
    FrameCallback callback;
    std::atomic<bool> running;
    uint8_t width, height;

    bool isEvdiRunning() {
        char path[64];
        for (int i = 0; i < 16; i++) {
            snprintf(path, sizeof(path), "/dev/dri/card%d", i);

            int fd = open(path, O_RDWR, O_CLOEXEC);
            if (fd < 0) continue;

            drmVersionPtr v = drmGetVersion(fd);
            if (!v) {
                close(fd);
                continue;
            }

            bool ok = (strcmp(v->name, "evdi") == 0);
            std::cout << v->name << std::endl;
            drmFreeVersion(v);
            close(fd);
            if (ok)
                return true;
        }
        return false;
    }

    int findEvdiDevice()
    {
        int deviceId = evdi_add_device();
        if (deviceId < 0)
        {
            fprintf(stderr, "evdi_add_device failed\n");
            return -1;
        }

        // if (!isEvdiRunning()) {
        //     return -1;
        // }

        for (int i = 0; i < 10; i++)
        {
            enum evdi_device_status status = evdi_check_device(deviceId);
            if (status != AVAILABLE) {
                usleep(1000000); // 1 second
                fprintf(stdout, "Waiting for device...(%d)\n", i+1);
                continue;
            }
            printf("Found available EVDI device at /dev/dri/card%d\n", i);
            return i;
        }
        return -1; // No EVDI device found
    }

public:
    LinuxEvdiManager() : handle(nullptr), running(false) {}
    ~LinuxEvdiManager()
    {
        stop();
        if (handle)
        {
            evdi_disconnect(handle);
            evdi_unregister_buffer(handle, evdiBuffer->id);
            evdi_close(handle);
        }
        if (evdiBuffer)
        {
            free(evdiBuffer->buffer);
            evdiBuffer->buffer = NULL;
            free(evdiBuffer);
            evdiBuffer = NULL;
            _buffer.clear();
        }
    }

    bool initialize() override
    {
        // int deviceId = findEvdiDevice();
        int deviceId = evdi_add_device();
        if (deviceId < 0) return false;
        handle = evdi_open(deviceId);
        return handle != nullptr;
    }

    bool createDisplay(const std::string &name, int w, int h) override
    {
        width = w, height = h;

        evdi_connect(handle, display_edid_1080p, display_edid_1080p_len, 0);

        _buffer.resize(w * h * 4);
        evdiBuffer->id = 0;
        evdiBuffer->buffer = _buffer.data();
        evdiBuffer->width = w;
        evdiBuffer->height = h;
        evdiBuffer->stride = w * 4;

        evdi_register_buffer(handle, *evdiBuffer);

        return true;
    }

    void setFrameCallback(FrameCallback cb) override { callback = cb; }

    void run() override
    {
        running = true;
        evdi_event_context ctx = {};
        ctx.user_data = this;
        ctx.update_ready_handler = [](int buffer_id, void *user_data)
        {
            auto *self = static_cast<LinuxEvdiManager *>(user_data);
            if (!self->callback)
                return;
            ScreenFrame frame{
                self->_buffer.data(),
                self->width,
                self->height,
                (size_t)self->width * 4};
            self->callback(frame);
        };

        while (running)
        {
            evdi_handle_events(handle, &ctx);
        }
    }

    void stop()
    {
        running = false;
    }
};
