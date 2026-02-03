#pragma once

#include "../Constants/edid_data.hpp"
#include "../VirtualDisplayManager/VirtualDisplayManager.hpp"
#include "evdi_lib.h"
#include <stdlib.h>
#include <stdio.h>
#include <atomic>

class LinuxEvdiManager : public VirtualDisplayManager {
private:
    evdi_handle handle;
    std::vector<uint8_t> _buffer;
    evdi_buffer* evdiBuffer;
    FrameCallback callback;
    std::atomic<bool> running;
    uint8_t width, height;

    int findEvdiDevice()
    {
        for (int i = 0; i < 64; i++)
        { // Check cards 0 to 63
            enum evdi_device_status status = evdi_check_device(i);
            if (status != AVAILABLE) continue;
            printf("Found available EVDI device at /dev/dri/card%d\n", i);
            return i;
        }
        return -1; // No EVDI device found
    }

public:
    LinuxEvdiManager() : handle(nullptr), running(false) {}
    ~LinuxEvdiManager() {
        stop();
        if (handle) {
            evdi_disconnect(handle);
            evdi_unregister_buffer(handle, evdiBuffer->id);
            evdi_close(handle);
        }
        if (evdiBuffer) {
            free(evdiBuffer->buffer);
            evdiBuffer->buffer = NULL;
            free(evdiBuffer);
            evdiBuffer = NULL;
            _buffer.clear();
        }
    }

    bool initialize() override {
        int deviceId = findEvdiDevice();
        if (deviceId < 0) return false;
        if (evdi_check_device(deviceId) < 0) {
            fprintf(stderr, "EVDI kernel module not found. Run: sudo modprobe evdi\n");
            return false;
        }
        handle = evdi_open(deviceId);
        return handle != nullptr;
    }

    bool createDisplay(const std::string& name, int w, int h) override {
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

    void run() override {
        running = true;
        evdi_event_context ctx = {};
        ctx.user_data = this;
        ctx.update_ready_handler = [](int buffer_id, void* user_data) {
            auto* self = static_cast<LinuxEvdiManager*>(user_data);
            if (!self->callback) return;
            ScreenFrame frame {
                self->_buffer.data(),
                self->width,
                self->height,
                (size_t)self->width * 4
            };
            self->callback(frame);
        };

        while (running) {
            evdi_handle_events(handle, &ctx);
        }
    }

    void stop() {
        running = false;
    }
};
