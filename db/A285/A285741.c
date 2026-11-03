/*
 * A285741  a(0) = 1; a(2*n) = a(n), a(2*n+1) = a(n) + R(a(n)), where R() is
 *          the digit reversal.
 * https://oeis.org/A285741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1; a(2*n) = a(n); a(2*n+1) = a(n) + reverse_decimal(a(n))
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 90 (2026-11-03)
 * Helpers:   mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/reverse_digits.h"
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
static size_t cache_cap = 0;

/* Recursive helper that assumes cache is large enough for the requested index */
static seq_t rec(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                   /* the base value is never cached */

    /* use cached value if available */
    if (n < cache_cap && cache[n] != (seq_t)0)
        return cache[n];

    idx_t k = n >> 1;          /* floor(n/2) */
    seq_t val = rec(k);        /* a(k) */

    if (n & 1) {               /* odd index: add reverse of a(k) */
        seq_t rev = reverse_digits(val);
        val = val + rev;
    }

    /* store computed value */
    cache[n] = val;
    return val;
}

/* public entry point */
seq_t a(idx_t n)
{
    /* ensure cache big enough for indices up to n */
    if (!cache) {
        ensure_capacity(n + 1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        for (size_t i = 0; i < cache_cap; ++i)
            cache[i] = (seq_t)0;
    } else if (cache_cap <= n) {
        size_t old_cap = cache_cap;
        ensure_capacity(n + 1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
        for (size_t i = old_cap; i < cache_cap; ++i)
            cache[i] = (seq_t)0;
    }

    return rec(n);
}
