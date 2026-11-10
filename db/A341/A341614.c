/*
 * A341614  Numbers k such that sigma(k) <= 2k < A003961(k).
 * https://oeis.org/A341614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with sigma(k) <= 2k < A003961(k); found by a
 *            running search, with terms cached
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 7402 (2026-11-10)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"

static bool A341614_test(uint64_t k) {
    uint64_t m = k, sig = 1, sh = 1;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            uint64_t pw = 1, s = 1;
            while (m % p == 0) {
                m /= p;
                pw *= p;
                s += pw;
                sh *= q;
            }
            sig *= s;
        }
    }
    if (m > 1) {
        sig *= (m + 1);
        sh *= next_prime(m);
    }
    return sig <= 2 * k && 2 * k < sh;
}

seq_t a(idx_t n) {
    static uint64_t *terms = 0;
    static size_t cnt = 0, cap = 0;
    static uint64_t cur = 0;
    while (cnt < (size_t)n) {
        cur++;
        if (A341614_test(cur)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[cnt++] = cur;
        }
    }
    return (seq_t)terms[n - 1];
}
