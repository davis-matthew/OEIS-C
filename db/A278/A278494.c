/*
 * A278494  Primes p for which there does not exist any such integer k that k
 *          - A002828(k) = p.
 * https://oeis.org/A278494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime p such that A002828(p+j) != j for j=1..4
 *            (search over primes)
 * Range:     u8 n <= 27, u16 n <= 2326, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9865 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool A278494_two_sq(uint64_t m) {
    uint64_t p;
    while (m % 2 == 0) m /= 2;
    for (p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            if (p % 4 == 3 && (e & 1)) return false;
        }
    }
    if (m > 1 && m % 4 == 3) return false;
    return true;
}

static int A278494_s(uint64_t m) {
    uint64_t r = 0;
    if (m == 0) return 0;
    while ((r + 1) * (r + 1) <= m) r++;
    if (r * r == m) return 1;
    if (A278494_two_sq(m)) return 2;
    while (m % 4 == 0) m /= 4;
    if (m % 8 == 7) return 4;
    return 3;
}

static bool A278494_isprime(uint64_t n) {
    uint64_t d;
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (d = 3; d <= n / d; d += 2)
        if (n % d == 0) return false;
    return true;
}

static uint64_t *A278494_terms = NULL;
static size_t A278494_len = 0, A278494_cap = 0;
static uint64_t A278494_last = 1;

seq_t a(idx_t n) {
    while (A278494_len < (size_t)n) {
        uint64_t p = ++A278494_last;
        int j;
        bool ok;
        if (!A278494_isprime(p)) continue;
        ok = true;
        for (j = 1; j <= 4; j++) {
            if (A278494_s(p + (uint64_t)j) == j) { ok = false; break; }
        }
        if (!ok) continue;
        if (A278494_len == A278494_cap) {
            size_t nc = A278494_cap ? A278494_cap * 2 : 64;
            A278494_terms = (uint64_t *)realloc(A278494_terms, nc * sizeof(uint64_t));
            A278494_cap = nc;
        }
        A278494_terms[A278494_len++] = p;
    }
    return (seq_t)A278494_terms[n - 1];
}
