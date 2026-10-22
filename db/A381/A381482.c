/*
 * A381482  a(n) = Sum_{k=0..floor(n/2)} binomial(n,k)^2 * binomial(n-k,k) *
 *          2^k.
 * https://oeis.org/A381482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n,k)^2 * binomial(n-k,k) *
 *            2^k
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t pow2  = 1;                     /* 2^k */
    idx_t limit = n / 2;                  /* floor(n/2) */

    for (idx_t k = 0; k <= limit; ++k) {
        seq_t bk     = binomial(n, k);            /* C(n,k)           */
        seq_t bk_sq  = bk * bk;                  /* C(n,k)^2          */
        seq_t c      = binomial(n - k, k);       /* C(n-k,k)          */
        seq_t term   = bk_sq * c;                /* C(n,k)^2 * C(n-k,k) */
        term = term * pow2;                      /* multiply by 2^k    */
        sum += term;

        pow2 <<= 1;                              /* advance 2^k (=> 2^(k+1)) */
    }

    return sum;
}
