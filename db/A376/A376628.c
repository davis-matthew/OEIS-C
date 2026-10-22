/*
 * A376628  G.f.: Sum_{k>=0} x^(k*(k+1)) / Product_{j=1..k} (1 - x^(2*j-1)).
 * https://oeis.org/A376628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in the generating function Sum_{k>=0}
 *            x^{k*(k+1)} / Product_{j=1..k} (1 - x^{2*j-1})
 * Range:     u8 n <= 52, u16 n <= 142, u32 n <= 442, u64 n <= 1504,
 *            u128 n <= 5458, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate an array to hold the number of partitions of each
       value using odd parts up to the current upper bound. */
    size_t N = (size_t)n;
    seq_t *ways = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!ways) return 0;          /* allocation failure – return 0 */

    ways[0] = 1;                  /* one way to partition 0 */

    seq_t result = 0;

    for (idx_t k = 0; ; ++k)
    {
        idx_t shift = k * (k + 1);          /* k(k+1) */
        if (shift > n) break;

        /* Add the contribution of the current k to the answer. */
        result += ways[n - shift];

        /* Prepare the array for the next value of k by adding the
           next odd part (2k+1).  This updates the counts of partitions
           for all numbers up to n using odd parts <= 2k+1. */
        idx_t coin = 2 * k + 1;
        for (idx_t m = coin; m <= n; ++m)
        {
            ways[m] += ways[m - coin];
        }
    }

    free(ways);
    return result;
}
