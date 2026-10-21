/*
 * A254796  Denominators of the convergents of the generalized continued
 *          fraction 2 + 1^2/(4 + 3^2/(4 + 5^2/(4 + ... ))).
 * https://oeis.org/A254796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 4*a(n-1) + (2*n-1)^2*a(n-2) with a(0)=1, a(1)=4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The cache holds the already-computed terms and grows
   as more indices are requested. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <algorithm>
#endif

/* Static cached values */
static seq_t *a_cache      = NULL;
static size_t a_cache_cap  = 0;
static idx_t a_cache_max   = 0;

/* Reallocate the cache to hold at least "needed" elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= a_cache_cap) return;

    size_t new_cap = a_cache_cap ? a_cache_cap << 1 : 2;
    while (new_cap < needed) new_cap <<= 1;

#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    if (a_cache) {
        std::copy(a_cache, a_cache + a_cache_cap, tmp);
        delete[] a_cache;
    }
    a_cache = tmp;
#else
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    a_cache = tmp;
#endif
    a_cache_cap = new_cap;
}

/* Return the n-th term of A254796. */
seq_t a(idx_t n)
{
    /* initialise base values on first call */
    if (!a_cache) {
        ensure_capacity(2);
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)4;
        a_cache_max = 1;
    }

    if (n <= a_cache_max) return a_cache[n];

    ensure_capacity((size_t)n + 1);

    for (idx_t i = a_cache_max + 1; i <= n; ++i) {
        seq_t prev     = a_cache[i - 1];
        seq_t prevprev = a_cache[i - 2];
        seq_t k = (seq_t)(2 * i - 1);
        seq_t term = (seq_t)4 * prev + k * k * prevprev;
        a_cache[i] = term;
    }
    a_cache_max = n;
    return a_cache[n];
}
