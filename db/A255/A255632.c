/*
 * A255632  Number of n-length words on {0,1,2,3,4} avoiding runs of zeros of
 *          length 1 (mod 3).
 * https://oeis.org/A255632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   mul_uint128_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n+3) = 4*a(n+2) + 5*a(n)   (n > 0)
 * Initial values: a(0)=1, a(1)=4, a(2)=17
 */

#include <stdlib.h>
#include "../utils/mul_uint128_seq.h"

#if defined(__SIZEOF_INT128__)

#else
/* If 128‑bit integers are not available, fall back to simple multiplication; */

#endif

/* Sequence A255632 */
seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t   size  = 0;   /* number of entries currently computed */
    static size_t  cap   = 0;   /* allocated size of the array */

    /* extend cache if needed */
    if (n >= size) {
        size_t new_cap = cap ? cap * 2 : 4;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;

        /* compute new terms */
        for (idx_t i = size; i <= n; ++i) {
            if (i == 0)
                cache[i] = 1;
            else if (i == 1)
                cache[i] = 4;
            else if (i == 2)
                cache[i] = 17;
            else {
                seq_t term4 = mul_uint128_seq(cache[i - 1], 4);
                seq_t term5 = mul_uint128_seq(cache[i - 3], 5);
                cache[i] = term4 + term5;
            }
        }
        size = n + 1;
    }

    return cache[n];
}
