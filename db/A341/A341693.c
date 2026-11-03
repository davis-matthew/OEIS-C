/*
 * A341693  Numbers k whose sum of digits divides sigma(k)-k.
 * https://oeis.org/A341693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   digit_sum, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A341693: numbers k whose sum of digits divides sigma(k)-k.
   a(n) = the nth such k (indexed from 1). */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
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

seq_t a(idx_t n) {
    /* Static cache of sequence values. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;    /* number of terms stored */
    static size_t cache_cap = 0;    /* allocated capacity */
    static idx_t next_k = 1;        /* next integer to test */

    /* Grow cache until we have at least n terms. */
    while (cache_len < n) {
        seq_t sigma = sigma_of(next_k);
        seq_t dsum = digit_sum((seq_t)next_k);
        /* sigma - k is always >=0 for k>=1 */
        seq_t diff = sigma - (seq_t)next_k;
        if (diff % dsum == 0) {
            /* add to cache */
            if (cache_len >= cache_cap) {
                ensure_capacity(cache_len + 1, &cache_cap,
                                OEISC_VPP(&cache), sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;  /* advance to next integer */
    }

    return cache[n - 1];
}
