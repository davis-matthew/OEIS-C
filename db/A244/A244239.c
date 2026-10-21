/*
 * A244239  Number of partitions of n into 3 parts such that every i-th
 *          smallest part (counted with multiplicity) is different from i.
 * https://oeis.org/A244239
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 889, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A244239 (Number of partitions of n into 3 parts such that the i‑th
    smallest part differs from i).  
    Recurrence (for n > 15):
        a(n) = a(n-1) + a(n-2) - a(n-4) - a(n-5) + a(n-6)
    Initial values (for 9 ≤ n ≤ 15):
        a(9)=1, a(10)=3, a(11)=4, a(12)=6, a(13)=7, a(14)=9, a(15)=11
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The OEIS offset is 9; values for n < 9 are unused by the harness. */
    if (n < 9) return 0;

    /* Static cache storing all previously computed terms. */
    static seq_t *cache  = NULL;
    static idx_t cap    = 0;   /* current size of allocated array */
    static idx_t max_n  = 0;   /* largest computed index */

    /* Expand the cache if it is not large enough. */
    if (cap <= n) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure → abort */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n. */
    if (max_n < n) {
        /* Initial values for 9 ≤ i ≤ 15. */
        static const seq_t init[7] = {
            (seq_t)1, (seq_t)3, (seq_t)4,
            (seq_t)6, (seq_t)7, (seq_t)9, (seq_t)11
        };

        /* Starting index for computation. */
        idx_t i = (max_n + 1 > 9) ? (max_n + 1) : 9;

        for (; i <= n; ++i) {
            if (i <= 15) {
                cache[i] = init[i - 9];
            } else { /* recurrence for i > 15 */
                cache[i] = cache[i - 1] + cache[i - 2]
                          - cache[i - 4] - cache[i - 5]
                          + cache[i - 6];
            }
        }
        max_n = n;
    }

    return cache[n];
}
