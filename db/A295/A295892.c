/*
 * A295892  a(n) = 1 if binary weights of n and A003961(n) are of the same
 *          parity, 0 otherwise.
 * https://oeis.org/A295892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 - (A000120(n) + A000120(A003961(n))) mod 2, i.e.
 *            A000035(1+A000120(n)+A000120(A003961(n)))
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 136 terms through n = 8101 (2026-11-08)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/next_prime.h"

static uint64_t A295892_A003961(uint64_t m) {
    uint64_t result = 1;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            while (m % p == 0) {
                m /= p;
                result *= q;
            }
        }
    }
    if (m > 1) {
        result *= next_prime(m);
    }
    return result;
}

static unsigned A295892_wt(uint64_t x) {
    unsigned c = 0;
    while (x) {
        c += (unsigned)(x & 1u);
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    unsigned s = 1u + A295892_wt(m) + A295892_wt(A295892_A003961(m));
    return (seq_t)(s & 1u);
}
