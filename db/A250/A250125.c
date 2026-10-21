/*
 * A250125  Coordination sequence of point of type 3.4.3.12 in 4-uniform
 *          tiling {3.3.4.3.4; 3.3.4.12; 3.3.12.4; 3.4.3.12}.
 * https://oeis.org/A250125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-5) + a(n-7) - a(n-12) + c(n),
   where c(n) = 1,4,6,11,13,14,19,16,18,11,7,6,4,2,-1,-1,-1,-1 for n=0..17,
   and c(n)=0 for n≥18. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* coefficients of the numerator part of the generating function */
    static const int NUM[18] =
        {1, 4, 6, 11, 13, 14, 19, 16, 18, 11, 7, 6, 4, 2, -1, -1, -1, -1};

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements currently stored */

    if (n >= cache_size) {
        size_t new_cap = cache_size > 0 ? cache_size : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);  /* allocation failure */
        cache = tmp;

        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val = 0;

            if (i >= 5)  val += cache[i - 5];
            if (i >= 7)  val += cache[i - 7];
            if (i >= 12) val -= cache[i - 12];

            if (i <= 17) {
                int coeff = NUM[i];
                if (coeff >= 0)
                    val += (seq_t)coeff;
                else
                    val -= (seq_t)(-coeff);
            }

            cache[i] = val;
        }
        cache_size = (size_t)n + 1;
    }
    return cache[n];
}
