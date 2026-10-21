/*
 * A260803  Number of ways to write n as n = x*y*z + x + y + z where 1 <= x <=
 *          y <= z <= n.
 * https://oeis.org/A260803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = number of solutions (x,y,z) with 1 ≤ x ≤ y ≤ z ≤ n and
 *            * n = x*y*z + x + y + z * Algorithm: For each x and y, solve the
 *            linear equation for z. * Complexity: O(n²) with a break that
 *            prunes large y
 * Range:     u8 n <= 47, u16 n <= 1566, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t cnt = 0;
    /* x can only be up to n/2, otherwise y ≤ n - x < x and no solutions */
    for (idx_t x = 1; x <= n / 2; ++x) {
        /* y ranges from x up to n - x (inclusive), otherwise T < 0 */
        for (idx_t y = x; y <= n - x; ++y) {
            /* T = n - x - y, guaranteed non‑negative in this loop */
            seq_t T = (seq_t)(n - x - y);
            seq_t denom = (seq_t)x * (seq_t)y + 1;
            if (denom > T) break;                 /* further y only increases denom, decreases T */
            if (T % denom != 0) continue;          /* z must be integer */
            seq_t z = T / denom;
            if (z < (seq_t)y || z > (seq_t)n) continue;  /* enforce x ≤ y ≤ z ≤ n */
            ++cnt;                                 /* found a valid triple */
        }
    }
    return cnt;
}
