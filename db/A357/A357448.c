/*
 * A357448  Fixed point starting with 0 of the two-block substitution 00->010,
 *          01->010, 10->101, 11->101.
 * https://oeis.org/A357448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A244040(n) mod 2. Recurrence: a(3n) = a(2n) a(3n+1) = 1 -
 *            a(2n) a(3n+2) = a(2n) Initial values: a(0)=0, a(1)=1, a(2)=0
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 0;

    bool inv = false;
    while (n > 2) {
        idx_t q = n / 3;
        idx_t r = n - 3 * q;  /* n % 3, but avoid modulo on 64-bit */
        if (r == 1) inv = !inv;
        n = 2 * q;
    }

    seq_t val = (n == 1) ? 1 : 0; /* n == 0 or 2 give 0 */
    if (inv) val ^= 1;
    return val;
}
