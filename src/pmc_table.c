/*
 * Copyright © 2014 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including
 * the next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libhsakmt.h"
#include "pmc_table.h"

static uint32_t kaveri_sq_counter_ids[] = {
1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82,
83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101,
102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117,
118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133,
134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 168, 169, 170,
171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186,
187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202,
203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218,
219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234,
235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250
};

/* Unused counters - 166, 292 - 297 */
static uint32_t carrizo_sq_counter_ids[] = {
1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82,
83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101,
102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117,
118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133,
134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165,
167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182,
183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198,
199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214,
215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246,
247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262,
263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278,
279, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 298
};

/* Unused counters - 166, 292 - 297 */
static uint32_t fiji_sq_counter_ids[] = {
1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82,
83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101,
102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117,
118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133,
134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165,
167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182,
183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198,
199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214,
215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246,
247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262,
263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278,
279, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 298
};

/* Unused counters - 163 - 166, 167 and 251 are *_DUMMY_LAST */
static uint32_t hawaii_sq_counter_ids[] = {
1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142,
143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157,
158, 159, 160, 161, 162, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177,
178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192,
193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207,
208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222,
223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237,
238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250
};

/* Polaris 10 and Polaris 11 have the same SQ cpunter IDs */
/* Unused counters - 167 and 275 are *_DUMMY_LAST */
static uint32_t polaris_sq_counter_ids[] = {
1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97,
98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142,
143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157,
158, 159, 160, 161, 162, 163, 164, 165, 168, 169, 170, 171, 172, 173, 174,
175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204,
205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234,
235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249,
250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264,
265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 276, 277, 278, 279, 280,
281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295
};

static struct perf_counter_block kaveri_blocks[PERFCOUNTER_BLOCKID__MAX] = {
	[PERFCOUNTER_BLOCKID__SQ] = {
		.num_of_slots = 8,
		.num_of_counters = sizeof(kaveri_sq_counter_ids) /
					sizeof(*kaveri_sq_counter_ids),
		.counter_ids = kaveri_sq_counter_ids,
		.counter_size_in_bits = 64,
		.counter_mask = BITMASK(64)
	},
};

static struct perf_counter_block carrizo_blocks[PERFCOUNTER_BLOCKID__MAX] = {
	[PERFCOUNTER_BLOCKID__SQ] = {
		.num_of_slots = 8,
		.num_of_counters = sizeof(carrizo_sq_counter_ids) /
					sizeof(*carrizo_sq_counter_ids),
		.counter_ids = carrizo_sq_counter_ids,
		.counter_size_in_bits = 64,
		.counter_mask = BITMASK(64)
	},
};

static struct perf_counter_block fiji_blocks[PERFCOUNTER_BLOCKID__MAX] = {
	[PERFCOUNTER_BLOCKID__SQ] = {
		.num_of_slots = 8,
		.num_of_counters = sizeof(fiji_sq_counter_ids) /
					sizeof(*fiji_sq_counter_ids),
		.counter_ids = fiji_sq_counter_ids,
		.counter_size_in_bits = 64,
		.counter_mask = BITMASK(64)
	},
};

static struct perf_counter_block hawaii_blocks[PERFCOUNTER_BLOCKID__MAX] = {
	[PERFCOUNTER_BLOCKID__SQ] = {
		.num_of_slots = 8,
		.num_of_counters = sizeof(hawaii_sq_counter_ids) /
					sizeof(*hawaii_sq_counter_ids),
		.counter_ids = hawaii_sq_counter_ids,
		.counter_size_in_bits = 64,
		.counter_mask = BITMASK(64)
	},
};

static struct perf_counter_block polaris_blocks[PERFCOUNTER_BLOCKID__MAX] = {
	[PERFCOUNTER_BLOCKID__SQ] = {
		.num_of_slots = 8,
		.num_of_counters = sizeof(polaris_sq_counter_ids) /
					sizeof(*polaris_sq_counter_ids),
		.counter_ids = polaris_sq_counter_ids,
		.counter_size_in_bits = 64,
		.counter_mask = BITMASK(64)
	},
};

/* Current APUs only have one IOMMU. If NUMA is introduced to APUs, we'll need
 * to expand the struct here to an array.
 */
static struct perf_counter_block iommu_block;

static HSAKMT_STATUS
alloc_pmc_blocks_iommu(void)
{
	DIR *dir;
	struct dirent *dent;
	const char sysfs_amdiommu_event_path[] =
			"/sys/bus/event_source/devices/amd_iommu/events";
	/* Counter source in IOMMU's Counter Bank Addressing register is 8 bits,
	 * so the biggest counter number/id possible is 0xff.
	 */
	const int max_counter_id = 0xff;
	char path[256];
	const int len = sizeof(path);
	FILE *file;
	int num;
	char counter_id[max_counter_id + 1];
	HSAKMT_STATUS ret = HSAKMT_STATUS_SUCCESS;
	uint32_t *ptr;
	struct perf_counter_block *block = &iommu_block;

	memset(block, 0, sizeof(struct perf_counter_block));

	dir = opendir(sysfs_amdiommu_event_path);
	if (!dir)
		goto out;

	memset(counter_id, 0, max_counter_id + 1);
	while ((dent = readdir(dir))) {
		if (!strcmp(dent->d_name, ".") || !strcmp(dent->d_name, ".."))
			continue;
		if (snprintf(path, len, "%s/%s", sysfs_amdiommu_event_path,
						dent->d_name) >= len) {
			fprintf(stderr, "Increase path length.\n");
			ret = HSAKMT_STATUS_NO_MEMORY;
			goto out;
		}
		file = fopen(path, "r");
		if (!file) {
			ret = HSAKMT_STATUS_ERROR;
			goto out;
		}
		if (fscanf(file, "csource=0x%x", &num) != 1) {
			ret = HSAKMT_STATUS_ERROR;
			fclose(file);
			goto out;
		}
		if (num > max_counter_id)
		/* This should never happen. If it does, check IOMMU driver. */
			fprintf(stderr,
				"Error: max_counter_id %d is set too small.\n",
				max_counter_id);
		else {
			counter_id[num] = 1;
			++block->num_of_counters;
		}
		fclose(file);
	}

	block->counter_ids = malloc(sizeof(uint32_t) * block->num_of_counters);
	if (!block->counter_ids) {
		ret = HSAKMT_STATUS_NO_MEMORY;
		goto out;
	}
	ptr = block->counter_ids;
	for (num = 0; num < (max_counter_id + 1); num++) {
		if (counter_id[num]) {
			ptr[0] = num;
			++ptr;
		}
	}

	if (snprintf(path, len, "%s/%d/%s",
		"/sys/devices/virtual/kfd/kfd/topology/nodes",
		0, /* IOMMU is in node 0. Change this if NUMA is introduced to APU. */
		"perf/iommu/max_concurrent") >= len) {
		fprintf(stderr, "Increase path length\n");
		ret = HSAKMT_STATUS_NO_MEMORY;
		goto out;
	};
	file = fopen(path, "r");
	if (!file) {
		ret = HSAKMT_STATUS_ERROR;
		goto out;
	}
	if (fscanf(file, "%d", &block->num_of_slots) != 1)
		ret = HSAKMT_STATUS_ERROR;
	fclose(file);

out:
	if (dir)
		closedir(dir);
	return ret;
}

HSAKMT_STATUS alloc_pmc_blocks(void)
{
	return alloc_pmc_blocks_iommu();
}

void free_pmc_blocks(void)
{
	if (iommu_block.counter_ids)
		free(iommu_block.counter_ids);
	iommu_block.counter_ids = NULL;
	iommu_block.num_of_counters = 0;
}

HSAKMT_STATUS
get_block_properties(uint32_t node_id,
                     enum perf_block_id block_id,
                     struct perf_counter_block *block)
{
	HSAKMT_STATUS rc = HSAKMT_STATUS_SUCCESS;
	uint16_t dev_id = get_device_id_by_node(node_id);

	if (block_id > PERFCOUNTER_BLOCKID__MAX ||
			block_id < PERFCOUNTER_BLOCKID__FIRST)
		return HSAKMT_STATUS_INVALID_PARAMETER;

	if (block_id == PERFCOUNTER_BLOCKID__IOMMUV2) {
		*block = iommu_block;
		return HSAKMT_STATUS_SUCCESS;
	}

	/* To avoid the long list, we read the 12 most significant digits of DID
	 * to identify the GPU instead of listing the complete 16 bits. If one
	 * day 12-bits is not good enough to distinguish the GPU, change the
	 * code here.
	 */
	switch(dev_id >> 4) {
	case 0x130:
	case 0x131:
		*block = kaveri_blocks[block_id];
		break;

	case 0x987:
		*block = carrizo_blocks[block_id];
		break;

	case 0x730:
		*block = fiji_blocks[block_id];
		break;

	case 0x67A:
	case 0x67B:
		*block = hawaii_blocks[block_id];
		break;

	case 0x67C:
	case 0x67D:
	case 0x67E:
	case 0x67F:
		*block = polaris_blocks[block_id];
		break;

	default:
		rc = HSAKMT_STATUS_INVALID_PARAMETER;
	}

	return rc;
}


