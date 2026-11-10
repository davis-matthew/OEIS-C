/*
 * A267817  Numbers m that are divisible by A268336(m).
 * https://oeis.org/A267817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m with m divisible by A268336(m), where A268336(m) =
 *            lcm(lambda(m),m)/m = lambda(m)/gcd(lambda(m),m); found by a
 *            incremental search with cached terms
 * Range:     u8 n <= 55, u16 n <= 1771, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9898 (2026-11-07)
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

static uint64_t A267817_gcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static uint64_t A267817_lambda(uint64_t m)
{
    if (m == 1) return 1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);
    uint64_t lam = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t term;
        if (p == 2) {
            if (e == 1) term = 1;
            else if (e == 2) term = 2;
            else { term = 1; for (uint64_t k = 0; k + 2 < e; ++k) term *= 2; }
        } else {
            term = p - 1;
            for (uint64_t k = 1; k < e; ++k) term *= p;
        }
        uint64_t g = A267817_gcd(lam, term);
        lam = (lam / g) * term;
    }
    return lam;
}

static uint64_t *A267817_cache = NULL;
static size_t A267817_cap = 0;
static size_t A267817_cnt = 0;
static uint64_t A267817_next = 1;

seq_t a(idx_t n)
{
    size_t need = (size_t)n;
    if (need > A267817_cap) {
        size_t nc = A267817_cap ? A267817_cap : 1024;
        while (nc < need) nc *= 2;
        A267817_cache = (uint64_t *)realloc(A267817_cache, nc * sizeof(uint64_t));
        A267817_cap = nc;
    }
    while (A267817_cnt < need) {
        uint64_t m = A267817_next++;
        uint64_t lam = A267817_lambda(m);
        uint64_t g = A267817_gcd(lam, m);
        uint64_t q = lam / g;
        if (m % q == 0)
            A267817_cache[A267817_cnt++] = m;
    }
    return (seq_t)A267817_cache[need - 1];
}
