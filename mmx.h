/* Copyright (c) 2017 Evan Nemerson <evan@nemerson.com>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#if !defined(SIMDE__MMX_H)
#  if !defined(SIMDE__MMX_H)
#    define SIMDE__MMX_H
#  endif
#  include "simde-common.h"

#  if defined(SIMDE_MMX_FORCE_NATIVE)
#    define SIMDE_MMX_NATIVE
#  elif defined(__MMX__) && (!defined(SIMDE_MMX_NO_NATIVE) && !defined(SIMDE_NO_NATIVE))
#    define SIMDE_MMX_NATIVE
#  endif

#  if defined(SIMDE_MMX_NATIVE)
#    include <mmintrin.h>
#  endif
#  include <stdint.h>
#  include <limits.h>
#  include <stdlib.h>
#  include <string.h>

typedef SIMDE__ALIGN(16) union {
#if defined(SIMDE__ENABLE_GCC_VEC_EXT)
  int8_t          i8 __attribute__((__vector_size__(8), __may_alias__));
  int16_t        i16 __attribute__((__vector_size__(8), __may_alias__));
  int32_t        i32 __attribute__((__vector_size__(8), __may_alias__));
  int64_t        i64 __attribute__((__vector_size__(8), __may_alias__));
  uint8_t         u8 __attribute__((__vector_size__(8), __may_alias__));
  uint16_t       u16 __attribute__((__vector_size__(8), __may_alias__));
  uint32_t       u32 __attribute__((__vector_size__(8), __may_alias__));
  uint64_t       u64 __attribute__((__vector_size__(8), __may_alias__));
  float          f32 __attribute__((__vector_size__(8), __may_alias__));
#else
  int8_t         i8[8];
  int16_t        i16[4];
  int32_t        i32[2];
  int64_t        i64[1];
  uint8_t        u8[8];
  uint16_t       u16[4];
  uint32_t       u32[2];
  uint64_t       u64[1];
  float          f32[2];
#endif

#if defined(SIMDE_MMX_NATIVE)
  __m64          n;
#endif
} simde__m64;

#if defined(SIMDE_MMX_NATIVE)
HEDLEY_STATIC_ASSERT(sizeof(__m64) == sizeof(simde__m64), "__m64 size incorrect");
#define SIMDE__M64_C(expr) ((simde__m64) { .n = expr })
#endif

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_add_pi8 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_add_pi8(a.n, b.n));
#else
  simde__m64 r;
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < 8 ; i++) {
    r.i8[i] = a.i8[i] + b.i8[i];
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_paddb (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_paddb(a.n, b.n));
#else
  return simde_mm_add_pi8(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_add_pi16 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_add_pi16(a.n, b.n));
#else
  simde__m64 r;
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (8 / sizeof(int16_t)) ; i++) {
    r.i16[i] = a.i16[i] + b.i16[i];
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_paddw (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_paddw(a.n, b.n));
#else
  return simde_mm_add_pi16(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_add_pi32 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_add_pi32(a.n, b.n));
#else
  simde__m64 r;
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (8 / sizeof(int32_t)) ; i++) {
    r.i32[i] = a.i32[i] + b.i32[i];
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_paddd (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_paddd(a.n, b.n));
#else
  return simde_mm_add_pi32(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_adds_pi8 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_adds_pi8(a.n, b.n));
#else
  simde__m64 r;
  SIMDE__VECTORIZE
  for (int i = 0 ; i < 8 ; i++) {
    if ((((b.i8[i]) > 0) && ((a.i8[i]) > (INT8_MAX - (b.i8[i]))))) {
      r.i8[i] = INT8_MAX;
    } else if ((((b.i8[i]) < 0) && ((a.i8[i]) < (CHAR_MIN - (b.i8[i]))))) {
      r.i8[i] = CHAR_MIN;
    } else {
      r.i8[i] = (a.i8[i]) + (b.i8[i]);
    }
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_paddsb (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_paddsb(a.n, b.n));
#else
  return simde_mm_adds_pi8(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_adds_pu8 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_adds_pu8(a.n, b.n));
#else
  simde__m64 r;
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < 8 ; i++) {
    const int32_t x = a.u8[i] + b.u8[i];
    if (x < 0)
      r.u8[i] = 0;
    else if (x > UINT8_MAX)
      r.u8[i] = UINT8_MAX;
    else
      r.u8[i] = (uint8_t) x;
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_paddusb (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_paddusb(a.n, b.n));
#else
  return simde_mm_adds_pu8(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_adds_pi16 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_adds_pi16(a.n, b.n));
#else
  simde__m64 r;
  SIMDE__VECTORIZE
  for (int i = 0 ; i < 4 ; i++) {
    if ((((b.i16[i]) > 0) && ((a.i16[i]) > (INT16_MAX - (b.i16[i]))))) {
      r.i16[i] = INT16_MAX;
    } else if ((((b.i16[i]) < 0) && ((a.i16[i]) < (SHRT_MIN - (b.i16[i]))))) {
      r.i16[i] = SHRT_MIN;
    } else {
      r.i16[i] = (a.i16[i]) + (b.i16[i]);
    }
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_paddsw (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_paddsw(a.n, b.n));
#else
  return simde_mm_adds_pi16(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_adds_pu16 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_adds_pu16(a.n, b.n));
#else
  simde__m64 r;
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (8 / sizeof(int16_t)) ; i++) {
    const uint32_t x = a.u16[i] + b.u16[i];
    if (x > UINT16_MAX)
      r.u16[i] = UINT16_MAX;
    else
      r.u16[i] = (uint16_t) x;
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_paddusw (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_paddusw(a.n, b.n));
#else
  return simde_mm_adds_pu16(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_and_si64 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_and_si64(a.n, b.n));
#else
  return (simde__m64) { .i64 = { a.i64[0] & b.i64[0] } };
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_pand (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_pand(a.n, b.n));
#else
  return simde_mm_and_si64(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_andnot_si64 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_andnot_si64(a.n, b.n));
#else
  return (simde__m64) { .i64 = { ~(a.i64[0]) & b.i64[0] } };
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_pandn (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_pandn(a.n, b.n));
#else
  return simde_mm_andnot_si64(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_cmpeq_pi8 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_cmpeq_pi8(a.n, b.n));
#else
  simde__m64 r;
  SIMDE__VECTORIZE
  for (int i = 0 ; i < 8 ; i++) {
    r.i8[i] = (a.i8[i] == b.i8[i]) * 0xff;
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_pcmpeqb (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_pcmpeqb(a.n, b.n));
#else
  return simde_mm_cmpeq_pi8(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_cmpeq_pi16 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_cmpeq_pi16(a.n, b.n));
#else
  simde__m64 r;
  SIMDE__VECTORIZE
  for (int i = 0 ; i < 4 ; i++) {
    r.i16[i] = (a.i16[i] == b.i16[i]) * 0xffff;
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_pcmpeqw (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_pcmpeqw(a.n, b.n));
#else
  return simde_mm_cmpeq_pi16(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_cmpeq_pi32 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_cmpeq_pi32(a.n, b.n));
#else
  simde__m64 r;
  SIMDE__VECTORIZE
  for (int i = 0 ; i < 2 ; i++) {
    r.i32[i] = (a.i32[i] == b.i32[i]) * 0xffffffff;
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_pcmpeqd (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_pcmpeqd(a.n, b.n));
#else
  return simde_mm_cmpeq_pi32(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_cmpgt_pi8 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_cmpgt_pi8(a.n, b.n));
#else
  simde__m64 r;
  SIMDE__VECTORIZE
  for (int i = 0 ; i < 8 ; i++) {
    r.i8[i] = (a.i8[i] > b.i8[i]) * 0xff;
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_pcmpgtb (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_pcmpgtb(a.n, b.n));
#else
  return simde_mm_cmpgt_pi8(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_cmpgt_pi16 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_cmpgt_pi16(a.n, b.n));
#else
  simde__m64 r;
  SIMDE__VECTORIZE
  for (int i = 0 ; i < 4 ; i++) {
    r.i16[i] = (a.i16[i] > b.i16[i]) * 0xffff;
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_pcmpgtw (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_pcmpgtw(a.n, b.n));
#else
  return simde_mm_cmpgt_pi16(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_cmpgt_pi32 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_cmpgt_pi32(a.n, b.n));
#else
  simde__m64 r;
  SIMDE__VECTORIZE
  for (int i = 0 ; i < 2 ; i++) {
    r.i32[i] = (a.i32[i] > b.i32[i]) * 0xffffffff;
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_pcmpgtd (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_pcmpgtd(a.n, b.n));
#else
  return simde_mm_cmpgt_pi32(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
int64_t
simde_mm_cvtm64_si64 (simde__m64 a) {
#if defined(SIMDE_MMX_NATIVE)
#  if defined(__PGI)
  return *((int64_t*) &a);
#  else
  return _mm_cvtm64_si64(a.n);
#  endif
#else
  return a.i64[0];
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_cvtsi32_si64 (int32_t a) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_cvtsi32_si64(a));
#else
  return (simde__m64) { .i32 = { a, 0 } };
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_from_int (int32_t a) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_from_int(a));
#else
  return simde_mm_cvtsi32_si64(a);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_cvtsi64_m64 (int64_t a) {
#if defined(SIMDE_MMX_NATIVE)
#  if defined(__PGI)
  simde__m64 r;
  *((int64_t*) &r) = a;
  return r;
#  else
  return SIMDE__M64_C(_mm_cvtsi64_m64(a));
#  endif
#else
  return (simde__m64) { .i64 = { a } };
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_from_int64 (int64_t a) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_from_int64(a));
#else
  return simde_mm_cvtsi64_m64(a);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
int32_t
simde_mm_cvtsi64_si32 (simde__m64 a) {
#if defined(SIMDE_MMX_NATIVE)
  return _mm_cvtsi64_si32(a.n);
#else
  return a.i32[0];
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
void
simde_mm_empty (void) {
#if defined(SIMDE_MMX_NATIVE)
  _mm_empty();
#else
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
void
simde_m_empty (void) {
#if defined(SIMDE_MMX_NATIVE)
  _m_empty();
#else
  simde_mm_empty();
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_madd_pi16 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_madd_pi16(a.n, b.n));
#else
  simde__m64 r;
  SIMDE__VECTORIZE
  for (int i = 0 ; i < 4 ; i += 2) {
    r.i32[i / 2] = (a.i16[i] * b.i16[i]) + (a.i16[i + 1] * b.i16[i + 1]);
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_pmaddwd (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_pmaddwd(a.n, b.n));
#else
  return simde_mm_madd_pi16(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_mulhi_pi16 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_mulhi_pi16(a.n, b.n));
#else
  simde__m64 r;
  SIMDE__VECTORIZE
  for (int i = 0 ; i < 4 ; i++) {
    r.i16[i] = (int16_t) ((a.i16[i] * b.i16[i]) >> 16);
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_pmulhw (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_pmulhw(a.n, b.n));
#else
  return simde_mm_mulhi_pi16(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_mullo_pi16 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_mullo_pi16(a.n, b.n));
#else
  simde__m64 r;
  SIMDE__VECTORIZE
  for (int i = 0 ; i < 4 ; i++) {
    r.i16[i] = (int16_t) ((a.i16[i] * b.i16[i]) & 0xffff);
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_pmullw (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_pmullw(a.n, b.n));
#else
  return simde_mm_mullo_pi16(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_or_si64 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_or_si64(a.n, b.n));
#else
  return (simde__m64) { .i64 = { a.i64[0] | b.i64[0] } };
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_por (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_por(a.n, b.n));
#else
  return simde_mm_or_si64(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_packs_pi16 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_packs_pi16(a.n, b.n));
#else
  simde__m64 r;

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (8 / sizeof(int16_t)) ; i++) {
    if (a.i16[i] < CHAR_MIN) {
      r.i8[i] = CHAR_MIN;
    } else if (a.i16[i] > INT8_MAX) {
      r.i8[i] = INT8_MAX;
    } else {
      r.i8[i] = (int8_t) a.i16[i];
    }
  }

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (8 / sizeof(int16_t)) ; i++) {
    if (b.i16[i] < CHAR_MIN) {
      r.i8[i + 4] = CHAR_MIN;
    } else if (b.i16[i] > INT8_MAX) {
      r.i8[i + 4] = INT8_MAX;
    } else {
      r.i8[i + 4] = (int8_t) b.i16[i];
    }
  }

  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_packsswb (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_packsswb(a.n, b.n));
#else
  return simde_mm_packs_pi16(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_packs_pi32 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_packs_pi32(a.n, b.n));
#else
  simde__m64 r;

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (8 / sizeof(a.i32[0])) ; i++) {
    if (a.i32[i] < SHRT_MIN) {
      r.i16[i] = SHRT_MIN;
    } else if (a.i32[i] > INT16_MAX) {
      r.i16[i] = INT16_MAX;
    } else {
      r.i16[i] = (int16_t) a.i32[i];
    }
  }

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (8 / sizeof(b.i32[0])) ; i++) {
    if (b.i32[i] < SHRT_MIN) {
      r.i16[i + 2] = SHRT_MIN;
    } else if (b.i32[i] > INT16_MAX) {
      r.i16[i + 2] = INT16_MAX;
    } else {
      r.i16[i + 2] = (int16_t) b.i32[i];
    }
  }

  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_packssdw (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_packssdw(a.n, b.n));
#else
  return simde_mm_packs_pi32(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_packs_pu16 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_packs_pu16(a.n, b.n));
#else
  simde__m64 r;

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (8 / sizeof(int16_t)) ; i++) {
    if (a.i16[i] > UINT8_MAX) {
      r.u8[i] = UINT8_MAX;
    } else if (a.i16[i] < 0) {
      r.u8[i] = 0;
    } else {
      r.u8[i] = (int8_t) a.i16[i];
    }
  }

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (8 / sizeof(int16_t)) ; i++) {
    if (b.i16[i] > UINT8_MAX) {
      r.u8[i + 4] = UINT8_MAX;
    } else if (b.i16[i] < 0) {
      r.u8[i + 4] = 0;
    } else {
      r.u8[i + 4] = (int8_t) b.i16[i];
    }
  }

  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_packuswb (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_packuswb(a.n, b.n));
#else
  return simde_mm_packs_pu16(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_set_pi8 (int8_t e7, int8_t e6, int8_t e5, int8_t e4, int8_t e3, int8_t e2, int8_t e1, int8_t e0) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_set_pi8(e7, e6, e5, e4, e3, e2, e1, e0));
#else
  return (simde__m64) { .i8 = { e0, e1, e2, e3, e4, e5, e6, e7 } };
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_x_mm_set_pu8 (uint8_t e7, uint8_t e6, uint8_t e5, uint8_t e4, uint8_t e3, uint8_t e2, uint8_t e1, uint8_t e0) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_set_pi8((int8_t) e7, (int8_t) e6, (int8_t) e5, (int8_t) e4,
				  (int8_t) e3, (int8_t) e2, (int8_t) e1, (int8_t) e0));
#else
  return (simde__m64) { .u8 = { e0, e1, e2, e3, e4, e5, e6, e7 } };
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_set_pi16 (int16_t e3, int16_t e2, int16_t e1, int16_t e0) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_set_pi16(e3, e2, e1, e0));
#else
  return (simde__m64) { .i16 = { e0, e1, e2, e3 } };
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_x_mm_set_pu16 (uint16_t e3, uint16_t e2, uint16_t e1, uint16_t e0) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_set_pi16((int16_t) e3, (int16_t) e2, (int16_t) e1, (int16_t) e0));
#else
  return (simde__m64) { .u16 = { e0, e1, e2, e3 } };
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_set_pi32 (int32_t e1, int32_t e0) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_set_pi32(e1, e0));
#else
  return (simde__m64) { .i32 = { e0, e1 } };
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_set1_pi8 (int8_t a) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_set1_pi8(a));
#else
  return simde_mm_set_pi8(a, a, a, a, a, a, a, a);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_set1_pi16 (int16_t a) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_set1_pi16(a));
#else
  return simde_mm_set_pi16(a, a, a, a);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_set1_pi32 (int8_t a) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_set1_pi32(a));
#else
  return simde_mm_set_pi32(a, a);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_setr_pi8 (int8_t e7, int8_t e6, int8_t e5, int8_t e4, int8_t e3, int8_t e2, int8_t e1, int8_t e0) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_setr_pi8(e7, e6, e5, e4, e3, e2, e1, e0));
#else
  return (simde__m64) { .i8 = { e7, e6, e5, e4, e3, e2, e1, e0 } };
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_setr_pi16 (int16_t e3, int16_t e2, int16_t e1, int16_t e0) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_setr_pi16(e3, e2, e1, e0));
#else
  return (simde__m64) { .i16 = { e3, e2, e1, e0 } };
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_setr_pi32 (int32_t e1, int32_t e0) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_setr_pi32(e1, e0));
#else
  return (simde__m64) { .i32 = { e1, e0 } };
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_setzero_si64 (void) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_setzero_si64());
#else
  return (simde__m64) { .i32 = { 0, 0 } };
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_sll_pi16 (simde__m64 a, simde__m64 count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_sll_pi16(a.n, count.n));
#else
  simde__m64 r;

  if (HEDLEY_UNLIKELY(count.u64[0] > 15)) {
    memset(&r, 0, sizeof(r));
    return r;
  }

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r.u16) / sizeof(r.u16[0])) ; i++) {
    r.u16[i] = a.u16[i] << count.u64[0];
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_psllw (simde__m64 a, simde__m64 count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_psllw(a.n, count.n));
#else
  return simde_mm_sll_pi16(a, count);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_sll_pi32 (simde__m64 a, simde__m64 count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_sll_pi32(a.n, count.n));
#else
  simde__m64 r;

  if (HEDLEY_UNLIKELY(count.u64[0] > 31)) {
    memset(&r, 0, sizeof(r));
    return r;
  }

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r.u32) / sizeof(r.u32[0])) ; i++) {
    r.u32[i] = a.u32[i] << count.u64[0];
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_pslld (simde__m64 a, simde__m64 count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_pslld(a.n, count.n));
#else
  return simde_mm_sll_pi32(a, count);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_slli_pi16 (simde__m64 a, int count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_slli_pi16(a.n, count));
#else
  simde__m64 r;

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (sizeof(r.u16) / sizeof(r.u16[0])) ; i++) {
    r.u16[i] = a.u16[i] << count;
  }

  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_psllwi (simde__m64 a, int count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_psllwi(a.n, count));
#else
  return simde_mm_slli_pi16(a, count);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_slli_pi32 (simde__m64 a, int count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_slli_pi32(a.n, count));
#else
  simde__m64 r;

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (8 / sizeof(int)) ; i++) {
    r.u32[i] = a.u32[i] << count;
  }

  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_pslldi (simde__m64 a, int count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_pslldi(a.n, count));
#else
  return simde_mm_slli_pi32(a, count);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_slli_si64 (simde__m64 a, int count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_slli_si64(a.n, count));
#else
  return (simde__m64) { .u64 = { a.u64[0] << count } };
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_psllqi (simde__m64 a, int count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_psllqi(a.n, count));
#else
  return simde_mm_slli_si64(a, count);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_sll_si64 (simde__m64 a, simde__m64 count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_sll_si64(a.n, count.n));
#else
  simde__m64 r;

  if (HEDLEY_UNLIKELY(count.u64[0] > 63)) {
    memset(&r, 0, sizeof(r));
    return r;
  }

  r.u64[0] = a.u64[0] << count.u64[0];

  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_psllq (simde__m64 a, simde__m64 count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_psllq(a.n, count.n));
#else
  return simde_mm_sll_si64(a, count);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_srl_pi16 (simde__m64 a, simde__m64 count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_srl_pi16(a.n, count.n));
#else
  simde__m64 r;

  if (HEDLEY_UNLIKELY(count.u64[0] > 15)) {
    memset(&r, 0, sizeof(r));
    return r;
  }

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < sizeof(r.u16) / sizeof(r.u16[0]) ; i++) {
    r.u16[i] = a.u16[i] >> count.u64[0];
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_psrlw (simde__m64 a, simde__m64 count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_psrlw(a.n, count.n));
#else
  return simde_mm_srl_pi16(a, count);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_srl_pi32 (simde__m64 a, simde__m64 count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_srl_pi32(a.n, count.n));
#else
  simde__m64 r;

  if (HEDLEY_UNLIKELY(count.u64[0] > 31)) {
    memset(&r, 0, sizeof(r));
    return r;
  }

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < sizeof(r.u32) / sizeof(r.u32[0]) ; i++) {
    r.u32[i] = a.u32[i] >> count.u64[0];
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_psrld (simde__m64 a, simde__m64 count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_psrld(a.n, count.n));
#else
  return simde_mm_srl_pi32(a, count);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_srli_pi16 (simde__m64 a, int count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_srli_pi16(a.n, count));
#else
  simde__m64 r;

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (8 / sizeof(uint16_t)) ; i++) {
    r.u16[i] = a.u16[i] >> count;
  }

  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_psrlwi (simde__m64 a, int count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_psrlwi(a.n, count));
#else
  return simde_mm_srli_pi16(a, count);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_srli_pi32 (simde__m64 a, int count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_srli_pi32(a.n, count));
#else
  simde__m64 r;

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (8 / sizeof(int)) ; i++) {
    r.u32[i] = a.u32[i] >> count;
  }

  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_psrldi (simde__m64 a, int count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_psrldi(a.n, count));
#else
  return simde_mm_srli_pi32(a, count);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_srli_si64 (simde__m64 a, int count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_srli_si64(a.n, count));
#else
  return (simde__m64) { .u64 = { a.u64[0] >> count } };
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_psrlqi (simde__m64 a, int count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_psrlqi(a.n, count));
#else
  return simde_mm_srli_si64(a, count);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_srl_si64 (simde__m64 a, simde__m64 count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_srl_si64(a.n, count.n));
#else
  simde__m64 r;

  if (HEDLEY_UNLIKELY(count.u64[0] > 63)) {
    memset(&r, 0, sizeof(r));
    return r;
  }

  r.u64[0] = a.u64[0] >> count.u64[0];
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_psrlq (simde__m64 a, simde__m64 count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_psrlq(a.n, count.n));
#else
  return simde_mm_srl_si64(a, count);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_srai_pi16 (simde__m64 a, int count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_srai_pi16(a.n, count));
#else
  simde__m64 r;

  const uint16_t m = (uint16_t) ((~0U) << ((sizeof(int16_t) * CHAR_BIT) - count));

  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (8 / sizeof(int16_t)) ; i++) {
    const uint16_t is_neg = ((uint16_t) (((a.u16[i]) >> ((sizeof(int16_t) * CHAR_BIT) - 1))));
    r.u16[i] = (a.u16[i] >> count) | (m * is_neg);
  }

  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_psrawi (simde__m64 a, int count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_psrawi(a.n, count));
#else
  return simde_mm_srai_pi16(a, count);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_srai_pi32 (simde__m64 a, int count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_srai_pi32(a.n, count));
#else
  simde__m64 r;

  const uint32_t m = (uint32_t) ((~0U) << ((sizeof(int) * CHAR_BIT) - count));
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (8 / sizeof(int)) ; i++) {
    const uint32_t is_neg = ((uint32_t) (((a.u32[i]) >> ((sizeof(int) * CHAR_BIT) - 1))));
    r.u32[i] = (a.u32[i] >> count) | (m * is_neg);
  }

  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_psradi (simde__m64 a, int count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_psradi(a.n, count));
#else
  return simde_mm_srai_pi32(a, count);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_sra_pi16 (simde__m64 a, simde__m64 count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_sra_pi16(a.n, count.n));
#else
  simde__m64 r;
  int cnt = (int) count.i64[0];

  if (cnt > 15 || cnt < 0) {
    for (size_t i = 0 ; i < (sizeof(r.i16) / sizeof(r.i16[0])) ; i++) {
      r.u16[i] = (a.i16[i] < 0) ? 0xffff : 0x0000;
    }
  } else {
    const uint16_t m = (uint16_t) ((~0U) << ((sizeof(int16_t) * CHAR_BIT) - cnt));
    for (size_t i = 0 ; i < (sizeof(r.i16) / sizeof(r.i16[0])) ; i++) {
      const uint16_t is_neg = a.i16[i] < 0;
      r.u16[i] = (a.u16[i] >> cnt) | (m * is_neg);
    }
  }

  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_psraw (simde__m64 a, simde__m64 count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_psraw(a.n, count.n));
#else
  return simde_mm_sra_pi16(a, count);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_sra_pi32 (simde__m64 a, simde__m64 count) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_sra_pi32(a.n, count.n));
#else
  simde__m64 r;
  const uint64_t cnt = count.u64[0];

  if (cnt > 31) {
    for (size_t i = 0 ; i < (sizeof(r.i32) / sizeof(r.i32[0])) ; i++) {
      r.u32[i] = (a.i32[i] < 0) ? UINT32_MAX : 0;
    }
  } else if (cnt == 0) {
    memcpy(&r, &a, sizeof(r));
  } else {
    const uint32_t m = (uint32_t) ((~0U) << ((sizeof(int32_t) * CHAR_BIT) - cnt));
    for (size_t i = 0 ; i < (sizeof(r.i32) / sizeof(r.i32[0])) ; i++) {
      const uint32_t is_neg = a.i32[i] < 0;
      r.u32[i] = (a.u32[i] >> cnt) | (m * is_neg);
    }
  }

  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_psrad (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_psrad(a.n, b.n));
#else
  return simde_mm_sra_pi32(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_sub_pi8 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_sub_pi8(a.n, b.n));
#else
  simde__m64 r;
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < 8 ; i++) {
    r.i8[i] = a.i8[i] - b.i8[i];
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_psubb (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_psubb(a.n, b.n));
#else
  return simde_mm_sub_pi8(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_sub_pi16 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_sub_pi16(a.n, b.n));
#else
  simde__m64 r;
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (8 / sizeof(int16_t)) ; i++) {
    r.i16[i] = a.i16[i] - b.i16[i];
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_psubw (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_psubw(a.n, b.n));
#else
  return simde_mm_sub_pi16(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_sub_pi32 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_sub_pi32(a.n, b.n));
#else
  simde__m64 r;
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (8 / sizeof(int)) ; i++) {
    r.i32[i] = a.i32[i] - b.i32[i];
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_psubd (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_psubd(a.n, b.n));
#else
  return simde_mm_sub_pi32(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_subs_pi8 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_subs_pi8(a.n, b.n));
#else
  simde__m64 r;
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (8) ; i++) {
    if (((b.i8[i]) > 0 && (a.i8[i]) < CHAR_MIN + (b.i8[i]))) {
      r.i8[i] = CHAR_MIN;
    } else if ((b.i8[i]) < 0 && (a.i8[i]) > INT8_MAX + (b.i8[i])) {
      r.i8[i] = INT8_MAX;
    } else {
      r.i8[i] = (a.i8[i]) - (b.i8[i]);
    }
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_psubsb (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_psubsb(a.n, b.n));
#else
  return simde_mm_subs_pi8(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_subs_pu8 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_subs_pu8(a.n, b.n));
#else
  simde__m64 r;
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (8) ; i++) {
    const int32_t x = a.u8[i] - b.u8[i];
    if (x < 0) {
      r.u8[i] = 0;
    } else if (x > UINT8_MAX) {
      r.u8[i] = UINT8_MAX;
    } else {
      r.u8[i] = (uint8_t) x;
    }
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_psubusb (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_psubusb(a.n, b.n));
#else
  return simde_mm_subs_pu8(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_subs_pi16 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_subs_pi16(a.n, b.n));
#else
  simde__m64 r;
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (8 / sizeof(int16_t)) ; i++) {
    if (((b.i16[i]) > 0 && (a.i16[i]) < SHRT_MIN + (b.i16[i]))) {
      r.i16[i] = SHRT_MIN;
    } else if ((b.i16[i]) < 0 && (a.i16[i]) > INT16_MAX + (b.i16[i])) {
      r.i16[i] = INT16_MAX;
    } else {
      r.i16[i] = (a.i16[i]) - (b.i16[i]);
    }
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_psubsw (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_psubsw(a.n, b.n));
#else
  return simde_mm_subs_pi16(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_subs_pu16 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_subs_pu16(a.n, b.n));
#else
  simde__m64 r;
  SIMDE__VECTORIZE
  for (size_t i = 0 ; i < (8 / sizeof(uint16_t)) ; i++) {
    const int x = a.u16[i] - b.u16[i];
    if (x < 0) {
      r.u16[i] = 0;
    } else if (x > UINT16_MAX) {
      r.u16[i] = UINT16_MAX;
    } else {
      r.u16[i] = (uint16_t) x;
    }
  }
  return r;
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_psubusw (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_psubusw(a.n, b.n));
#else
  return simde_mm_subs_pu16(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_unpackhi_pi8 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_unpackhi_pi8(a.n, b.n));
#else
  return (simde__m64) { .i8 = {
      a.i8[4], b.i8[4], a.i8[5], b.i8[5],
      a.i8[6], b.i8[6], a.i8[7], b.i8[7]
    }
  };
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_punpckhbw (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_punpckhbw(a.n, b.n));
#else
  return simde_mm_unpackhi_pi8(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_unpackhi_pi16 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_unpackhi_pi16(a.n, b.n));
#else
  return (simde__m64) { .i16 = { a.i16[2], b.i16[2], a.i16[3], b.i16[3] } };
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_punpckhwd (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_punpckhwd(a.n, b.n));
#else
  return simde_mm_unpackhi_pi16(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_unpackhi_pi32 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_unpackhi_pi32(a.n, b.n));
#else
  return (simde__m64) { .i32 = { a.i32[1], b.i32[1] } };
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_punpckhdq (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_punpckhdq(a.n, b.n));
#else
  return simde_mm_unpackhi_pi32(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_unpacklo_pi8 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_unpacklo_pi8(a.n, b.n));
#else
  return (simde__m64) { .i8 = {
      a.i8[0], b.i8[0], a.i8[1], b.i8[1],
      a.i8[2], b.i8[2], a.i8[3], b.i8[3]
    }
  };
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_punpcklbw (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_punpcklbw(a.n, b.n));
#else
  return simde_mm_unpacklo_pi8(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_unpacklo_pi16 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_unpacklo_pi16(a.n, b.n));
#else
  return (simde__m64) { .i16 = { a.i16[0], b.i16[0], a.i16[1], b.i16[1] } };
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_punpcklwd (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_punpcklwd(a.n, b.n));
#else
  return simde_mm_unpacklo_pi16(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_unpacklo_pi32 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_unpacklo_pi32(a.n, b.n));
#else
  return (simde__m64) { .i32 = { a.i32[0], b.i32[0] } };
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_punpckldq (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_punpckldq(a.n, b.n));
#else
  return simde_mm_unpacklo_pi32(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_mm_xor_si64 (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_mm_xor_si64(a.n, b.n));
#else
  return (simde__m64) { .i64 = { a.i64[0] ^ b.i64[0] } };
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
simde__m64
simde_m_pxor (simde__m64 a, simde__m64 b) {
#if defined(SIMDE_MMX_NATIVE)
  return SIMDE__M64_C(_m_pxor(a.n, b.n));
#else
  return simde_mm_xor_si64(a, b);
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
int32_t
simde_m_to_int (simde__m64 a) {
#if defined(SIMDE_MMX_NATIVE)
  return _m_to_int(a.n);
#else
  return a.i32[0];
#endif
}

SIMDE__FUNCTION_ATTRIBUTES
int64_t
simde_m_to_int64 (simde__m64 a) {
#if defined(SIMDE_MMX_NATIVE)
  return simde_mm_cvtm64_si64(a);
#else
  return a.i64[0];
#endif
}

#endif /* !defined(SIMDE__MMX_H) */
