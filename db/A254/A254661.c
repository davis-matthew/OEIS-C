/*
 * A254661  Number of ways to write n as the sum of a triangular number, an
 *          even square and a second pentagonal number.
 * https://oeis.org/A254661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑negative integer solutions (x,y,z) with *
 *            x*(x+1)/2 + y^2 + z*(3*z+1)/2 = n, where y is even
 * Range:     u8 n <= 2, u16 n <= 100, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t x = 0;
         (seq_t)x * (x + 1) / 2 <= n;
         ++x) {
        seq_t t = (seq_t)x * (x + 1) / 2;
        seq_t rem_tri = (seq_t)n - t;
        for (idx_t y = 0; ; y += 2) {
            seq_t e = (seq_t)y * y;
            if (e > rem_tri)
                break;
            seq_t rem = rem_tri - e;          /* remainder to be a pentagonal number */
            /* Test if rem is a pentagonal number: p = z*(3z+1)/2 */
            /* Solve 24p+1 = k^2 where k ≡ 1 (mod 6) */
            seq_t val = 24 * rem + 1;
            seq_t k   = floor_sqrt(val);
            if (k * k == val && ((k - 1) % 6) == 0)
                count += 1;
        }
    }
    return count;
}
