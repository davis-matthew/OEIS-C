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
 * Helpers:   divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sigma(n) is the sum of all divisors of n
   and tau(n) is the number of divisors of n. */
#include "../utils/divisor_count_u64.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* number of divisors of n */
    idx_t tau = divisor_count_u64(n);

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
