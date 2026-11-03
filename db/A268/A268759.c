/*
 * A268759  Triangle T(n,k) read by rows: T(n,k) = (1/4)*(1 + k)*(2 + k)*(k -
 *          n)*(1 + k - n).
 * https://oeis.org/A268759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where * r = floor((sqrt(8 n+1)-1)/2) (row index) *
 *            k = n - r*(r+1)/2 (column index in that row) * T(r,k) =
 *            ((k+1)*(k+2)*(r-k)*(r-k-1))/4 for r > k+1, * 0 otherwise
 * Range:     u8 n <= 68, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 70 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t triangle_term(idx_t r, idx_t k)
{
    /* For r <= k+1 the product contains a zero factor. */
    if (r <= k + 1) {
        return 0;
    }
    seq_t a = (seq_t)(k + 1);
    seq_t b = (seq_t)(k + 2);
    seq_t c = (seq_t)(r - k);
    seq_t d = (seq_t)(r - k - 1);
    return (a * b * c * d) / 4;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    /* Compute column index k within the row. */
    idx_t k = n - r * (r + 1) / 2;

    return triangle_term(r, k);
}
