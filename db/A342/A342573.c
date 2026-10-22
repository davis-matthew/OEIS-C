/*
 * A342573  The number of ordered n-tuples consisting of n permutations (not
 *          necessarily distinct) such that the first element of each of them
 *          is the same.
 * https://oeis.org/A342573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * ((n-1)!)^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined below offset, safe placeholder */

    /* Compute (n-1)! */
    seq_t fact = factorial(n - 1);

    /* Compute ((n-1)!) ^ n */
    seq_t pow_val = big_pow(fact, (seq_t)n);

    /* Multiply by n and return */
    return (seq_t)n * pow_val;
}
