/*
 * A344110  Triangle read by rows: T(n,k) = 2^(n*k), n >= 0, 0 <= k <= n.
 * https://oeis.org/A344110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 2^(r*c) where r is the largest integer with r*(r+1)/2 <=
 *            n, and c = n - r*(r+1)/2. Here r is the row number and c the
 *            column index within that row
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 26, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find r by binary search on the triangular numbers using 128‑bit
       arithmetic to avoid overflows. */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;   /* prevent underflow in high */
            high = mid - 1;
        }
    }

    /* Compute the starting index of row r (s = r*(r+1)/2) safely. */
    idx_t row_start;
    if (r & 1) { /* r is odd */
        row_start = r * ((r + 1) / 2);
    } else {     /* r is even */
        row_start = (r / 2) * (r + 1);
    }

    idx_t c = n - row_start;          /* column in the row */
    seq_t exponent = (seq_t)r;        /* use seq_t for arbitrary precision */
    exponent *= (seq_t)c;              /* exponent = r * c */
    return big_pow((seq_t)2, exponent); /* compute 2^(exponent) */
}
