/*
 * A373339  Number of permutations in symmetric group S_n with an even number
 *          of cycles of length 2 or more.
 * https://oeis.org/A373339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!/2 - (n-2)*2^(n-2) for n >= 2 a(0) = a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 1U)
        return (seq_t)1;

    /* n! divided by 2 (n! is even for n >= 2) */
    seq_t fact  = factorial(n);
    seq_t half  = fact / ((seq_t)2);

    /* subtract (n-2)*2^(n-2) */
    seq_t pow_two = pow2(n - 2);
    seq_t sub     = pow_two * (seq_t)(n - 2);

    return half - sub;
}
