#include "parallel_hash_opencl.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define CL_TARGET_OPENCL_VERSION 100
#include <CL/cl.h>

extern const char astrolabous_parallel_hash_source[];

static int get_platforms(cl_platform_id **p_platforms)
{
	int err;
	cl_uint num_platforms;
	err = clGetPlatformIDs(1, NULL, &num_platforms);
	if (err < 0) return err;
	if (num_platforms == 0) return 0;
	*p_platforms = malloc(sizeof(cl_platform_id) * num_platforms);
	err = clGetPlatformIDs(num_platforms, *p_platforms, NULL);
	if (err < 0) {
		free(*p_platforms);
		return err;
	}
	return num_platforms;
}

static int get_devices(cl_platform_id platform, cl_device_id **p_devices)
{
	int err;
	cl_uint num_devices;
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, NULL, &num_devices);
	if (err < 0) return err;
	if (num_devices == 0) return 0;
	*p_devices = malloc(sizeof(cl_device_id) * num_devices);
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, num_devices, *p_devices, NULL);
	if (err < 0) {
		free(*p_devices);
		return err;
	}
	return num_devices;
}

static int choose_device(cl_device_id *p_device)
{
	int err, plat_idx, dev_idx, num_platforms, num_devices;
	size_t max_work_group_size;
	cl_uint max_compute_units, threads, max_threads = 0;
	cl_bool avail;
	cl_platform_id *platforms;
	cl_device_id *devices;
	num_platforms = get_platforms(&platforms);
	if (num_platforms <= 0) return num_platforms;
	for (plat_idx = 0; plat_idx < num_platforms; ++plat_idx) {
		num_devices = get_devices(platforms[plat_idx], &devices);
		if (num_devices <= 0) {
			free(platforms);
			return num_devices;
		}
		for (dev_idx = 0; dev_idx < num_devices; ++dev_idx) {
			err = clGetDeviceInfo(devices[dev_idx], CL_DEVICE_AVAILABLE, sizeof(avail), &avail, NULL);
			if (err < 0) {
				free(platforms);
				free(devices);
				return err;
			}
			if (!avail) continue;
			err = clGetDeviceInfo(devices[dev_idx], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(max_work_group_size), &max_work_group_size, NULL);
			if (err < 0) {
				free(platforms);
				free(devices);
				return err;
			}
			err = clGetDeviceInfo(devices[dev_idx], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(max_compute_units), &max_compute_units, NULL);
			if (err < 0) {
				free(platforms);
				free(devices);
				return err;
			}
			threads = max_work_group_size * max_compute_units;
			if (threads > max_threads) {
				max_threads = threads;
				*p_device = devices[dev_idx];
			}
		}
		free(devices);
	}
	free(platforms);
	return max_threads;
}

static char *err_str_opencl(int err)
{
	char *buf;
	buf = malloc(20);
	snprintf(buf, 20, "OpenCL error %d", err);
	return buf;
}

struct ctx_opencl {
	cl_device_id *p_device;
	cl_context *p_context;
	cl_program *p_program;
	cl_kernel *p_kernel;
	cl_command_queue *p_queue;
	cl_mem *p_buffer;
};

static void cleanup_opencl(struct ctx_opencl *ctx)
{
	if (ctx->p_buffer)
		clReleaseMemObject(*ctx->p_buffer);
	if (ctx->p_queue)
		clReleaseCommandQueue(*ctx->p_queue);
	if (ctx->p_kernel)
		clReleaseKernel(*ctx->p_kernel);
	if (ctx->p_program)
		clReleaseProgram(*ctx->p_program);
	if (ctx->p_context)
		clReleaseContext(*ctx->p_context);
	/*if (ctx->p_device)
		clReleaseDevice(*ctx->p_device);*/
}

char *astrolabous_parallel_hash_opencl(
		uint32_t *buf, uint32_t n_hash, uint64_t n_iter)
{
	struct ctx_opencl ctx = { 0 };
	size_t work_size;
	size_t prog_size;
	const char *prog_src;
	char *err_str;
	size_t err_size = 0;
	cl_int err;
	cl_device_id device;
	cl_context context;
	cl_program program;
	cl_kernel kernel;
	cl_command_queue queue;
	cl_mem buffer;
	err = choose_device(&device);
	if (err < 0)
		return err_str_opencl(err);
	if (err == 0) {
		err_str = malloc(32);
		strcpy(err_str, "OpenCL device not found");
		return err_str;
	}
	context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
	if (err < 0)
		return err_str_opencl(err);
	ctx.p_context = &context;
	prog_src = astrolabous_parallel_hash_source;
	prog_size = strlen(prog_src);
	program = clCreateProgramWithSource(context, 1, &prog_src, &prog_size, &err);
	if (err < 0) {
		cleanup_opencl(&ctx);
		return err_str_opencl(err);
	}
	ctx.p_program = &program;
	err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
	if (err < 0) {
		if (err == CL_BUILD_PROGRAM_FAILURE) {
			clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &err_size);
			err_size += 40;
			err_str = malloc(err_size);
			strcpy(err_str, "OpenCL program failed building:\n");
			clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, err_size, err_str + strlen(err_str), NULL);
			cleanup_opencl(&ctx);
			return err_str;
		}
		return err_str_opencl(err);
	}
	kernel = clCreateKernel(program, "parallel_hash", &err);
	if (err < 0) {
		cleanup_opencl(&ctx);
		return err_str_opencl(err);
	}
	ctx.p_kernel = &kernel;
	queue = clCreateCommandQueue(context, device, 0, &err);
	if (err < 0) {
		cleanup_opencl(&ctx);
		return err_str_opencl(err);
	}
	ctx.p_queue = &queue;
	buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, n_hash * 32, NULL, &err);
	if (err < 0) {
		cleanup_opencl(&ctx);
		return err_str_opencl(err);
	}
	ctx.p_buffer = &buffer;
	err = clSetKernelArg(kernel, 0, sizeof(buffer), &buffer);
	if (err < 0) {
		cleanup_opencl(&ctx);
		return err_str_opencl(err);
	}
	err = clSetKernelArg(kernel, 1, sizeof(n_iter), &n_iter);
	if (err < 0) {
		cleanup_opencl(&ctx);
		return err_str_opencl(err);
	}
	err = clEnqueueWriteBuffer(queue, buffer, CL_FALSE, 0, n_hash * 32, buf, 0, NULL, NULL);
	if (err < 0) {
		cleanup_opencl(&ctx);
		return err_str_opencl(err);
	}
	work_size = n_hash;
	err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &work_size, NULL, 0, NULL, NULL);
	if (err < 0) {
		cleanup_opencl(&ctx);
		return err_str_opencl(err);
	}
	err = clEnqueueReadBuffer(queue, buffer, CL_TRUE, 0, n_hash * 32, buf, 0, NULL, NULL);
	if (err < 0) {
		cleanup_opencl(&ctx);
		return err_str_opencl(err);
	}
	cleanup_opencl(&ctx);
	return NULL;
}

int astrolabous_opencl_max_threads(void)
{
	cl_device_id device;
	return choose_device(&device);
}
