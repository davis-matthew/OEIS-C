/*
 * A388911  a(n) = Sum_{k=0..n} 2^k * binomial(n,k) * binomial(2*n+2*k,k).
 * https://oeis.org/A388911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^k * binomial(n,k) * binomial(2*n+2*k,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t two = 1;                   /* 2^k */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t bk = binomial(n, k);   /* C(n,k)   */

        idx_t m = 2 * n + 2 * k;     /* 2*n + 2*k */
        seq_t tk = binomial(m, k);   /* C(2n+2k,k) */

        seq_t term = two * bk * tk;  /* 2^k * C(n,k) * C(2n+2k,k) */
        sum += term;                 /* accumulate */

        two <<= 1;                   /* next power of two */
    }

    return sum;
}
