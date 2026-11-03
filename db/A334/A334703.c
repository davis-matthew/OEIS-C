/*
 * A334703  Triangle read by rows: T(n,k) = binomial(n*k,3) (0 <= k <= n).
 * https://oeis.org/A334703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binom(r * k, 3) * where r = floor((sqrt(8*n+1) - 1) / 2)
 *            is the row * index and k = n - r*(r+1)/2 is the column within
 *            that row
 * Range:     u8 n <= 13, u16 n <= 53, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-01)
 * Helpers:   binomial, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    /* n >= 0, idx_t is unsigned 64-bit */
    idx_t rows = (isqrt(8 * n + 1) - 1) / 2;          /* r */
    idx_t k = n - rows * (rows + 1) / 2;                        /* column */
    idx_t prod = rows * k;                                      /* r*k */
    return binomial(prod, 3);                                      /* C(r*k,3) */
}
