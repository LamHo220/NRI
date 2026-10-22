// © 2021 NVIDIA Corporation

#pragma once

#if NRI_ENABLE_D3D12_SUPPORT

#    include <d3d12.h>
#    include <type_traits>
#    include <utility>

// WIDL-generated d3d12.h exposes two source-level COM signatures:
//   - value-returning: T GetDesc(), T GetCPUDescriptorHandleForHeapStart(), ...
//   - out-parameter:   void GetDesc(T*), ...
// The header selects one of them with the condition below. It must be mirrored
// here exactly, otherwise NRI and d3d12.h disagree about the interface layout.
// D3D12MA documents the same workaround in D3D12MemAlloc.cpp.
//
// Do not use this macro at call sites. Use the helpers below instead: they
// return by value in both cases and keep the d3d12.h ABI difference in one place.
#    if defined(_MSC_VER) || !defined(_WIN32)
#        define NRI_D3D12_COM_RETURNS_BY_VALUE 1
#    else
#        define NRI_D3D12_COM_RETURNS_BY_VALUE 0
#    endif

namespace nri {

template <typename T, typename = void>
struct HasValueGetDesc : std::false_type {};

template <typename T>
struct HasValueGetDesc<T, std::void_t<decltype(std::declval<T*>()->GetDesc())>> : std::true_type {};

static_assert(NRI_D3D12_COM_RETURNS_BY_VALUE == HasValueGetDesc<ID3D12Resource>::value,
    "NRI_D3D12_COM_RETURNS_BY_VALUE does not match the signatures in d3d12.h");

// The helpers live directly in "nri", next to the rest of the D3D12 backend.
// The desc getters are named "GetNativeDesc" rather than "GetDesc" so that they
// are not hidden by member functions like DeviceBase::GetDesc() or
// DeviceD3D12::GetResourceDesc() at the call sites.
NRI_INLINE D3D12_RESOURCE_DESC GetNativeDesc(ID3D12Resource* resource) {
#    if NRI_D3D12_COM_RETURNS_BY_VALUE
    return resource->GetDesc();
#    else
    D3D12_RESOURCE_DESC desc = {};
    resource->GetDesc(&desc);
    return desc;
#    endif
}

NRI_INLINE D3D12_HEAP_DESC GetNativeDesc(ID3D12Heap* heap) {
#    if NRI_D3D12_COM_RETURNS_BY_VALUE
    return heap->GetDesc();
#    else
    D3D12_HEAP_DESC desc = {};
    heap->GetDesc(&desc);
    return desc;
#    endif
}

NRI_INLINE D3D12_DESCRIPTOR_HEAP_DESC GetNativeDesc(ID3D12DescriptorHeap* heap) {
#    if NRI_D3D12_COM_RETURNS_BY_VALUE
    return heap->GetDesc();
#    else
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    heap->GetDesc(&desc);
    return desc;
#    endif
}

NRI_INLINE D3D12_COMMAND_QUEUE_DESC GetNativeDesc(ID3D12CommandQueue* queue) {
#    if NRI_D3D12_COM_RETURNS_BY_VALUE
    return queue->GetDesc();
#    else
    D3D12_COMMAND_QUEUE_DESC desc = {};
    queue->GetDesc(&desc);
    return desc;
#    endif
}

NRI_INLINE D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* heap) {
#    if NRI_D3D12_COM_RETURNS_BY_VALUE
    return heap->GetCPUDescriptorHandleForHeapStart();
#    else
    D3D12_CPU_DESCRIPTOR_HANDLE handle = {};
    heap->GetCPUDescriptorHandleForHeapStart(&handle);
    return handle;
#    endif
}

NRI_INLINE D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* heap) {
#    if NRI_D3D12_COM_RETURNS_BY_VALUE
    return heap->GetGPUDescriptorHandleForHeapStart();
#    else
    D3D12_GPU_DESCRIPTOR_HANDLE handle = {};
    heap->GetGPUDescriptorHandleForHeapStart(&handle);
    return handle;
#    endif
}

NRI_INLINE D3D12_RESOURCE_ALLOCATION_INFO GetResourceAllocationInfo(ID3D12Device* device, UINT nodeMask, UINT resourceDescNum, const D3D12_RESOURCE_DESC* resourceDescs) {
#    if NRI_D3D12_COM_RETURNS_BY_VALUE
    return device->GetResourceAllocationInfo(nodeMask, resourceDescNum, resourceDescs);
#    else
    D3D12_RESOURCE_ALLOCATION_INFO info = {};
    device->GetResourceAllocationInfo(&info, nodeMask, resourceDescNum, resourceDescs);
    return info;
#    endif
}

NRI_INLINE LUID GetAdapterLuid(ID3D12Device* device) {
#    if NRI_D3D12_COM_RETURNS_BY_VALUE
    return device->GetAdapterLuid();
#    else
    LUID luid = {};
    device->GetAdapterLuid(&luid);
    return luid;
#    endif
}

} // namespace nri

#endif
