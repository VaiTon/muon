/* SPDX-FileCopyrightText: VaiTon <eyadlorenzo@gmail.com> */
/* SPDX-License-Identifier: GPL-3.0-only */

#include <mmintrin.h>

int
simd_mmx(void)
{
	return _mm_cvtsi64_si32(_mm_setzero_si64());
}
