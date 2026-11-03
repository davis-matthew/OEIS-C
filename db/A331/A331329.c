/*
 * A331329  a(n) = binomial(5*n, n)*hypergeom([-4*n, -n], [-5*n], -1).
 * https://oeis.org/A331329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^k * binomial(n,k) * binomial(4*n,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 741 (2026-11-01)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t pow2k = 1;      /* 2^0 */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = binomial(n, k) * binomial(4 * n, k);
        term *= pow2k;
        sum += term;
        pow2k *= 2;       /* 2^(k+1) for next iteration */
    }

    return sum;
}
