/*
 * A334703  Triangle read by rows: T(n,k) = binomial(n*k,3) (0 <= k <= n).
 * https://oeis.org/A334703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binom(r * k, 3) * where r = floor((sqrt(8*n+1) - 1) / 2)
 *            is the row * index and k = n - r*(r+1)/2 is the column within
 *            that row
 * Range:     u8 n <= 12, u16 n <= 42, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   _floor_isqrt_u64, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* n >= 0, idx_t is unsigned 64-bit */
    idx_t rows = (_floor_isqrt_u64(8 * n + 1) - 1) / 2;          /* r */
    idx_t k = n - rows * (rows + 1) / 2;                        /* column */
    idx_t prod = rows * k;                                      /* r*k */
    return binom(prod, 3);                                      /* C(r*k,3) */
}
