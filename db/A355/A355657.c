/*
 * A355657  Primes p such that A355655(i) = 1, where i is the index of p in
 *          A000040.
 * https://oeis.org/A355657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th prime p for which the smallest b > 1 with
 *            b^(p-1) == 1 (mod p^2) is itself prime (A355655(pi(p)) = 1)
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A355657_pm(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = (uint64_t)(((unsigned __int128)r * b) % m);
        b = (uint64_t)(((unsigned __int128)b * b) % m);
        e >>= 1;
    }
    return r;
}

static bool A355657_isp(uint64_t x)
{
    uint64_t d;
    if (x < 2) return false;
    if (x < 4) return true;
    if (x % 2 == 0) return false;
    for (d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t cand = 2;

    while (cnt < (size_t)n) {
        uint64_t p = cand;
        cand = (p == 2) ? 3 : p + 2;
        if (!A355657_isp(p)) continue;
        {
            /* b may exceed p^2 (e.g. p = 2 gives b = 5); always terminates
               since b = p^2 + 1 works */
            uint64_t m = p * p, b;
            for (b = 2;; ++b)
                if (A355657_pm(b, p - 1, m) == 1) break;
            if (!A355657_isp(b)) continue;
        }
        if (cnt == cap) {
            cap = cap ? cap * 2 : 64;
            terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            if (!terms) abort();
        }
        terms[cnt++] = p;
    }
    return (seq_t)terms[(size_t)n - 1];
}
