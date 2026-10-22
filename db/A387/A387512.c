/*
 * A387512  a(n) = Sum_{k=0..floor(n/3)} 2^k * 3^(n-2*k) *
 *          binomial(n-2*k,k)^2.
 * https://oeis.org/A387512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} 2^k * 3^(n-2k) * binomial(n-2k,k)^2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"      /* seq_t pow3(idx_t exp); */
#include "../utils/binomial.h"  /* seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow2   = 1;                      /* 2^k current value */
    idx_t maxk   = n / 3;                  /* last k */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t m = n - 2 * k;              /* exponent for 3 and binomial parameter */
        seq_t bin = binomial(m, k);       /* binomial(n-2k, k) */

        /* term = 2^k * 3^m * bin^2 */
        seq_t term = bin;                /* bin */
        term = term * bin;               /* bin^2 */
        term = term * pow3(m);           /* 3^(n-2k) */
        term = term * pow2;              /* 2^k */

        result += term;

        pow2 *= (seq_t)2;                /* prepare 2^(k+1) for next iteration */
    }

    return result;
}
