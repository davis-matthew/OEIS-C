/*
 * A275717  Numbers n for which A003961(n) > A003961(n-1).
 * https://oeis.org/A275717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m >= 2 with A003961(m) > A003961(m-1); computed by a
 *            linear search over m
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9989 (2026-11-08)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static uint64_t A003961_f(uint64_t m) {
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

seq_t a(idx_t n) {
    uint64_t prev = 1;
    uint64_t count = 0;
    uint64_t m = 1;
    for (;;) {
        m++;
        uint64_t cur = A003961_f(m);
        if (cur > prev) {
            count++;
            if (count == (uint64_t)n) return (seq_t)m;
        }
        prev = cur;
    }
}
