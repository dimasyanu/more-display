#pragma once

#include <cstdint>
#include <functional>
#include <memory>

// Represents the raw pixel data captured from the OS
struct ScreenFrame
{
  const uint8_t *data;
  int width;
  int height;
  size_t size;
};

class VirtualDisplayManager
{
public:
  // Virtual destructor
  virtual ~VirtualDisplayManager() = default;

  // Setup the driver
  virtual bool initialize() = 0;

  // Create the virtual monitor/display in the OS
  virtual bool createDisplay(const std::string &displayName, int width, int height) = 0;

  // Define what happens when a new frame is ready
  using FrameCallback = std::function<void(const ScreenFrame &)>;
  virtual void setFrameCallback(FrameCallback callback) = 0;

  // Start the event loop
  virtual void run() = 0;

  // Create the correct version (Linux/Windows/Mac) based on the OS
  static std::unique_ptr<VirtualDisplayManager> create();
};
