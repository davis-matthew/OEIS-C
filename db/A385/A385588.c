/*
 * A385588  Number of non-derangements of length n with 2 excedances.
 * https://oeis.org/A385588
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 - 2^(n+1) + 3^n + n^2/2 + 5*n/2 - n*2^n The fractional
 *            terms are combined as (n^2 + 5*n)/2, which is always an even
 *            integer for n ≥ 0, so the division is exact. The implementation
 *            uses the provided pow2 and pow3 utilities for efficient
 *            exponentiation
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* For safety, return 0 for indices below the offset */
    if (n < 3) return 0;

    /* Compute the powers of two and three */
    seq_t pow2_n      = pow2(n);          /* 2^n   */
    seq_t pow2_np1    = pow2(n + 1);      /* 2^(n+1) */
    seq_t pow3_n      = pow3(n);          /* 3^n   */

    /* Compute the combined fractional term (n^2 + 5*n)/2 */
    seq_t sn   = (seq_t)n;
    seq_t numerator   = sn * sn + (seq_t)5 * sn;   /* n^2 + 5*n ; always even */
    seq_t frac_term   = numerator / 2;             /* exact division */

    /* Assemble the result */
    seq_t res = (seq_t)1;                 /* start with 1 */
    res -= pow2_np1;                     /* subtract 2^(n+1) */
    res += pow3_n;                       /* add 3^n */
    res += frac_term;                    /* add (n^2 + 5*n)/2 */
    res -= sn * pow2_n;                  /* subtract n*2^n */

    return res;
}
