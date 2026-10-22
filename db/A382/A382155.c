/*
 * A382155  a(n) = (n+1)! if n <= 2; thereafter a(n) = 4*n if n even or 2*n if
 *          n odd.
 * https://oeis.org/A382155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)! if n <= 2; thereafter a(n) = 4*n if n even or 2*n
 *            if n odd
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;
    if ((n & 1) == 0) {
        return ((seq_t)n) * ((seq_t)4);
    } else {
        return ((seq_t)n) * ((seq_t)2);
    }
}
