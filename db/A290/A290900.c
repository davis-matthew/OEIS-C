/*
 * A290900  p-INVERT of the positive integers, where p(S) = 1 - S^2 - S^3.
 * https://oeis.org/A290900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 14*a(n-2) + 19*a(n-3) - 14*a(n-4) + 6*a(n-5) -
 *            a(n-6); Initial values: a(0)=0, a(1)=1, a(2)=5, a(3)=17,
 *            a(4)=51, a(5)=149
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial terms */
    static const seq_t init[6] = {0, 1, 5, 17, 51, 149};

    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t len = 0;   /* number of entries already stored (>=0) */
    static size_t cap = 0;   /* allocated capacity (>=0) */

    /* Directly return small index values */
    if (n < 6)
        return init[(size_t)n];

    /* Allocate cache on first use */
    if (!cache) {
        /* Allocate an initial block at least large enough for the first
           few terms; use a power‑of‑two to keep growth fast. */
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            return 0;          /* out of memory; return 0 as safe fallback */

        /* Fill cache with the six initial terms */
        for (size_t i = 0; i < 6; ++i)
            cache[i] = init[i];
        len = 6;
    }

    /* Grow the cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap * 2;
        while (newcap < (size_t)n + 1)
            newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            return 0;          /* allocation failure */
        cache = tmp;
        cap = newcap;
    }

    /* Compute the missing values up to n */
    if ((size_t)n + 1 > len) {
        for (size_t i = len; i <= (size_t)n; ++i) {
            seq_t val = (seq_t)6 * cache[i-1]
                      - (seq_t)14 * cache[i-2]
                      + (seq_t)19 * cache[i-3]
                      - (seq_t)14 * cache[i-4]
                      + (seq_t)6  * cache[i-5]
                      -          cache[i-6];
            cache[i] = val;
        }
        len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
