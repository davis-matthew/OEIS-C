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
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

/* Find the antidiagonal D that contains the given index n (1‑based). */
static inline idx_t diagonal_of_idx(idx_t n)
{
    idx_t low = 1, high = 1;
    /* Exponential search for an upper bound */
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;
    /* Binary search */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

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
        seq_t powjk = ipow_idx((seq_t)j, col);
        seq_t mult  = powjk * (seq_t)(row / j);
        sum += mult;
    }
    return sum;
}
