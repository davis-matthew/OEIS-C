/*
 * A341860  a(n) is the least k such that A341857(k) = psi(k^2)/k = n, or 0 if
 *          no such k exists.
 * https://oeis.org/A341860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that psi(k^2)/k = n (psi = A002322,
 *            Carmichael lambda); computed by incremental search over k
 * Range:     u8 n <= 16, u16 n <= 166, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Validated: standard, pass, 74 terms through n = 498 (2026-11-10)
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

static uint64_t A341860_ratio(uint64_t k)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(k, primes, exps, &cnt);
    uint64_t l = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        unsigned e = (unsigned)exps[i];
        uint64_t val;
        if (p == 2) {
            if (e == 1) val = 2;
            else val = (uint64_t)1 << (2 * e - 2);
        } else {
            val = p - 1;
            for (unsigned j = 0; j < 2 * e - 1; ++j) val *= p;
        }
        uint64_t a = l, b = val;
        while (b) { uint64_t t = a % b; a = b; b = t; }
        l = (l / a) * val;
    }
    return l / k;
}

#define A341860_TAB 4097

seq_t a(idx_t n)
{
    static uint64_t first[A341860_TAB];
    static uint64_t scanned = 0;

    if (n == 0) return 0;
    if (n < A341860_TAB) {
        while (first[n] == 0) {
            ++scanned;
            uint64_t v = A341860_ratio(scanned);
            if (v < A341860_TAB && first[v] == 0) first[v] = scanned;
        }
        return (seq_t)first[n];
    }
    for (uint64_t k = 1;; ++k) {
        if (A341860_ratio(k) == (uint64_t)n) return (seq_t)k;
    }
}
