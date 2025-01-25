#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef __ALGORITHIM_H_
#define __ALGORITHIM_H_

#if _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

#if _WIN32
#define FFI_PLUGIN_EXPORT __declspec(dllexport)
#else
#define FFI_PLUGIN_EXPORT
#endif

/**
 * @brief 计算心率（简化示例，不是完整算法）
 * @param data 指向数据数组的指针
 * @param size 数据数组的大小（即数据点的数量）
 * @param fs 采样频率（每秒采样点数）
 * @return int 返回的心率值
 */
FFI_PLUGIN_EXPORT int hr_algorithim_lib(double *data, int size, int fs);

/**
 * @brief FIR滤波器（简化示例，不是完整算法）
 * @param input 输入信号的一个样本值
 * @return double 返回经过“滤波”后的输出值
 */
FFI_PLUGIN_EXPORT double fir_filter_lib(double input);

#endif
