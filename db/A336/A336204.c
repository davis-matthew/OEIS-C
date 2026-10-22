/*
 * A336204  a(n) = Sum_{k=0..n} 2^k * binomial(n,k)^n.
 * https://oeis.org/A336204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^k * binomial(n,k)^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow, binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t pow2k = 1;                     /* 2^0 */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t c    = binomial(n, k);          /* C(n,k) */
        seq_t pow  = big_pow(c, (seq_t)n);    /* C(n,k)^n */
        sum += pow * pow2k;                   /* 2^k * C(n,k)^n */
        pow2k = pow2k * 2;                    /* next power of two */
    }
    return sum;
}
