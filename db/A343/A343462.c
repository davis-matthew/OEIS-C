/*
 * A343462  Number of n-digit positive integers that undulate.
 * https://oeis.org/A343462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 5*a(n-1) + 10*a(n-2) - 20*a(n-3) - 15*a(n-4) + 21*a(n-5)
 *            + 7*a(n-6) - 8*a(n-7) - a(n-8) + a(n-9) with initial values:
 *            a(1)=9, a(2)=81, a(3)=525, a(4)=3105, a(5)=18939, a(6)=114381,
 *            a(7)=693129, a(8)=4195557, a(9)=25405586, a(10)=153820395
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;            /* undefined for n=0 */

    static seq_t *cache = NULL;      /* 1‑based cache */
    static size_t cache_cap = 0;
    static idx_t cached_up_to = 0;

    if ((idx_t)cache_cap <= n) {
        size_t new_cap = (size_t)(n + 1);   /* accommodate indices 0..n */
        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache,
                                          new_cap * sizeof(seq_t));
            if (!tmp) return 0;                /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }
    }

    /* Base values up to index 10 */
    static const seq_t base[11] = {
        (seq_t)0,      (seq_t)9,     (seq_t)81,
        (seq_t)525,    (seq_t)3105,  (seq_t)18939,
        (seq_t)114381, (seq_t)693129, (seq_t)4195557,
        (seq_t)25405586, (seq_t)153820395
    };

    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        if (i <= 10) {
            cache[(size_t)i] = base[(size_t)i];
        } else {
            cache[(size_t)i] =
                5 * cache[(size_t)(i-1)] + 10 * cache[(size_t)(i-2)]
              - 20 * cache[(size_t)(i-3)] - 15 * cache[(size_t)(i-4)]
              + 21 * cache[(size_t)(i-5)] + 7 * cache[(size_t)(i-6)]
              - 8  * cache[(size_t)(i-7)] - cache[(size_t)(i-8)]
              + cache[(size_t)(i-9)];
        }
    }
    cached_up_to = n;
    return cache[(size_t)n];
}
