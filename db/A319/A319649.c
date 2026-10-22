/*
 * A319649  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals: A(n,k)
 *          = Sum_{j=1..n} j^k * floor(n/j).
 * https://oeis.org/A319649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{j=1}^{row} j^col * floor(row/j) where (row, col) are
 *            obtained by mapping the index n to the antidiagonal of the
 *            square array A(n,k)
 * Range:     u8 n <= 30, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   diagonal_of_idx, seq_pow_custom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/diagonal_of_idx.h"
#include "../utils/seq_pow_custom.h"

seq_t a(idx_t n)
{
    /* Determine (row, col) from the index n via antidiagonal mapping. */
    idx_t D = diagonal_of_idx(n);
    unsigned __int128 prev = (unsigned __int128)(D - 1) * D / 2;
    idx_t pos  = n - (idx_t)prev - 1;   /* 0‑based position on this diagonal */
    idx_t row  = pos + 1;               /* n in the definition A(n,k) */
    idx_t col  = D - row;               /* k in the definition A(n,k) */

    /* Compute the sum A(row, col) = Σ_{j=1}^{row} j^col * floor(row/j). */
    seq_t sum = (seq_t)0;
    for (idx_t j = 1; j <= row; ++j) {
        seq_t powjk = seq_pow_custom((seq_t)j, col);
        seq_t mult  = powjk * (seq_t)(row / j);
        sum += mult;
    }
    return sum;
}
