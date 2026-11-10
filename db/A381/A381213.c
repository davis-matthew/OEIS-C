/*
 * A381213  Numbers k such that A381201(k) != A381203(k).
 * https://oeis.org/A381213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k >= 1 with A381201(k) != A381203(k), where both are
 *            taken over the set of distinct bases and exponents in k's
 *            factorization (product vs lcm); computed by a linear scan with
 *            trial-division factoring
 * Range:     u8 n <= 11, u16 n <= 3142, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9588 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t A381213_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static bool A381213_differs(uint64_t k)
{
    uint64_t vals[128];
    size_t sz = 0;
    uint64_t m = k;
    uint64_t p = 2;
    while (p * p <= m) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            uint64_t add[2] = { p, e };
            for (int t = 0; t < 2; ++t) {
                bool ex = false;
                for (size_t j = 0; j < sz; ++j)
                    if (vals[j] == add[t]) { ex = true; break; }
                if (!ex) vals[sz++] = add[t];
            }
        }
        p += (p == 2) ? 1 : 2;
    }
    if (m > 1) {
        uint64_t add[2] = { m, 1 };
        for (int t = 0; t < 2; ++t) {
            bool ex = false;
            for (size_t j = 0; j < sz; ++j)
                if (vals[j] == add[t]) { ex = true; break; }
            if (!ex) vals[sz++] = add[t];
        }
    }
    uint64_t prod = 1, l = 1;
    for (size_t i = 0; i < sz; ++i) {
        prod *= vals[i];
        uint64_t g = A381213_gcd(l, vals[i]);
        l = (l / g) * vals[i];
    }
    return prod != l;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t next_k = 1;

    while (cnt < (size_t)n) {
        uint64_t k = next_k++;
        if (A381213_differs(k)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[cnt++] = k;
        }
    }
    return (seq_t)terms[(size_t)n - 1];
}
