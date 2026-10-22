/*
 * A320152  Number of integer solutions to a^2 + 2*b^2 + 2*c^2 + 6*d^2 = n.
 * https://oeis.org/A320152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to a² + 2 b² + 2 c² + 6 d² =
 *            n
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square‑root of an unsigned 64‑bit number (floor) */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 sq = (__int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) lo = mid + 1;
        else hi = mid - 1;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* upper bound for |d| is floor(sqrt(n / 6)) */
    idx_t max_d = isqrt_u64(n / (idx_t)6);
    int64_t d_lim = (int64_t)max_d;

    for (int64_t d = -d_lim; d <= d_lim; ++d) {
        idx_t d2 = (idx_t)((int64_t)d * d);
        idx_t six_d2 = (idx_t)6 * d2;
        if (six_d2 > n) continue;   /* just in case of overflow */
        idx_t m = n - six_d2;        /* remaining after fixing d */

        idx_t max_a = isqrt_u64(m);
        int64_t a_lim = (int64_t)max_a;
        for (int64_t a = -a_lim; a <= a_lim; ++a) {
            idx_t a2 = (idx_t)((int64_t)a * a);
            if (a2 > m) continue;
            idx_t m2 = m - a2;          /* remaining after fixing a */

            idx_t max_b = isqrt_u64(m2 / (idx_t)2);
            int64_t b_lim = (int64_t)max_b;
            for (int64_t b = -b_lim; b <= b_lim; ++b) {
                idx_t b2 = (idx_t)((int64_t)b * b);
                idx_t two_b2 = (idx_t)2 * b2;
                if (two_b2 > m2) continue;
                idx_t rem = m2 - two_b2; /* must equal 2*c² */

                if (rem & (idx_t)1) continue;   /* odd, not representable */

                idx_t c2 = rem >> 1;            /* c² = rem / 2 */
                idx_t croot = isqrt_u64(c2);
                if (croot * croot == c2) {
                    /* c = 0 contributes 1 solution,
                       otherwise ±c contribute 2 solutions */
                    count += (croot == 0 ? (seq_t)1 : (seq_t)2);
                }
            }
        }
    }
    return count;
}
