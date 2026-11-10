/*
 * A337378  Numbers k for which A003961(k) > 2*sigma(k).
 * https://oeis.org/A337378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A003961(k) > 2*sigma(k); found by search
 *            (linear scan with trial-division factoring)
 * Range:     u8 n <= 63, u16 n <= 15361, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 84 terms through n = 8755 (2026-11-08)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static bool A337378_test(uint64_t k) {
    unsigned __int128 A = 1, S = 1;
    uint64_t m = k;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            unsigned __int128 pp = 1, ss = 1;
            while (m % p == 0) {
                m /= p;
                A *= q;
                pp *= p;
                ss += pp;
            }
            S *= ss;
        }
    }
    if (m > 1) {
        A *= next_prime(m);
        S *= (unsigned __int128)(m + 1);
    }
    return A > 2 * S;
}

seq_t a(idx_t n) {
    static uint64_t last_n = 0, last_k = 0;
    uint64_t cnt, k;
    if (last_n != 0 && (uint64_t)n >= last_n) {
        cnt = last_n;
        k = last_k;
    } else {
        cnt = 0;
        k = 0;
    }
    while (cnt < (uint64_t)n) {
        k++;
        if (A337378_test(k)) cnt++;
    }
    last_n = cnt;
    last_k = k;
    return (seq_t)k;
}
