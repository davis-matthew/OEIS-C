/*
 * A391705  Number of plane partitions of n with 6 parts.
 * https://oeis.org/A391705
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 37, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 47 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the rational generating function
   A(q) = (q^17 + q^15 + 6q^14 + 6q^13 + 8q^12 + 8q^11 + 14q^10
           + 12q^9 + 9q^8 + 11q^6)
          / ((1 - q)(1 - q^2)(1 - q^3)(1 - q^4)(1 - q^5)(1 - q^6))

   Let D(q)=∏_{k=1}^6 (1 - q^k)=∑_{i=0}^{21} d[i] q^i
   and N(q)=∑_{i=0}^{21} c[i] q^i  (c[6]=11, c[8]=9, …).
   For n ≥ 0:
       a(n) = c[n] - Σ_{i=1}^{21} d[i] * a(n-i)      (d[0] = 1)
   The series is generated iteratively with a cache so that
   subsequent calls are answered in O(1) after the first
   computation.

   The sequence starts at n = 6:  a(6)=11, a(7)=11, a(8)=31, …
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

seq_t a(idx_t n)
{
    static size_t cache_len   = 0;   /* number of terms already computed   */
    static size_t cache_cap   = 0;   /* allocated capacity of cache array   */
    static seq_t *cache       = NULL;/* array holding computed terms       */
    static bool   initialized = false;
    static int64_t d[22];           /* coefficients of (1-q)(1-q²)…(1-q⁶) */
    static int64_t c[22] = {0};     /* coefficients of the numerator      */

    /* One‑time setup of the generating function coefficients */
    if (!initialized) {
        /* build D(q) = ∏(1-q^k) */
        for (int i = 0; i < 22; ++i) d[i] = 0;
        d[0] = 1;
        int deg = 0;
        for (int k = 1; k <= 6; ++k) {
            for (int i = deg; i >= 0; --i) {
                d[i + k] -= d[i];
            }
            deg += k;
        }

        /* numerator coefficients */
        c[6]  = 11;
        c[8]  = 9;
        c[9]  = 12;
        c[10] = 14;
        c[11] = 8;
        c[12] = 8;
        c[13] = 6;
        c[14] = 6;
        c[15] = 1;
        c[17] = 1;

        initialized = true;
    }

    /* Ensure we have space for n+1 terms */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        /* double the capacity to keep realloc inexpensive */
        if (new_cap < 64) new_cap = 64;
        else new_cap = new_cap * 2;
        ensure_capacity(new_cap, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
    }

    /* Compute all missing terms up to index n */
    while (cache_len <= (size_t)n) {
        seq_t term = 0;
        seq_t plus  = 0;
        seq_t minus = 0;
        size_t idx = cache_len;

        /* Sum contributions from the denominator coefficients */
        for (size_t i = 1; i <= 21 && i <= idx; ++i) {
            int64_t coeff = d[i];
            if (coeff > 0) {
                plus += mul_small(cache[idx - i], (unsigned)coeff);
            } else if (coeff < 0) {
                minus += mul_small(cache[idx - i], (unsigned)(-coeff));
            }
        }

        /* N(q) coefficient for this index, if within range */
        if (idx < 22) {
            term = (seq_t)c[idx];
        }

        /* Apply the recurrence: a(n) = c[n] - Σ d[i]*a(n-i) */
        term += minus;
        if (plus != (seq_t)0) term -= plus;

        cache[cache_len] = term;
        ++cache_len;
    }

    return cache[n];
}
