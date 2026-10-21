/*
 * A289433  Numbers c_n of Fibonacci-quilt legal decompositions of n.
 * https://oeis.org/A289433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies a(n) = a(n-1) + a(n-5) + a(n-7) for n ≥ 8 with
 *            initial values a(1)=1, a(2)=1, a(3)=1, a(4)=2, a(5)=2, a(6)=3,
 *            a(7)=4
 * Range:     u8 n <= 19, u16 n <= 35, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* one‑based cache, cache[0] unused */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;  /* number of allocated entries (≥0) */

    if (n == 0)
        return (seq_t)0; /* undefined for offset 0, return 0 */

    /* Expand cache if needed */
    size_t required = (size_t)n + 1;   /* indices 0 … n inclusive */
    if (required > cache_cap) {
        /* Allocate or reallocate */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * required);
        if (tmp == NULL)            /* allocation failed – safe fallback */
            return (seq_t)0;
        cache = tmp;

        /* Compute new entries sequentially */
        for (size_t i = cache_cap; i < required; ++i) {
            if (i == 0) {
                /* dummy entry */
                cache[i] = (seq_t)0;
            } else if (i <= 7) {
                /* initial values, 1‑based */
                switch (i) {
                    case 1: cache[i] = (seq_t)1; break;
                    case 2: cache[i] = (seq_t)1; break;
                    case 3: cache[i] = (seq_t)1; break;
                    case 4: cache[i] = (seq_t)2; break;
                    case 5: cache[i] = (seq_t)2; break;
                    case 6: cache[i] = (seq_t)3; break;
                    case 7: cache[i] = (seq_t)4; break;
                    default: /* never reached */ cache[i] = (seq_t)0; break;
                }
            } else {
                /* recurrence a(i)=a(i-1)+a(i-5)+a(i-7) */
                cache[i] = cache[i-1] + cache[i-5] + cache[i-7];
            }
        }
        cache_cap = required;
    }

    return cache[(size_t)n];
}
