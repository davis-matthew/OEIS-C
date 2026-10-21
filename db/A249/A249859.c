/*
 * A249859  Least common multiple of n + 2 and n - 2.
 * https://oeis.org/A249859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(n - 2, n + 2)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* a(1) = 3
     * a(2) = 0
     * For n >= 3, compute lcm(n-2, n+2) = (n-2)*(n+2)/gcd(n-2, n+2) */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)0;

    seq_t n_seq = (seq_t)n;
    seq_t x = n_seq - (seq_t)2;
    seq_t y = n_seq + (seq_t)2;
    seq_t g = gcd(x, y);
    /* The division is exact because g divides x */
    return (x / g) * y;
}
