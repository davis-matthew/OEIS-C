/*
 * A265132  Number of nonintersecting (or self-avoiding) rook paths joining
 *          opposite corners of an n X n grid that move in 3 or fewer cardinal
 *          directions.
 * https://oeis.org/A265132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = 2*n^(n-1) - C(2*n-2, n-1) (n ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* n is at least 1 */
    seq_t base     = (seq_t)n;                 /* n         */
    seq_t exp      = (seq_t)(n - 1u);          /* n-1       */
    seq_t power    = big_pow(base, exp);      /* n^(n-1)   */
    seq_t two_power = seq_mul(power, (seq_t)2u); /* 2*n^(n-1) */
    idx_t m        = n - 1u;                   /* m = n-1   */
    seq_t binom    = binomial(2u * m, m);      /* C(2m, m)  */
    return two_power - binom;                  /* result    */
}
