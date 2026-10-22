/*
 * A386918  a(n) = 2^n * binomial(4*n,n).
 * https://oeis.org/A386918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS sequence A386918
 * Formula: a(n) = 2^n * binomial(4*n, n)
 */

#include "../utils/binom.h"  /* static seq_t binom(idx_t a, idx_t b); */
#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    return pow2(n) * binom(4 * n, n);
}
