/*
 * A246916  Sum of the cumulative sums of all the permutations of divisors of
 *          number n.
 * https://oeis.org/A246916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) * (tau(n)+1)! / 2
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 59, u64 n <= 179, u128 n <= 839,
 *            big any n
 * Validated: standard, pass, 60 terms through n = 8131 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sigma(n) is the sum of all divisors of n
   and tau(n) is the number of divisors of n. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/num_divisors.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* number of divisors of n */
    idx_t tau = num_divisors(n);

    /* sum of divisors of n */
    seq_t sigma = sigma_of(n);

    /* (tau+1)! / 2  – compute as product of numbers 1..tau+1 excluding the factor 2 */
    seq_t half = 1;
    for (idx_t i = 1; i <= tau + 1; ++i)
    {
        if (i != 2)
            half *= (seq_t)i;
    }

    /* final result */
    return sigma * half;
}
