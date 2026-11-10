/*
 * A274299  A bisection of A002326.
 * https://oeis.org/A274299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A002326(2*n+1) = multiplicative order of 2 mod (4*n+3);
 *            computed by iterating powers of 2
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9365 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t p = (seq_t)n * 4 + 3;
    seq_t power = 2 % p;
    seq_t m = 1;
    while (power != 1) {
        seq_t rem = p - power;
        if (power >= rem) power = power - rem;
        else power = power * 2;
        m++;
    }
    return m;
}
