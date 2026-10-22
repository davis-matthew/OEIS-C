/*
 * A381681  a(n) is one of two integer components (with A000254) used in
 *          computing the inverse second moment of X+n, where X~Poisson(1).
 * https://oeis.org/A381681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{i=1..n} (1/i)*Sum_{j=0..i-1} (-1)^j/j!.
 *            Recurrence used: T_1 = 1 T_{k+1} = k*T_k + (-1)^k (k ≥ 1) a(0) =
 *            0 a(n) = n * a(n-1) + T_n (n ≥ 1) T_k is an auxiliary integer
 *            sequence. The function memoises computed values so that any call
 *            order works
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed values */
    static seq_t *a_cache = NULL;
    static seq_t *T_cache = NULL;   /* T_cache[0] unused, T_cache[1] = 1 */
    static size_t cache_size = 0;    /* number of entries stored (>=1) */
    static size_t cache_cap  = 0;    /* allocated capacity */

    /* Initialise caches on first call */
    if (cache_size == 0) {
        cache_cap = 16;                            /* initial capacity */
        a_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        T_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        a_cache[0] = 0;                            /* a(0) = 0 */
        cache_size = 1;                            /* indices stored: 0 */
    }

    /* Extend cache if required */
    if ((idx_t)cache_size <= n) {
        /* Allocate more space */
        size_t needed = n + 1;
        if (cache_cap < needed) {
            size_t new_cap = cache_cap;
            while (new_cap < needed) new_cap <<= 1;
            a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
            T_cache = (seq_t *)realloc(T_cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* Compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t T_i;                      /* T_i = T_{i} */

            if (i == 1) {
                T_i = 1;                    /* base case */
            } else {
                seq_t prod = (seq_t)(i - 1) * T_cache[i - 1];
                if (((i - 1) & 1) == 1) {  /* k = i-1 is odd: subtract 1 */
                    T_i = prod - 1;
                } else {                   /* even: add 1 */
                    T_i = prod + 1;
                }
            }

            seq_t a_i = (seq_t)i * a_cache[i - 1] + T_i;   /* a(i) */

            a_cache[i] = a_i;
            if (i >= 1) T_cache[i] = T_i;
        }
        cache_size = n + 1;
    }

    return a_cache[n];
}
