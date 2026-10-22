/*
 * A308698  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals, where
 *          A(n,k) is Sum_{d|n} d^(k*d).
 * https://oeis.org/A308698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read along antidiagonals of the square array * A(n,k) =
 *            ∑_{d|n} d^(k*d) (n≥1, k≥0)
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 31, u64 n <= 59, u128 n <= 97,
 *            big any n
 * Helpers:   antidiagonal_coords, big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/antidiagonal_coords.h"
#include "../utils/big_pow.h"

/* Compute the entry A(n,k) for a given row n (1‑based) and column k. */
static seq_t row_value(idx_t n, idx_t k)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            sum += big_pow((seq_t)d, (seq_t)(k * d));
            idx_t d2 = n / d;
            if (d2 != d)
                sum += big_pow((seq_t)d2, (seq_t)(k * d2));
        }
    }
    return sum;
}

/* Read (n,k) from the antidiagonal ordering, convert the row index
 * from 0‑based to 1‑based, and evaluate A(n,k). */
seq_t a(idx_t n)
{
    idx_t zero = n - 1;
    idx_t i, k;
    antidiagonal_coords(zero, &i, &k);
    idx_t row = i + 1;
    return row_value(row, k);
}
