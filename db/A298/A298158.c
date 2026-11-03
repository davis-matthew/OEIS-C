/*
 * A298158  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = -1,
 *          a(1) = -2, a(2) = 1, a(3) = 1.
 * https://oeis.org/A298158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4) Initial values:
 *            a(0) = -1, a(1) = -2, a(2) = 1, a(3) = 1
 * Range:     i8 n <= 7, i16 n <= 18, i32 n <= 41, i64 n <= 87, i128 n <= 179,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 1985 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
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

static seq_t *cache   = NULL;
static size_t cache_cap = 0;
static bool   initialized = false;
static idx_t  last_computed = 3;

seq_t a(idx_t n)
{
    if (!initialized) {
        ensure_capacity(4, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        cache[0] = (seq_t)-1;
        cache[1] = (seq_t)-2;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        initialized = true;
    }

    if (n < 4)
        return cache[(size_t)n];

    if (cache_cap <= (size_t)n)
        ensure_capacity((size_t)n + 1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));

    for (size_t i = (size_t)last_computed + 1; i <= (size_t)n; ++i) {
        cache[i] = cache[i - 1]
                 + (seq_t)3 * cache[i - 2]
                 - (seq_t)2 * cache[i - 3]
                 - (seq_t)2 * cache[i - 4];
    }
    last_computed = n;
    return cache[(size_t)n];
}
