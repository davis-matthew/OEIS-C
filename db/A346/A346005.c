/*
 * A346005  Successive numbers arising from the Moessner construction of the
 *          cubes on page 64 of Conway-Guy's "Book of Numbers".
 * https://oeis.org/A346005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 120, u32 n <= 4875, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A346005
 * Formula: a(n) = n           if n mod 3 == 0
 *          a(n) = ((n+2)/3)^3 if n mod 3 == 1
 *          a(n) = (n+1)^2/3   otherwise
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t r = n % 3;
    if (r == 0) {
        return (seq_t)n;
    } else if (r == 1) {
        seq_t m = ((seq_t)n + 2) / 3;  // integer division, exact
        return m * m * m;
    } else { /* r == 2 */
        seq_t plus1 = (seq_t)n + 1;
        return (plus1 * plus1) / 3;    // exact division
    }
}
