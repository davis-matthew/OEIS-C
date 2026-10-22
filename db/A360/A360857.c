/*
 * A360857  Triangle read by rows. T(n, k) = binomial(n, ceil(k/2)) *
 *          binomial(n + 1, floor(k/2)).
 * https://oeis.org/A360857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r, ceil(k/2)) * binomial(r+1, floor(k/2)) where
 *            r = largest integer with r*(r+1)/2 <= n, k = n - r*(r+1)/2
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r containing index n via binary search */
    idx_t low = 0, high = n;
    idx_t r;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t t = mid * (mid + 1) / 2;
        if (t <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    r = high;  /* satisfies r*(r+1)/2 <= n < (r+1)*(r+2)/2 */

    idx_t prev_cnt = r * (r + 1) / 2;
    idx_t k = n - prev_cnt;

    idx_t c1 = (k + 1) / 2;  /* ceil(k/2) */
    idx_t c2 = k / 2;        /* floor(k/2) */

    seq_t v1 = binomial(r, c1);
    seq_t v2 = binomial(r + 1, c2);
    return v1 * v2;
}
