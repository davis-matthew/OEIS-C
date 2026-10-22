/*
 * A374454  Expansion of o.g.f. 1/(1 - 4*x - 6*x^2 - 4*x^3 - x^4).
 * https://oeis.org/A374454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, a(2)=22, a(3)=116,
 *            a(n)=4*a(n-1)+6*a(n-2)+4*a(n-3)+a(n-4) for n>=4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Compute the nth term of OEIS A374454.  The function is memoized
 * and works for arbitrary build‐dependent integer type seq_t
 * (uint8_t, uint16_t, uint32_t, uint64_t, unsigned __int128,
 * or arbitrary-precision integer).  It can be called in any order. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t max_computed = (idx_t)-1;

    /* First call: allocate cache and initialise base values. */
    if (cache == NULL) {
        size_t init_cap = 4;                    /* store a(0)..a(3) */
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) abort();             /* out‑of‑memory */
        cache_size = init_cap;

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)22;
        cache[3] = (seq_t)116;
        max_computed = 3;
    }

    /* If the requested index is already known, return it. */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Grow the cache if necessary. */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_cap = cache_size;
        while ((size_t)(n + 1) > new_cap) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_size = new_cap;
    }

    /* Compute the missing terms using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] * ((seq_t)4) +
                   cache[i - 2] * ((seq_t)6) +
                   cache[i - 3] * ((seq_t)4) +
                   cache[i - 4];
    }
    max_computed = n;
    return cache[n];
}
