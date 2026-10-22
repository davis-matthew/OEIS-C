/*
 * A362681  The number of steps, starting from n, to reach k<=2 in an
 *          iteration k -> 2*k - (sum of proper factors of 2*k).
 * https://oeis.org/A362681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of steps to reach k <= 2 in the iteration k ← 2*k
 *            − sum_of_proper_factors(2*k) (proper factors exclude 1 and the
 *            number itself) initial terms: a(1)=0, a(2)=0
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_proper_factors_from_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/sum_proper_factors_from_idx.h"

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;

    seq_t k = (seq_t)n;
    seq_t steps = 0;

    while (k > 2)
    {
        /* compute step: new_k = 2*k - sum_of_proper_factors(2*k) */
        seq_t two_k = (seq_t)2 * k;
        seq_t sum = sum_proper_factors_from_idx((idx_t)two_k);

        if (two_k <= sum)
            k = 0;                /* new_k <= 2, terminate in next loop test */
        else
            k = two_k - sum;

        ++steps;
    }
    return steps;
}
