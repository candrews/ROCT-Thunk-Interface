/*
 * Copyright (C) 2014-2018 Advanced Micro Devices, Inc. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "KFDBaseComponentTest.hpp"
#include "IsaGenerator.hpp"

#ifndef __KFD_MEMORY_TEST__H__
#define __KFD_MEMORY_TEST__H__

/* @class KFDTopologyTest
 * This class has no additional features to KFDBaseComponentTest
 * The separation was made so we are able to group all memory tests together
 */
class KFDMemoryTest :  public KFDBaseComponentTest {
 public:
    KFDMemoryTest(void) :m_pIsaGen(NULL) {}
    ~KFDMemoryTest(void) {}
 protected:
    virtual void SetUp();
    virtual void TearDown();

 protected:
    IsaGenerator* m_pIsaGen;

    void BigBufferVRAM(int defaultGPUNode, HSAuint64 granularityMB, HSAuint64 *lastSize);
    void BigBufferSystemMemory(int defaultGPUNode, HSAuint64 granularityMB, HSAuint64 *lastSize);
};

#endif  // __KFD_MEMORY_TEST__H__
