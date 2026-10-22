/*
 * A356250  Square array T(n,k), n >= 1, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=1..n} (j * floor(n/j))^k.
 * https://oeis.org/A356250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) where T(n,k) = Σ_{j=1..n} (j * ⌊n/j⌋)^k read
 *            by antidiagonals downwards (n≥1, k≥0)
 * Range:     u8 n <= 30, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the diagonal index s (smallest s with s*(s+1)/2 ≥ n). */
    idx_t s = 1;
    while ((s * (s + 1)) / 2 < n) {
        ++s;
    }

    /* Elements before the current diagonal. */
    idx_t prev = (s - 1) * s / 2;

    /* Zero‑based position inside the current diagonal of length s. */
    idx_t t = n - prev - 1;

    /* Corresponding coordinates (row, col) in the square array. */
    idx_t row = t + 1;
    idx_t col = s - 1 - t;

    /* T(n,0) = n. */
    if (col == 0) {
        return (seq_t)row;
    }

    /* Compute Σ_{j=1..row} (j * ⌊row/j⌋)^col. */
    seq_t sum = 0;
    for (idx_t j = 1; j <= row; ++j) {
        idx_t q = row / j;
        seq_t base = (seq_t)j * (seq_t)q;
        seq_t term = big_pow(base, (seq_t)col);
        sum += term;
    }
    return sum;
}
