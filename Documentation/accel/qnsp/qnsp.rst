.. SPDX-License-Identifier: GPL-2.0-only

==============================================
Qualcomm Neural Signal Processor (QNSP) Driver
==============================================

Introduction
============

The QNSP (Qualcomm Neural Signal Processor) driver is a DRM-based accelerator
driver for Qualcomm's Hexagon DSPs. It is designed with a modern, modular
architecture that aligns with the Linux kernel's accelerator subsystem
(drivers/accel/).

The driver enables user-space applications to offload tasks to the Hexagon DSPs
(Digital Signal Processor) available on Qualcomm SoCs. This includes support
for offloading to Qualcomm SoCs providing a FastRPC-capable DSPs and is not
limited to compute-intensive or AI workloads. It provides a generic offload
mechanism for various DSP tasks including audio, sensors, and computer vision.

Key Features
============

*   **DRM/Accel Framework Integration**: Fully integrated with the DRM
      accelerator subsystem, exposing a standard character device
      (e.g., `/dev/accel/accel0`).
*   **GEM Memory Management**: Uses the Graphics Execution Manager (GEM) for
      buffer allocation and management, supporting DMA-BUF for efficient buffer
      sharing.
*   **FastRPC Protocol Support**: Implements the FastRPC protocol for remote
      procedure calls to the DSP.
*   **Modular Architecture**: Clean separation of core driver logic, memory
      management, and transport layers.
*   **IOMMU Support**: Robust memory management with IOMMU isolation using
      context banks.
*   **Hexagon DSP Support**: Supports offloading to Qualcomm SoCs providing a
      FastRPC-capable DSPs.

Architecture
============

The QNSP driver is composed of several key components:

1.  **Core Driver (`qnsp_drv`)**: Handles device registration, file operations,
      and integration with the DRM subsystem.
2.  **Memory Manager (`qnsp_memory_manager`)**: Manages IOMMU context banks and
      memory allocations. It supports pluggable backends (currently DMA-coherent)
      for flexibility.
3.  **GEM Subsystem (`qnsp_gem`)**: Implements DRM GEM callbacks for buffer object
      creation, mapping, and lifecycle management.
4.  **RPMsg Transport (`qnsp_rpmsg`)**: Handles low-level communication with the
      DSP via the RPMsg framework.
5.  **Compute Bus (`qnsp_compute_bus`)**: A custom bus type for enumerating and
      managing compute context bank devices.
6.  **FastRPC Layer (`qnsp_fastrpc`, `qnsp_fastrpc_ops`)**: Implements the FastRPC
      protocol logic and IOCTL interfaces for user-space.

User-Space API
==============

The driver exposes a set of IOCTLs for user-space applications to interact with
the DSP. The key IOCTLs are:

*   `DRM_IOCTL_QNSP_QUERY`: Query device information and capabilities.
*   `DRM_IOCTL_QNSP_INIT_ATTACH`: Attach to the DSP's root protection domain.
*   `DRM_IOCTL_QNSP_INIT_CREATE`: Create a new process on the DSP.
*   `DRM_IOCTL_QNSP_INVOKE`: Invoke a remote method on the DSP.
*   `DRM_IOCTL_QNSP_GEM_CREATE`: Allocate a GEM buffer object.
*   `DRM_IOCTL_QNSP_GEM_MMAP_OFFSET`: Get the mmap offset for a GEM object.
*   `DRM_IOCTL_QNSP_MAP`: Map a buffer into the DSP's address space.
*   `DRM_IOCTL_QNSP_MUNMAP`: Unmap a buffer from the DSP.

Usage Example
=============

A typical flow for a user-space application using the QNSP driver would be:

1.  Open the accelerator device node (`/dev/accel/accel*`).
2.  Query device info using `DRM_IOCTL_QNSP_QUERY` to identify the DSP (optional).
3.  Attach to the DSP using `DRM_IOCTL_QNSP_INIT_ATTACH`.
4.  Create a DSP process using `DRM_IOCTL_QNSP_INIT_CREATE`.
5.  Allocate memory buffers using `DRM_IOCTL_QNSP_GEM_CREATE` or import DMA-BUFs.
6.  Map buffers to the DSP using `DRM_IOCTL_QNSP_MAP` (if necessary).
7.  Invoke remote methods using `DRM_IOCTL_QNSP_INVOKE`.
8.  Clean up resources (unmap, close handles) when done.

Internal Implementation
=======================

Memory Management
-----------------

The memory manager handles device-specific memory allocation. It creates virtual
"IOMMU devices" corresponding to the hardware context banks defined in the device
tree.

*   **Manual Enumeration**: The driver manually parses the device tree to find
      `compute-cb` nodes and creates devices on the `qnsp-compute-cb` bus.
*   **DMA Coherent Backend**: Currently, a DMA-coherent backend is used for memory
      allocation, ensuring cache coherency between the CPU and DSP.

FastRPC Protocol
----------------

The driver implements the FastRPC protocol, which involves marshalling arguments
(scalars and buffers) into a message format that the DSP understands.

*   **Invocation Context**: Each remote call is tracked by a
      `fastrpc_invoke_context` structure.
*   **Message Passing**: Messages are sent over RPMsg channels.
*   **Response Handling**: The driver waits for a response from the DSP and
      unmarshals the results back to user-space.

Debugging
=========

The driver provides extensive dynamic debug logging (`qnsp_dbg`) which can be
enabled at runtime.
