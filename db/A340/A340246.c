/*
 * A340246  Sum of the smallest parts q of the partitions of n into 4 parts
 *          q,r,s,t such that 1 <= q <= r <= s <= t and q + r + s > t.
 * https://oeis.org/A340246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sum of the smallest parts q of the partitions of n into 4 parts
 *            q,r,s,t such that 1 <= q <= r <= s <= t and q + r + s > t.
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=1,
 * Range:     u8 n <= 28, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 4) {
        return (seq_t)0;
    }
    seq_t sum = 0;
    idx_t qmax = n / 4;
    for (idx_t q = 1; q <= qmax; ++q) {
        idx_t m = n - q;               /* remaining sum for r+s+t */
        idx_t rmax = m / 3;             /* floor((n-q)/3) */
        for (idx_t r = q; r <= rmax; ++r) {
            /* lower bound for s: maximum of r and floor((m - q - 2r)/2)+1 */
            long long lb = (long long)m - (long long)q - 2LL * (long long)r;
            idx_t s_min = r;
            if (lb > 0) {
                idx_t cand = (idx_t)(lb / 2) + 1;
                if (cand > s_min) s_min = cand;
            }
            /* upper bound for s: floor((m - r)/2) */
            idx_t s_max = (m - r) / 2;
            if (s_min > s_max) {
                continue; /* no s satisfies constraints */
            }
            idx_t count = s_max - s_min + 1; /* number of valid s values */
            seq_t contrib = (seq_t)q * (seq_t)count;
            sum += contrib;
        }
    }
    return sum;
}
