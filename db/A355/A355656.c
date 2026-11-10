/*
 * A355656  Primes p such that A355655(i) = 0, where i is the index of p in
 *          A000040.
 * https://oeis.org/A355656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime p such that the smallest b > 1 with b^(p-1) ==
 *            1 (mod p^2) is composite; computed by search
 * Range:     u8 n <= 39, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static bool A355656_isprime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A355656_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = (uint64_t)((unsigned __int128)r * b % m);
        b = (uint64_t)((unsigned __int128)b * b % m);
        e >>= 1;
    }
    return r;
}

/* true if the smallest b>1 with b^(p-1)==1 mod p^2 is composite */
static bool A355656_composite_b(uint64_t p)
{
    uint64_t m = p * p;
    for (uint64_t b = 2;; ++b) {
        if (A355656_powmod(b, p - 1, m) == 1)
            return !A355656_isprime(b);
    }
}

seq_t a(idx_t n)
{
    static uint64_t *terms = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t cand = 2;

    while (cnt < (size_t)n) {
        uint64_t p = cand++;
        if (!A355656_isprime(p)) continue;
        if (!A355656_composite_b(p)) continue;
        if (cnt == cap) {
            cap = cap ? cap * 2 : 64;
            terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            if (!terms) abort();
        }
        terms[cnt++] = p;
    }
    return (seq_t)terms[(size_t)n - 1];
}
