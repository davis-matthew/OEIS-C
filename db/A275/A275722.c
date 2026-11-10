/*
 * A275722  Numbers n for which A003961(n+1) < A003961(n).
 * https://oeis.org/A275722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A003961(k+1) < A003961(k); found by a
 *            sequential scan, cached
 * Range:     u8 n <= 121, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 91 terms through n = 9826 (2026-11-08)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static uint64_t A275722_shift(uint64_t m) {
    uint64_t r = 1;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            while (m % p == 0) {
                m /= p;
                r *= q;
            }
        }
    }
    if (m > 1) r *= next_prime(m);
    return r;
}

seq_t a(idx_t n) {
    static uint64_t *terms = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t m = 1;
    static uint64_t sm = 1;   /* A003961(m) */
    while (cnt < (size_t)n) {
        uint64_t sn = A275722_shift(m + 1);
        if (sn < sm) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[cnt++] = m;
        }
        sm = sn;
        m++;
    }
    return (seq_t)terms[(size_t)n - 1];
}
