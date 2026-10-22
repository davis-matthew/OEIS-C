/*
 * A374258  Square array: T(n,k) = ((3^(n+1) + 1)^(k-1) + 2)/3, read by
 *          descending antidiagonals.
 * https://oeis.org/A374258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ((3^(row+1)+1)^(col-1)+2)/3, where (row,col) is the
 *            element of the square array read by descending antidiagonals
 *            (sum = row+col is constant)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 37, u64 n <= 70, u128 n <= 158,
 *            big any n
 * Helpers:   big_pow, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* n is guaranteed ≥ 1 (offset 1). */
    if (n == 0) return 0;              /* safety guard, though not used */

    /* Find diagonal t (sum = row+col) such that t*(t-1)/2 ≥ n. */
    idx_t t = 2;
    while (1) {
        __uint128_t val = (__uint128_t)t * (t - 1) / 2;
        if (val >= n) break;
        t++;
    }

    /* Number of entries before this diagonal. */
    __uint128_t prev_total = (__uint128_t)(t - 1) * (t - 2) / 2;

    /* 0‑based offset within the diagonal. */
    idx_t offset = (idx_t)(n - prev_total - 1);

    /* Convert offset to row and column indices. */
    idx_t row = offset + 1;          /* rows start at 1 */
    idx_t col = t - row;             /* columns also start at 1 */

    /* Base b = 3^(row+1)+1. */
    seq_t base = pow3(row + 1) + 1;

    /* Exponent e = col-1. */
    idx_t exp = col - 1;

    seq_t val = big_pow(base, (seq_t)exp); /* Compute base^exp */
    val += 2;
    val /= 3;
    return val;
}
