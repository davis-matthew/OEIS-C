/*
 * A310578  Coordination sequence Gal.5.4.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = Coordination sequence Gal.5.4.5
 *   a(0) = 1
 *   Let d(m) = a(m+1) – a(m).
 *   d(0) = 3, d(1) = 2.
 *   For m ≥ 2 set r = m mod 8, k = m/8:
 *      if r == 0: d(m) = 6·k + 8
 *      else if r == 1: d(m) = –6·k
 *      else: d(m) = (m is odd)? 2 : 4
 *   Then a(n) = 1 + Σ_{m=0}^{n-1} d(m).  */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    int64_t sum = 1;   /* signed accumulator for intermediate sums */
    for (idx_t m = 0; m < n; ++m) {
        int64_t d;
        if (m == 0) {
            d = 3;
        } else if (m == 1) {
            d = 2;
        } else {
            idx_t r = m % 8;
            idx_t k = m / 8;
            if (r == 0) {
                d = 6 * (int64_t)k + 8;
            } else if (r == 1) {
                d = -6 * (int64_t)k;
            } else {
                d = (m & 1) ? 2 : 4;
            }
        }
        sum += d;
    }
    return (seq_t)sum;
}
