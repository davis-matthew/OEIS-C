/*
 * A242640  Triangle read by rows: T(s,n) (1 <= s <= n) = Sum_{d|n, d <= s}
 *          d^2 + s*Sum_{d|n, d>s} d.
 * https://oeis.org/A242640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A242640: Triangle read by rows  
 * T(s,n) = Sum_{d|n, d <= s} d^2 + s*Sum_{d|n, d > s} d
 * a(n) = T(s,n) where (s,n) are obtained by unfolding the triangle row‑by‑row.
 */
#include <math.h>   /* for sqrt, though not used directly */
#include <stdint.h>

seq_t a(idx_t idx)
{
    /* Find the row r such that r*(r+1)/2 >= idx */
    idx_t r = 1;
    while ((unsigned __int128)r * (r + 1) / 2 < idx)
        ++r;

    /* Position s within the row */
    idx_t prev_tri = (unsigned __int128)r * (r - 1) / 2;
    idx_t s = idx - prev_tri;   /* 1 <= s <= r */

    seq_t sum_sq = 0;
    seq_t sum_lin = 0;

    for (idx_t d = 1; d * d <= r; ++d) {
        if (r % d == 0) {
            if (d <= s)
                sum_sq += (seq_t)d * (seq_t)d;
            else
                sum_lin += d;

            idx_t d2 = r / d;
            if (d2 != d) {
                if (d2 <= s)
                    sum_sq += (seq_t)d2 * (seq_t)d2;
                else
                    sum_lin += d2;
            }
        }
    }

    return sum_sq + (seq_t)s * sum_lin;
}
