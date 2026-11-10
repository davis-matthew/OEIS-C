/*
 * A274298  A bisection of A002326.
 * https://oeis.org/A274298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A002326(2*n) = multiplicative order of 2 mod (4n+1);
 *            computed by iterating powers of 2
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9771 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A002326_a(idx_t n) {
    seq_t p = (seq_t)n * 2 + 1;

    if (p == 1) return 1;

    seq_t power = 2 % p;
    seq_t m = 1;

    while (power != 1) {
        power = (power * 2) % p;
        m++;
    }

    return m;
}

seq_t a(idx_t n) {
    return A002326_a(2 * n);
}
