/*
 * A244540  Expansion of phi(q) * (phi(q) + phi(q^2)) / 2 in powers of q where
 *          phi() is a Ramanujan theta function.
 * https://oeis.org/A244540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (r_2(n)+r_{1,2}(n))/2, where * r_2(n) = 4*(d1(n)-d3(n))
 *            (d1,d3 are numbers of divisors congruent to 1,3 mod 4) *
 *            r_{1,2}(n) counts integer solutions of a²+2b²=n, with signs and
 *            order differentiated
 * Range:     u8 n <= 2500, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   r2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/r2.h"

/* Integer square root: floor(sqrt(x)) for non‑negative x */
static inline idx_t int_sqrt(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > x / mid)   /* mid*mid would overflow  */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* Number of representations of n as a^2 + 2b^2 (order and sign counted). */
static inline seq_t r12(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t count = 0;
    for (idx_t b = 0;; ++b) {
        idx_t bb = b * b;
        idx_t two_bb = bb * 2;
        if (two_bb > n) break;
        idx_t a_sq = n - two_bb;
        idx_t a = int_sqrt(a_sq);
        if (a * a == a_sq) {
            seq_t sols = (a == 0) ? 1 : 2;   /* a = 0 gives one solution */
            if (b == 0)
                count += sols;
            else
                count += sols << 1;          /* b = ± gives double the count */
        }
    }
    return count;
}

seq_t a(idx_t n)
{
    seq_t r2val   = r2(n);
    seq_t r12val  = r12(n);
    return (r2val + r12val) >> 1;  /* divide by 2 */
}
