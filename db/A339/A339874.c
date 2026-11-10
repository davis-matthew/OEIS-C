/*
 * A339874  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => f(i) = f(j), where f(n) = A052126(n) for n > 1, and f(1) = 0.
 * https://oeis.org/A339874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = 1 + A322826(n), i.e. a(n) = 1 + #{k
 *            >= 1 : first(k) <= m}, where k = A052126(n), m = first(k),
 *            first(1) = 2, first(k) = k*gpf(k) for k > 1. Computed with a
 *            sieve of greatest prime factors
 * Range:     u8 n <= 5040, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9649 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;
    uint64_t t = (uint64_t)n, g = 1;
    uint64_t p;
    for (p = 2; p * p <= t; p++) {
        while (t % p == 0) { g = p; t /= p; }
    }
    if (t > 1) g = t;
    uint64_t k = (uint64_t)n / g;
    uint64_t m;
    if (k == 1) {
        m = 2;
    } else {
        uint64_t kg = k, gk = 1, tt = k;
        for (p = 2; p * p <= tt; p++) {
            while (tt % p == 0) { gk = p; tt /= p; }
        }
        if (tt > 1) gk = tt;
        (void)kg;
        m = k * gk;
    }
    uint64_t lim = m / 2;
    uint32_t *gp = (uint32_t *)calloc((size_t)lim + 2, sizeof(uint32_t));
    if (!gp) return (seq_t)0;
    for (p = 2; p <= lim; p++) {
        if (gp[p] == 0) {
            uint64_t j;
            for (j = p; j <= lim; j += p) gp[j] = (uint32_t)p;
        }
    }
    uint64_t count = 0;
    if (m >= 2) count = 1; /* k = 1 */
    uint64_t kk;
    for (kk = 2; kk <= lim; kk++) {
        if (kk * (uint64_t)gp[kk] <= m) count++;
    }
    free(gp);
    return (seq_t)(count + 1);
}
