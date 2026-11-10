/*
 * A354837  Odd numbers k such that gcd(k, A007088(k)) != 1.
 * https://oeis.org/A354837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th odd k with gcd(k, A007088(k)) > 1, where A007088(k)
 *            is the binary expansion of k read as a decimal number; found by
 *            search, computing A007088(k) mod k bit by bit
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A354837_is_term(uint64_t k) {
    unsigned __int128 r = 0;
    int i;
    for (i = 63; i >= 0; i--) {
        if ((k >> i) != 0) {
            r = (r * 10 + ((k >> i) & 1)) % k;
        }
    }
    uint64_t x = k, y = (uint64_t)r;
    while (y) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x != 1;
}

seq_t a(idx_t n) {
    uint64_t k = 1;
    idx_t count = 0;
    while (count < n) {
        k += 2;
        if (A354837_is_term(k)) count++;
    }
    return (seq_t)k;
}
