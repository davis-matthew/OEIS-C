/*
 * A318572  Squarefree numbers A005117(k) whose largest prime factor is not
 *          A318411(k).
 * https://oeis.org/A318572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th squarefree m >= 2 (m = A005117(k)) with A073482(k)
 *            != 1 + lcm{p-1 : p | m} (A318411(k)); found by search with trial
 *            factorization
 * Range:     u8 n <= 27, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 72 terms through n = 4446 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t a318572_gcd(uint64_t x, uint64_t y) {
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

/* returns 0 if not squarefree, 1 if squarefree and lpf == 1+lcm(p-1), 2 if squarefree and differs */
static int a318572_classify(uint64_t m) {
    uint64_t lcm = 1, lpf = 1, r = m;
    for (uint64_t p = 2; p * p <= r; p++) {
        if (r % p == 0) {
            r /= p;
            if (r % p == 0) return 0;
            uint64_t q = p - 1;
            lcm = (lcm / a318572_gcd(lcm, q)) * q;
            lpf = p;
        }
    }
    if (r > 1) {
        uint64_t q = r - 1;
        lcm = (lcm / a318572_gcd(lcm, q)) * q;
        lpf = r;
    }
    return (lpf == lcm + 1) ? 1 : 2;
}

seq_t a(idx_t n) {
    static uint64_t *cache = 0;
    static idx_t size = 0, cap = 0;
    static uint64_t m = 1;

    if (n > size) {
        if (n > cap) {
            idx_t nc = cap ? cap : 64;
            while (nc < n) nc *= 2;
            cache = (uint64_t *)realloc(cache, (size_t)nc * sizeof(uint64_t));
            cap = nc;
        }
        while (size < n) {
            m++;
            if (a318572_classify(m) == 2)
                cache[size++] = m;
        }
    }
    return (seq_t)cache[n - 1];
}
