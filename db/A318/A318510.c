/*
 * A318510  Completely multiplicative with a(prime(k)) = A002487(prime(k+1)).
 * https://oeis.org/A318510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, look
 * Formula:   a(n) = Product a(p)^e over n = Product p^e, completely
 *            multiplicative, a(prime(k)) = A002487(prime(k+1))
 * Range:     u8 n <= 255, u16 n <= 65535, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 104 terms through n = 9290 (2026-11-08)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static uint64_t A002487_fusc(uint64_t n) {
    uint64_t x = 1, y = 0;
    while (n > 0) {
        if (n & 1) y += x;
        else x += y;
        n >>= 1;
    }
    return y;
}

seq_t a(idx_t n) {
    seq_t result = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            seq_t s = (seq_t)A002487_fusc(next_prime(p));
            while (m % p == 0) {
                m /= p;
                result *= s;
            }
        }
    }
    if (m > 1) {
        result *= (seq_t)A002487_fusc(next_prime(m));
    }
    return result;
}
