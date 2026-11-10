/*
 * A341031  Numbers k such that A066840(k) is a square.
 * https://oeis.org/A341031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with A066840(k) a perfect square (0 counts),
 *            where A066840(k) = sum of 1 <= j <= k/2 with gcd(j,k)=1;
 *            computed by an incremental search with an smallest-prime-factor
 *            sieve and inclusion-exclusion
 * Range:     u8 n <= 43, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Validated: standard, pass, 81 terms through n = 2459 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static uint32_t *T341031_spf = NULL;
static uint64_t T341031_lim = 0;
static uint64_t *T341031_terms = NULL;
static size_t T341031_cnt = 0, T341031_cap = 0;
static uint64_t T341031_next = 1;

static void T341031_build(uint64_t limit)
{
    uint64_t i, j;
    free(T341031_spf);
    T341031_spf = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    for (i = 2; i <= limit; ++i) {
        if (T341031_spf[i] == 0) {
            for (j = i; j <= limit; j += i)
                if (T341031_spf[j] == 0) T341031_spf[j] = (uint32_t)i;
        }
    }
    T341031_lim = limit;
}

static uint64_t T341031_val(uint64_t n)
{
    uint64_t half = n / 2;
    if (half == 0) return 0;
    uint64_t primes[16];
    int cnt = 0;
    uint64_t t = n;
    while (t > 1) {
        uint64_t p = T341031_spf[t];
        primes[cnt++] = p;
        while (t % p == 0) t /= p;
    }
    uint64_t res = 0;
    uint64_t subsets = 1ULL << cnt;
    for (uint64_t mask = 0; mask < subsets; ++mask) {
        uint64_t d = 1;
        int bits = 0;
        for (int i = 0; i < cnt; ++i)
            if (mask & (1ULL << i)) { d *= primes[i]; ++bits; }
        uint64_t m = half / d;
        if (m == 0) continue;
        uint64_t term = d * (m * (m + 1) / 2);
        if (bits & 1) res -= term; else res += term;
    }
    return res;
}

static int T341031_issq(uint64_t v)
{
    uint64_t r = (uint64_t)__builtin_sqrt((double)v);
    while (r * r > v) --r;
    while ((r + 1) * (r + 1) <= v) ++r;
    return r * r == v;
}

seq_t a(idx_t n)
{
    while (T341031_cnt < (size_t)n) {
        if (T341031_next > T341031_lim) {
            uint64_t nl = T341031_lim ? T341031_lim * 2 : 65536;
            T341031_build(nl);
        }
        uint64_t end = T341031_lim;
        while (T341031_next <= end && T341031_cnt < (size_t)n) {
            uint64_t k = T341031_next++;
            if (T341031_issq(T341031_val(k))) {
                if (T341031_cnt == T341031_cap) {
                    T341031_cap = T341031_cap ? T341031_cap * 2 : 1024;
                    T341031_terms = (uint64_t *)realloc(T341031_terms, T341031_cap * sizeof(uint64_t));
                }
                T341031_terms[T341031_cnt++] = k;
            }
        }
    }
    return (seq_t)T341031_terms[(size_t)n - 1];
}
