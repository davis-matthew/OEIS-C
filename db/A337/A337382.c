/*
 * A337382  Numbers k for which A003973(k) < 2*sigma(k).
 * https://oeis.org/A337382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k (searched in increasing order) with A003973(k) <
 *            2*sigma(k)
 * Range:     u8 n <= 135, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 91 terms through n = 9946 (2026-11-08)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"

static bool A337382_test(uint64_t k) {
    uint64_t sg = 1, sh = 1;
    uint64_t t = k;
    for (uint64_t p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            uint64_t q = next_prime(p);
            uint64_t s1 = 1, pw1 = 1, s2 = 1, pw2 = 1;
            while (t % p == 0) {
                t /= p;
                pw1 *= p; s1 += pw1;
                pw2 *= q; s2 += pw2;
            }
            sg *= s1;
            sh *= s2;
        }
    }
    if (t > 1) {
        uint64_t q = next_prime(t);
        sg *= (1 + t);
        sh *= (1 + q);
    }
    return sh < 2 * sg;
}

seq_t a(idx_t n) {
    static uint64_t c_n = 0, c_k = 0;
    uint64_t cnt, k;
    if (c_n != 0 && c_n <= (uint64_t)n) {
        cnt = c_n; k = c_k;
    } else {
        cnt = 0; k = 0;
    }
    while (cnt < (uint64_t)n) {
        k++;
        if (A337382_test(k)) cnt++;
    }
    c_n = cnt; c_k = k;
    return (seq_t)k;
}
