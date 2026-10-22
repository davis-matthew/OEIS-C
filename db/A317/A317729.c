/*
 * A317729  Number of n X 4 0..1 arrays with every element unequal to 1, 2, 3,
 *          4, 5, 6, 7 or 8 king-move adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A317729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) + 42*a(n-2) + 107*a(n-3) - 30*a(n-4) +
 *            12*a(n-5) - 16*a(n-6), with a(1) = 2, a(2) = 98, a(3) = 1250,
 *            a(4) = 19208, a(5) = 293378, a(6) = 4458098. The recurrence is
 *            used iteratively with memoization
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Memoization cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* number of allocated elements (indices 0..cap-1) */

    if (n == 0)                /* undefined below offset; return 0 safely */
        return (seq_t)0;

    /* Allocate or extend cache if needed */
    if (cap < (size_t)n + 1) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {                 /* allocation failure – abort */
            exit(1);
        }
        cache = tmp;

        /* Initialise new entries */
        for (size_t i = cap; i <= (size_t)n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)2;
            } else if (i == 2) {
                cache[i] = (seq_t)98;
            } else if (i == 3) {
                cache[i] = (seq_t)1250;
            } else if (i == 4) {
                cache[i] = (seq_t)19208;
            } else if (i == 5) {
                cache[i] = (seq_t)293378;
            } else if (i == 6) {
                cache[i] = (seq_t)4458098;
            } else {
                /* recurrence for i >= 7 */
                seq_t pos = seq_mul(cache[i-1], (seq_t)12);
                pos     += seq_mul(cache[i-2], (seq_t)42);
                pos     += seq_mul(cache[i-3], (seq_t)107);
                pos     += seq_mul(cache[i-5], (seq_t)12);

                seq_t neg = seq_mul(cache[i-4], (seq_t)30);
                neg     += seq_mul(cache[i-6], (seq_t)16);

                /* pos >= neg by recurrence definition */
                cache[i] = pos - neg;
            }
        }
        cap = newcap;
    }

    return cache[(size_t)n];
}
