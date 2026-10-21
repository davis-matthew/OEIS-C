/*
 * A281334  Triangle read by rows: T(n, k) = (n - k)*(k + 1)^3 + k, 0 <= k <=
 *          n.
 * https://oeis.org/A281334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the nth element of the triangle T(n,k) = (n - k)*(k +
 *            1)^3 + k, read by rows with offset 1 (row‑by‑row ordering)
 * Range:     u8 n <= 31, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t find_row(idx_t n)
{
    /* Find the largest r such that r*(r+1)/2 < n.
       Uses binary search over 0..n (safe because r <= n). */
    idx_t low = 0;
    idx_t high = n;          /* r cannot exceed n */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;   /* upper half bias */
        if (((__uint128_t)mid * (mid + 1) / 2) < n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;   /* row index, 0‑based */
}

seq_t a(idx_t n)
{
    /* offset is 1 (as in OEIS), so n >= 1 */
    idx_t r = find_row(n);                     /* row index */
    idx_t prev_cum = (idx_t)(((__uint128_t)r * (r + 1)) / 2); /* terms before row r */
    idx_t k = n - prev_cum - 1;                /* column index */

    seq_t s_r = (seq_t)r;
    seq_t s_k = (seq_t)k;
    seq_t k1 = s_k + 1;
    seq_t pow3 = k1 * k1 * k1;                 /* (k+1)^3 */
    seq_t diff = s_r - s_k;                    /* r - k */
    seq_t term = diff * pow3 + s_k;            /* (r-k)*(k+1)^3 + k */

    return term;
}
