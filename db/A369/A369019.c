/*
 * A369019  Triangle read by rows: T(n, k) = binomial(n, k - 1)*(k - 1)^(k -
 *          1)*k*(n - k + 1)^(n - k).
 * https://oeis.org/A369019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) where r = largest integer such that r(r+1)/2 <= n
 *            k = n - r(r+1)/2 (0 <= k <= r) T(r, k) = binomial(r,
 *            k-1)*(k-1)^(k-1)*k*(r-k+1)^(r-k) For k = 0, T(r,0) = 0
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the row r containing index n */
    idx_t r = 0;
    /* The next row will start at (r+1)(r+2)/2, so increase r until that exceeds n */
    while (((r + 1) * (r + 2)) / 2 <= n)
        r++;

    /* Offset of the first element in this row */
    idx_t offset = r * (r + 1) / 2;
    /* Column index within the row */
    idx_t k = n - offset;

    if (k == 0)
        return 0;               /* the first element of each row is zero */

    /* Compute the terms of the formula */
    seq_t bin = binomial(r, k - 1);
    seq_t pow1 = big_pow((seq_t)(k - 1), (seq_t)(k - 1));
    seq_t pow2 = big_pow((seq_t)(r - k + 1), (seq_t)(r - k));

    /* Multiply all parts together */
    seq_t result = bin;
    result = result * pow1;
    result = result * (seq_t)k;
    result = result * pow2;

    return result;
}
