/*
 * A360045  a(n) = Sum_{k=0..floor(n/3)} binomial(n+2,3*k+2) * Catalan(k).
 * https://oeis.org/A360045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n+2,2) + Σ_{k=0}^{n-3} a(k)*a(n-k-3) (OEIS
 *            A360045, offset 0)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"   /* binomial(idx_t a, idx_t b) -> seq_t */

/* Static cache for memoisation */
static seq_t *cache = NULL;   /* array of computed values */
static idx_t cap   = 0;       /* current capacity */
static idx_t num   = 0;       /* next index to be computed */

/* Ensure the cache can store indices up to 'needed' (inclusive).
   Works in both C and C++. */
static void ensure_capacity(idx_t needed)
{
    if (cap > needed)
        return;
    size_t new_cap = cap ? (size_t)cap << 1 : 8;
    while (new_cap <= (size_t)needed)
        new_cap <<= 1;
    /* realloc returns void* in C, void* in C++. Cast explicitly. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();      /* allocation failure */
    cache = tmp;
    cap   = (idx_t)new_cap;
}

/* Sequence function */
seq_t a(idx_t n)
{
    /* Initialise cache lazily */
    if (!cache)
        ensure_capacity(n);

    /* Grow cache if needed while computing up to n */
    for (; num <= n; ++num) {
        if (num >= cap)
            ensure_capacity(num);
        if (num < 3) {
            /* binomial(num+2, 2) for n = 0,1,2 */
            cache[num] = binomial((idx_t)(num + 2), (idx_t)2);
        } else {
            seq_t val = binomial((idx_t)(num + 2), (idx_t)2);
            for (idx_t k = 0; k <= num - 3; ++k)
                val += cache[k] * cache[num - k - 3];
            cache[num] = val;
        }
    }

    return cache[n];
}
