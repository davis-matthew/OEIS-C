/*
 * A337374  Numbers k for which A003961(k) > 4*k.
 * https://oeis.org/A337374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with A003961(k) > 4*k (search over k, A003961
 *            by trial factorization)
 * Range:     u8 n <= 58, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9897 (2026-11-08)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/next_prime.h"

static uint64_t A003961_val(uint64_t k) {
    uint64_t result = 1;
    uint64_t m = k;
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

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t k = 0;
    while (count < (uint64_t)n) {
        k++;
        if (A003961_val(k) > 4 * k) count++;
    }
    return (seq_t)k;
}
