/*
 * A324937  Triangle read by rows: T(n, k) = 2*n*k + n + k - 8.
 * https://oeis.org/A324937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign, less
 * Formula:   a(n) = 2 * r * k + r + k - 8. Here r = ceil((sqrt(8n+1)-1)/2)
 *            (row index) and k = n - r*(r-1)/2 (column index)
 * Range:     i8 n <= 35, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) for the OEIS sequence A324937. */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 >= n.  Use binary search with 128‑bit
       intermediate to avoid overflow. */
    idx_t low  = 1;
    idx_t high = n;              /* r cannot exceed n */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 prod = (__int128)mid * (mid + 1) / 2;
        if (prod >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;               /* row index (1‑based) */

    /* Number of terms before row r: (r-1)*r/2 (computed with 128‑bit). */
    __int128 before128 = (__int128)(r - 1) * r / 2;
    idx_t k = (idx_t)(n - before128);  /* column index (1‑based) */

    /* Compute the triangle entry 2*n*k + n + k - 8 in seq_t. */
    seq_t rr = (seq_t)r;
    seq_t kk = (seq_t)k;
    return 2 * rr * kk + rr + kk - 8;
}
