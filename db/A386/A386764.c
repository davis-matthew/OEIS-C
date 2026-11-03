/*
 * A386764  a(n) = Sum_{k=0..n} 5^k * 3^(n-k) * binomial(2*n+k-1,k).
 * https://oeis.org/A386764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 5^k * 3^(n-k) * binomial(2*n + k - 1, k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   binomial, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t p5 = 1;                 /* 5^k  */
    seq_t p3 = ipow_u64_idx(3, n);    /* 3^(n-k)  */
    seq_t bin = 1;                /* binomial(2*n + k - 1, k)  */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term = bin * p5;
        term *= p3;
        result += term;

        if (k < n)
        {
            bin = bin * (2 * n + k) / (k + 1); /* next binomial value */
            p5 *= 5;                          /* next 5^k */
            p3 /= 3;                          /* next 3^(n-k) */
        }
    }
    return result;
}
