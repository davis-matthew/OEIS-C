/*
 * A344464  a(n) = Sum_{d|n} d^p(d), where p is the number of partitions of n.
 * https://oeis.org/A344464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^p(d), where p(d) = number of partitions of d
 *            Using a simple dynamic programming to compute partition numbers
 *            up to n, and the helper big_pow for exponentiation of
 *            arbitrary-precision integers
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    /* Static cache for partition numbers. */
    static seq_t *part = NULL;
    static idx_t part_max = 0;

    /* Increase the cache and recompute partitions if necessary. */
    if (n > part_max) {
        /* Allocate or extend the array. */
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(part, new_size * sizeof(seq_t));
        if (!tmp) {          /* allocation failure – return 0 */
            return (seq_t)0;
        }
        part = tmp;

        /* Initialize all entries to 0. */
        for (idx_t i = 0; i <= n; ++i) {
            part[i] = 0;
        }

        /* Compute partition numbers up to n using the coin-change DP. */
        part[0] = 1;   /* p(0) = 1 */
        for (idx_t coin = 1; coin <= n; ++coin) {
            for (idx_t sum = coin; sum <= n; ++sum) {
                part[sum] += part[sum - coin];
            }
        }

        part_max = n;
    }

    /* Sum of d^p(d) over all divisors d of n. */
    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            seq_t term = big_pow((seq_t)d, part[d]);
            result += term;
            if (d * d != n) {
                idx_t e = n / d;
                seq_t termE = big_pow((seq_t)e, part[e]);
                result += termE;
            }
        }
    }

    return result;
}
