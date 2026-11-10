/*
 * A337379  Numbers k for which A003961(k) < 2*sigma(k).
 * https://oeis.org/A337379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A003961(k) < 2*sigma(k); found by a
 *            search over k, factoring each k by trial division
 * Range:     u8 n <= 192, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 97 terms through n = 9421 (2026-11-08)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static bool A337379_test(uint64_t k) {
    unsigned __int128 s = 1, g = 1;
    uint64_t m = k;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            unsigned __int128 term = 1, pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= p;
                term += pw;
                g *= q;
            }
            s *= term;
        }
    }
    if (m > 1) {
        s *= ((unsigned __int128)m + 1);
        g *= next_prime(m);
    }
    return g < 2 * s;
}

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t k = 0;
    while (count < (uint64_t)n) {
        k++;
        if (A337379_test(k)) count++;
    }
    return (seq_t)k;
}
