/*
 * A393562  a(n) is the number of unit squares in the square lattice, divided
 *          by 4, that have at least one vertex strictly inside and at least
 *          one vertex strictly outside the circle x^2 + y^2 = n.
 * https://oeis.org/A393562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts unit lattice squares straddling the circle x^2 + y^2
 *            = n, then returns that count divided by 4
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of an unsigned 64‑bit number. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, mid, ans = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * (__int128)mid;
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t r = isqrt_u64(n);

    for (int64_t i = -(int64_t)r - 1; i <= (int64_t)r; ++i) {
        for (int64_t j = -(int64_t)r - 1; j <= (int64_t)r; ++j) {
            /* distances squared of the four corners */
            seq_t d00 = (seq_t)((int64_t)i * i) + (seq_t)((int64_t)j * j);
            seq_t d10 = (seq_t)((int64_t)(i + 1) * (i + 1)) + (seq_t)((int64_t)j * j);
            seq_t d01 = (seq_t)((int64_t)i * i) + (seq_t)((int64_t)(j + 1) * (j + 1));
            seq_t d11 = (seq_t)((int64_t)(i + 1) * (i + 1)) + (seq_t)((int64_t)(j + 1) * (j + 1));

            seq_t min = d00;
            if (d10 < min) min = d10;
            if (d01 < min) min = d01;
            if (d11 < min) min = d11;

            seq_t max = d00;
            if (d10 > max) max = d10;
            if (d01 > max) max = d01;
            if (d11 > max) max = d11;

            if (min < (seq_t)n && max > (seq_t)n)
                ++count;
        }
    }

    return count / (seq_t)4;
}
