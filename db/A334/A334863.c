/*
 * A334863  a(n) = A064097(A003961(n)).
 * https://oeis.org/A334863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, changed
 * Formula:   a(n) = A064097(A003961(n)); completely additive: a(1) = 0,
 *            a(prime(k)) = A064097(prime(k+1)), A064097(p) = 1 + A064097(p-1)
 *            for prime p, A064097(1) = 0
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 110 terms through n = 9997 (2026-11-08)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static uint64_t A064097_f(uint64_t m);

static uint64_t A064097_fp(uint64_t p) {
    return 1 + A064097_f(p - 1);
}

static uint64_t A064097_f(uint64_t m) {
    uint64_t res = 0;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            res += e * A064097_fp(p);
        }
    }
    if (m > 1) res += A064097_fp(m);
    return res;
}

seq_t a(idx_t n) {
    uint64_t m = n;
    uint64_t res = 0;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            res += e * A064097_fp(next_prime(p));
        }
    }
    if (m > 1) res += A064097_fp(next_prime(m));
    return (seq_t)res;
}
