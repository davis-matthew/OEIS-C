/*
 * A333937  Triangle read by rows: T(n, k) = (k-1)*n - binomial(n, 2) +
 *          floor((n-k)/2) + 1, transposed.
 * https://oeis.org/A333937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r-1)*c - binom(c,2) + floor((c - r)/2) + 1, where r and
 *            c are the row and column of the n‑th entry of the triangle read
 *            by rows (T(n,k) transposed)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Safety guard – the sequence is defined for n ≥ 1 */
    if (n == 0) return (seq_t)0;

    /*--- Find the row r: smallest integer with r*(r+1)/2 ≥ n ---*/
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)(mid + 1) * mid / 2;
        if (tri >= n) high = mid;
        else          low = mid + 1;
    }
    idx_t r = low;                          /* row index (1‑based) */

    /*--- Compute column c (1‑based) ---*/
    idx_t prev = (idx_t)((__int128)(r - 1) * r / 2); /* sum of previous rows */
    idx_t c = n - prev;                          /* column index in this row */

    /*--- floor((c - r)/2) with correct floor for negative values ---*/
    int64_t diff   = (int64_t)c - (int64_t)r;
    int64_t floord = (diff >= 0) ? (diff / 2) : ((diff - 1) / 2);

    /*--- Compute the value using 128‑bit arithmetic for safety ---*/
    __int128 val = (__int128)(r - 1) * c           /* (r-1)*c */
                  - (__int128)c * (c - 1) / 2     /* binom(c,2) */
                  + ( (__int128)floord )
                  + 1;

    return (seq_t)val;
}
