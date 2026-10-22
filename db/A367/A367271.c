/*
 * A367271  a(n) = binomial(2*n, n) * n^n.
 * https://oeis.org/A367271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n, n) * n^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the provided binom and big_pow utilities */
#include "../utils/binom.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* 0^0 is defined as 1 in this sequence */
    if (n == 0)
        return (seq_t)1;

    seq_t bin = binom(2 * n, n);          /* central binomial coefficient */
    seq_t pow = big_pow((seq_t)n, (seq_t)n); /* n^n */
    return bin * pow;
}
