/*
 * A286813  Number of positive odd solutions to equation x^2 + 8*y^2 = 8*n +
 *          9.
 * https://oeis.org/A286813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 8221, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of positive odd solutions (x, y) to
 * x^2 + 8*y^2 = 8*n + 9.  Both x and y must be odd positive integers.
 * For each odd y <= floor(sqrt(n+1)), compute
 *     val = 8*(n+1 - y^2) + 1
 *   which equals x^2.  If val is a perfect square with odd square root,
 *   we have a solution.  Count all such solutions. */
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* Integer square root for unsigned 64‑bit values */
static idx_t uint64_sqrt(idx_t n)
{
    idx_t low = 0, high;
    if (n >= 4) {
        high = (n >> 1) + 1;
    } else {
        high = n;
    }
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid && mid > n / mid) {
            high = mid - 1;
        } else {
            low = mid;
        }
    }
    return low;
}

/* Integer square root for arbitrary precision seq_t */
static seq_t seq_isqrt(seq_t v)
{
    if (v == 0) return 0;
    unsigned bits = bitlen_seq_t(v);
    unsigned sqrt_bits = (bits + 1) >> 1;
    seq_t low = 0;
    seq_t high = ((seq_t)1 << sqrt_bits);
    while (low < high) {
        seq_t mid = (low + high) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= v) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t ymax = uint64_sqrt(n + 1);       /* maximum odd y: y^2 <= n+1 */
    for (idx_t y = 1; y <= ymax; y += 2) {
        idx_t yy = y * y;
        /* val = 8*(n + 1 - y^2) + 1, which must be a square */
        seq_t val = (seq_t)8 * ((seq_t)n + 1 - (seq_t)yy) + 1;
        seq_t s = seq_isqrt(val);
        if (s * s == val && (s & 1)) {
            if (s > 0) count += 1;
        }
    }
    return count;
}
