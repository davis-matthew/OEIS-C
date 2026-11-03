/*
 * A385530  E.g.f. A(x) satisfies A(x) = exp(x*A(-4*x)).
 * https://oeis.org/A385530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 7, i128 n <= 10,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 12 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1
   a(n)=∑_{k=0}^{n-1} (k+1)·(-4)^k·binomial(n-1,k)·a(k)·a(n-1-k)  for n>0
*/
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

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

static seq_t *cache = NULL;
static size_t cache_len   = 0;
static size_t cache_cap   = 0;
static seq_t *pow_neg4 = NULL;
static size_t pow_cap   = 0;

seq_t a(idx_t n)
{
    /* initialise caches on first call */
    if (cache_len == 0)
    {
        ensure_capacity(1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_len = 1;

        ensure_capacity(1, &pow_cap, OEISC_VPP(&pow_neg4), sizeof(seq_t));
        pow_neg4[0] = (seq_t)1;
        pow_cap = 1;
    }

    /* extend caches to hold all indices up to n */
    if ((size_t)n + 1 > cache_cap)
        ensure_capacity((size_t)n + 1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
    if ((size_t)n + 1 > pow_cap)
        ensure_capacity((size_t)n + 1, &pow_cap, OEISC_VPP(&pow_neg4), sizeof(seq_t));

    /* compute terms up to n (if not already computed) */
    for (size_t cur = cache_len; cur <= (size_t)n; ++cur)
    {
        seq_t sum = 0;
        for (size_t k = 0; k < cur; ++k)
        {
            /* term = (k+1) * pow_neg4[k] * binomial(cur-1,k) * cache[k] * cache[cur-1-k] */
            seq_t term = (seq_t)(k + 1);
            term *= pow_neg4[k];
            term *= binomial((idx_t)cur - 1, (idx_t)k);
            term *= cache[k];
            term *= cache[cur - 1 - k];
            sum += term;
        }
        cache[cur] = sum;
        pow_neg4[cur] = pow_neg4[cur - 1] * (seq_t)(-4);
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
