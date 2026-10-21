/*
 * A270566  Number of ordered ways to write n as x^4 + y*(3y+1)/2 +
 *          z*(7z+1)/2, where x, y and z are integers with x nonnegative.
 * https://oeis.org/A270566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A270566
   Formula: a(n) = |{ (x,y,z)∈ℕ×ℤ×ℤ : x≥0 and
                       x^4 + y*(3y+1)/2 + z*(7z+1)/2 = n }|
*/
#include <stdint.h>

static inline idx_t isqrt_u64(idx_t v) {
    idx_t r = 0, l = 0, h = v;
    while (l <= h) {
        idx_t m = l + ((h - l) >> 1);
        if (m == 0) { r = 0; break; }
        if (m <= v / m) { r = m; l = m + 1; }
        else { h = m - 1; }
    }
    return r;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    for (idx_t x = 0; ; ++x) {
        seq_t x4 = (seq_t)x * (seq_t)x * (seq_t)x * (seq_t)x;
        if (x4 > (seq_t)n) break;
        idx_t rem1 = (idx_t)((seq_t)n - x4);
        seq_t rem1_seq = (seq_t)rem1;
        idx_t lim_y = isqrt_u64(rem1) + 2;
        long long limyl = (long long)lim_y;
        for (long long y = -limyl; y <= limyl; ++y) {
            int64_t y_sq = y * y;
            seq_t f_val = ((seq_t)(3 * y_sq + y)) / 2;
            if (f_val > rem1_seq) continue;
            seq_t rem2_seq = rem1_seq - f_val;
            idx_t rem2 = (idx_t)rem2_seq;
            idx_t lim_z = isqrt_u64(rem2) + 2;
            long long limzl = (long long)lim_z;
            for (long long z = -limzl; z <= limzl; ++z) {
                int64_t z_sq = z * z;
                seq_t g_val = ((seq_t)(7 * z_sq + z)) / 2;
                if (g_val == rem2_seq) count++;
            }
        }
    }
    return count;
}
