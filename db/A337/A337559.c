/*
 * A337559  Number of length three 1..n vectors that contain their harmonic
 *          mean.
 * https://oeis.org/A337559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A337559(n) = number of length‑three 1..n vectors that contain
 *            their harmonic mean. The algorithm enumerates the harmonic mean
 *            w (1 ≤ w ≤ n) and all unordered solutions (w,u,v) satisfying
 *            (2u−w)(2v−w)=w², which is equivalent to 1/w + 1/u + 1/v = 3/w.
 *            All permutations of each unordered triple are c...
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t ans = 0;
    for (idx_t w = 1; w <= n; ++w) {
        seq_t w2 = (seq_t)w * w;          /* w^2  (fits in seq_t even for large n) */
        for (idx_t d = 1; d <= w; ++d) {   /* loop over divisors of w^2 upto sqrt(w^2) == w */
            if (w2 % d != 0) continue;
            idx_t e = (idx_t)(w2 / d);     /* second divisor, e >= d since d <= w */

            /* parity condition: for even w both divisors must be even */
            if ((w & 1) == 0) {
                if ((d & 1) || (e & 1)) continue;
            }
            /* for odd w the parity is automatically correct – all divisors are odd */

            /* Compute the remaining two coordinates */
            seq_t u = ((seq_t)d + (seq_t)w) / 2;
            seq_t v = ((seq_t)e + (seq_t)w) / 2;

            if (u > n || v > n) continue;

            /* Count permutations of the unordered triple {w,u,v} */
            seq_t perm;
            if (u == w && v == w) {
                perm = 1;
            } else if (u == v || u == w || v == w) {
                perm = 3;
            } else {
                perm = 6;
            }
            ans += perm;
        }
    }
    return ans;
}
