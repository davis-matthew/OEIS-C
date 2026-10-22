/*
 * A360965  Array T(n,m) = (2^(n*m)-1)/(2^m-1) read by antidiagonals, n,m>=1.
 * https://oeis.org/A360965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (2^(i*j)-1)/(2^j-1) where (i,j) are the unique positive
 *            integers such that - let d be the smallest integer with
 *            d*(d+1)/2 ≥ n; - i = n - (d-1)*d/2 (row index); - j = d - i + 1
 *            (column index); Then a(n) is the entry of the array T(i,j) =
 *            (2^(i*j)-1)/(2^j-1)
 * Range:     u8 n <= 12, u16 n <= 30, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Sequence is 1‑based. For n == 0 return 0 to keep the
       function total – this case is never used by the OEIS test. */
    if (n == 0)
        return 0;

    /* Find the diagonal d for which d*(d+1)/2 ≥ n. */
    idx_t d = 1;
    while (d * (d + 1) / 2 < n)
        ++d;

    /* Number of terms preceding this diagonal. */
    idx_t prev = (d - 1) * d / 2;

    /* Row (i) and column (j) indices on the diagonal. */
    idx_t i = n - prev;          /* 1 ≤ i ≤ d */
    idx_t j = d - i + 1;         /* 1 ≤ j ≤ d */

    /* Compute T(i,j) = (2^(i*j)-1)/(2^j-1). */
    seq_t numerator   = pow2(i * j) - 1;
    seq_t denominator = pow2(j)     - 1;

    return numerator / denominator;
}
