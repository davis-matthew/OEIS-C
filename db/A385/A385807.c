/*
 * A385807  Number of integer lattice points (x, y) strictly inside a triangle
 *          of base 2n - 1 and height n - 1, such that 1 <= x <= 2n - 1, 1 <=
 *          y < min(x, 2n - x), and y | x.
 * https://oeis.org/A385807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of lattice points (x,y) strictly inside the
 *            triangle with base 2n-1 and height n-1 such that 1 ≤ x ≤ 2n-1, 1
 *            ≤ y < min(x, 2n‑x) and y | x. The implementation enumerates each
 *            x and counts divisors y of x with y ≤ min(x,2n‑x)-1
 * Range:     u8 n <= 42, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* External integer types: seq_t (sequence value) and idx_t (index).   */
seq_t a(idx_t n)
{
    /* 2*n-1 fits comfortably in idx_t (uint64_t). */
    idx_t max_x = 2 * n - 1;
    seq_t total = 0;

    for (idx_t x = 1; x <= max_x; ++x) {
        /* m = min(x, 2n - x). For 1 ≤ x ≤ 2n-1, both arguments are ≥ 1. */
        idx_t m = (x < (2 * n - x)) ? x : (2 * n - x);
        /* We need y < m, so we count divisors ≤ m-1. */
        idx_t limit = m - 1;
        if (limit == 0) continue;          /* No admissible y. */

        /* Enumerate all divisors of x up to sqrt(x). */
        for (idx_t d = 1; d * d <= x; ++d) {
            if (x % d == 0) {
                if (d <= limit)     /* divisor d itself */
                    ++total;
                idx_t other = x / d;   /* complementary divisor */
                if (other != d && other <= limit)
                    ++total;
            }
        }
    }
    return total;
}
