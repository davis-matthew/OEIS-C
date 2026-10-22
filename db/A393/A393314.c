/*
 * A393314  G.f. A(x) satisfies: A(x) = (1 + x + x^2 + x^3)^2 * A(x^4) / (1 -
 *          x).
 * https://oeis.org/A393314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=3, a(2)=6, a(3)=10, a(4)=16, a(5)=24
 *            a(n)=a(n-4)+a(n/4)+2*a((n-1)/4)+3*a((n-2)/4)+4*a((n-3)/4)+
 *            3*a((n-4)/4)+2*a((n-5)/4)+a((n-6)/4) for n>5
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts partitions weighted by powers of 4, see A393314. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoization array */
    static size_t cache_len = 0;  /* number of valid entries in cache */
    static size_t cache_cap = 0;  /* allocated size of cache */

    size_t target = (size_t)n;

    /* Expand cache if necessary */
    if (target + 1 > cache_cap) {
        /* choose new capacity (at least target+1, grow geometrically) */
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap <= target) new_cap <<= 1;

        /* allocate new array */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);   /* abort on allocation failure */

        /* initialise new entries if the expansion added space */
        /* existing entries are preserved by realloc */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute values up to target, if not already done */
    for (size_t i = cache_len; i <= target; ++i) {
        if (i <= 5) {   /* base cases */
            switch (i) {
                case 0: cache[i] = (seq_t)1; break;
                case 1: cache[i] = (seq_t)3; break;
                case 2: cache[i] = (seq_t)6; break;
                case 3: cache[i] = (seq_t)10; break;
                case 4: cache[i] = (seq_t)16; break;
                case 5: cache[i] = (seq_t)24; break;
            }
        } else {
            /* recurrence using previously computed values */
            seq_t val = cache[i - 4];
            val += cache[i / 4];
            val += cache[(i - 1) / 4] * ((seq_t)2);
            val += cache[(i - 2) / 4] * ((seq_t)3);
            val += cache[(i - 3) / 4] * ((seq_t)4);
            val += cache[(i - 4) / 4] * ((seq_t)3);
            val += cache[(i - 5) / 4] * ((seq_t)2);
            val += cache[(i - 6) / 4];
            cache[i] = val;
        }
    }
    cache_len = target + 1;
    return cache[target];
}
