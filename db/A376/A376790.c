/*
 * A376790  Table T(n,k) read by antidiagonals: T(n,k) (n >=1, k >= 2) is the
 *          number of inversions in the radix-k digit reversal permutation of
 *          0, 1, ..., k^n-1.
 * https://oeis.org/A376790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(n,k) where (n,k) are obtained by reading the table
 *            T(n,k) = (k^(2*n) - n*k^(n+1) + (n-1)*k^n)/4 in antidiagonal
 *            order (sum n+k ascending, n ascending)
 * Range:     u8 n <= 7, u16 n <= 24, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t idx)
{
    /* Find diagonal index t such that t*(t+1)/2 >= idx   (t = s-2) */
    seq_t x = (seq_t)idx * 8 + 1;          /* 8*idx + 1   */
    seq_t sqrtx = floor_sqrt(x);
    idx_t t = (idx_t)((sqrtx - 1) / 2);

    /* Adjust t upward until triangular(t) >= idx */
    while ((seq_t)t * (t + 1) / 2 < idx)
        ++t;

    /* Previous cumulative count before this diagonal */
    idx_t prev = (t * (t - 1)) / 2;
    idx_t pos0  = idx - prev - 1;          /* 0‑based position inside the diagonal */

    idx_t n_val = pos0 + 1;                /* row number in this diagonal   */
    idx_t k_val = (t + 2) - n_val;         /* column number                */

    seq_t n_seq = (seq_t)n_val;
    seq_t k_seq = (seq_t)k_val;

    /* Compute k^n */
    seq_t pow_val = 1;
    for (idx_t i = 0; i < n_val; ++i)
        pow_val *= k_seq;

    /* Compute B = k^n - n*k + (n-1) */
    seq_t B = pow_val;
    B -= n_seq * k_seq;
    B += n_seq - 1;

    /* T(n,k) = pow_val * B / 4 */
    seq_t result = (pow_val * B) / 4;
    return result;
}
