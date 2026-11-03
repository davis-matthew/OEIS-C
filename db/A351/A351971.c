/*
 * A351971  a(0) = 1; a(n) = Sum_{k=0..floor(n/4)} binomial(n,4*k) * a(k).
 * https://oeis.org/A351971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} binomial(n,4*k) * a(k)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 60, u128 n <= 116,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 975 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
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

/* Return the nth term of OEIS sequence A351971 with indexing starting at 0. */
seq_t a(idx_t n)
{
    /* Cache the already computed values. */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;   /* index of the last value stored in cache */
    static size_t cap = 0;       /* allocated size of cache array */

    /* Grow the cache if necessary. */
    size_t prev_cap = cap;
    if ((idx_t)(n + 1) > cap) {
        ensure_capacity((size_t)(n + 1), &cap, OEISC_VPP(&cache), sizeof(seq_t));
    }
    /* Initialise the first element on the first allocation. */
    if (prev_cap == 0) {
        cache[0] = 1;
    }

    /* If the value has already been computed, return it. */
    if (n <= cached_n) {
        return cache[n];
    }

    /* Compute all missing terms up to n. */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        seq_t sum = 0;
        idx_t maxk = i / 4;
        for (idx_t k = 0; k <= maxk; ++k) {
            sum += binomial(i, 4 * k) * cache[k];
        }
        cache[i] = sum;
    }

    /* Update the cached range and return the requested value. */
    cached_n = n;
    return cache[n];
}
