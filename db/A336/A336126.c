/*
 * A336126  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A000035(i) = A000035(j) and A007814(1+A000265(i)) =
 *          A007814(1+A000265(j)), for all i, j >= 1.
 * https://oeis.org/A336126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(t-1) + 1 + [n even], where t = number of trailing
 *            1-bits in the odd part of n (A007814(1+A000265(n))). This is the
 *            restricted growth transform of [n mod 2, t]
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9941 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    unsigned even = 0;
    while ((n & 1u) == 0) {
        n >>= 1;
        even = 1;
    }
    unsigned t = 0;
    while (n & 1u) {
        t++;
        n >>= 1;
    }
    return (seq_t)(2u * (t - 1u) + 1u + even);
}
