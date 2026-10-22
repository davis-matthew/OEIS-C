/*
 * A373395  Number of minimum connected dominating sets in the n-triangular
 *          graph.
 * https://oeis.org/A373395
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)^(n-3) for n>2; a(2) = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 2)          /* undefined for n < 2, return 0 */
        return (seq_t)0;
    if (n == 2)         /* special case */
        return (seq_t)1;

    /* Compute (n-1)^(n-3) using big_pow, then multiply by n */
    seq_t base = (seq_t)(n - 1);
    seq_t exp  = (seq_t)(n - 3);
    seq_t power = big_pow(base, exp);
    return (seq_t)n * power;
}
