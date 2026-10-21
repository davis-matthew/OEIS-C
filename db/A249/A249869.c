/*
 * A249869  Triangle giving the area of primitive Pythagorean triangles, with
 *          zero entries for non-primitive triangles.
 * https://oeis.org/A249869
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the flattened triangle of T(r,m) = r*m*(r+m)*(r-m) where
 *            r > m ≥ 1, r+m is odd, and gcd(r,m)=1, otherwise 0. a(2) is the
 *            first non‑zero term. The mapping from the flat index n to (r,m)
 *            uses n‑2 as a zero‑based position
 * Range:     u8 n <= 15, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* integer square root for unsigned 64‑bit values, using binary search.
   (Overflow‑safe when multiplied with unsigned __int128.) */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = (x >> 1) + 1;
    if (hi < 2) hi = 2;
    idx_t res = 0;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        if ((unsigned __int128)mid * mid <= x) {
            res = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                /* offset is 2 */

    /* zero‑based position in the flattened triangle */
    idx_t pos = n - 2;

    /* find row r such that (r-1)*r/2 > pos >= (r-2)*(r-1)/2 */
    idx_t temp = 1 + 8 * pos;
    idx_t sq   = isqrt_u64(temp);
    idx_t r    = (1 + sq) / 2 + 1;       /* r ≥ 2 */

    /* column index m (1‑based) within this row */
    idx_t cum_prev = (r - 2) * (r - 1) / 2;
    idx_t m  = pos - cum_prev + 1;       /* 1‑based column index */

    /* parity condition: r+m must be odd */
    if (((r + m) & 1ULL) == 0) return 0;

    /* coprimality */
    if (gcd((seq_t)r, (seq_t)m) != 1) return 0;

    /* compute the area value */
    seq_t val = (seq_t)r * (seq_t)m * (seq_t)(r + m) * (seq_t)(r - m);
    return val;
}
