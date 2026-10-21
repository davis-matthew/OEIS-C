/*
 * A291290  a(n) = n/3 if n == 0 mod 3, floor((n+1)/2) if n == 1 mod 3,
 *          otherwise n-2.
 * https://oeis.org/A291290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/3 if n % 3 == 0, floor((n+1)/2) if n % 3 == 1,
 *            otherwise n-2
 * Range:     u8 n <= 259, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t mod3 = n % 3;
    if (mod3 == 0) {
        return (seq_t)(n / 3);
    } else if (mod3 == 1) {
        /* floor((n+1)/2) = (n >> 1) + (n & 1) */
        return (seq_t)((n >> 1) + (n & 1));
    } else {
        return (seq_t)(n - 2);
    }
}
