/*
 * A377736  Numbers k such that k, k+1 and k+2 are all terms in A377732.
 * https://oeis.org/A377736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, bref
 * Formula:   a(n) = k such that k, k+1, k+2 are all in A377732; computed by a
 *            segmented sieve over candidates k = d*(m^2-d) (d <= m^2-d), then
 *            validated by factorisation
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: partial, pass, 3 terms through n = 3 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "../utils/factor_u64.h"

static uint64_t A377736_isqrt(uint64_t n)
{
    uint64_t r = (uint64_t)__builtin_sqrt((double)n);
    while (r > 0 && (__uint128_t)r * r > n) --r;
    while ((__uint128_t)(r + 1) * (r + 1) <= n) ++r;
    return r;
}

static void A377736_best_div(const uint64_t *pr, const uint64_t *ex, size_t cnt,
                             size_t idx, uint64_t cur, uint64_t limit,
                             uint64_t *best)
{
    if (idx == cnt) {
        if (cur <= limit && cur > *best) *best = cur;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t e = ex[idx];
    uint64_t val = cur;
    for (uint64_t i = 0; i <= e; ++i) {
        A377736_best_div(pr, ex, cnt, idx + 1, val, limit, best);
        if (i == e) break;
        if (val > limit / p) break;
        val *= p;
    }
}

static bool A377736_good(uint64_t k)
{
    if (k == 0) return false;
    uint64_t s = A377736_isqrt(k);
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(k, primes, exps, &cnt);
    uint64_t md = 1;
    A377736_best_div(primes, exps, cnt, 0, 1, s, &md);
    uint64_t sum = md + k / md;
    uint64_t r = A377736_isqrt(sum);
    return r * r == sum;
}

#define A377736_B ((uint64_t)1 << 26)

static uint64_t *A377736_terms = NULL;
static size_t A377736_len = 0, A377736_cap = 0;
static uint64_t A377736_L = 1;
static uint64_t *A377736_bits = NULL;

static void A377736_block(void)
{
    uint64_t L = A377736_L;
    uint64_t H2 = L + A377736_B + 2;
    size_t words = (size_t)(A377736_B / 64) + 1;
    if (!A377736_bits)
        A377736_bits = (uint64_t *)malloc(words * sizeof(uint64_t));
    memset(A377736_bits, 0, words * sizeof(uint64_t));
    uint64_t dmax = A377736_isqrt(H2 - 1);
    for (uint64_t d = 1; d <= dmax; ++d) {
        uint64_t emin = (L + d - 1) / d;
        if (emin < d) emin = d;
        uint64_t emax = (H2 - 1) / d;
        if (emin > emax) continue;
        uint64_t lo = d + emin, hi = d + emax;
        uint64_t m = A377736_isqrt(lo);
        if (m * m < lo) ++m;
        for (; m * m <= hi; ++m) {
            uint64_t k = d * (m * m - d);
            uint64_t off = k - L;
            A377736_bits[off >> 6] |= (uint64_t)1 << (off & 63);
        }
    }
    size_t nw = (size_t)(A377736_B / 64);
    for (size_t w = 0; w < nw; ++w) {
        uint64_t cur = A377736_bits[w];
        if (!cur) continue;
        uint64_t nxt = A377736_bits[w + 1];
        uint64_t w1 = (cur >> 1) | (nxt << 63);
        uint64_t w2 = (cur >> 2) | (nxt << 62);
        uint64_t t = cur & w1 & w2;
        while (t) {
            int b = __builtin_ctzll(t);
            t &= t - 1;
            uint64_t k = L + ((uint64_t)w << 6) + (uint64_t)b;
            if (A377736_good(k) && A377736_good(k + 1) && A377736_good(k + 2)) {
                if (A377736_len >= A377736_cap) {
                    A377736_cap = A377736_cap ? A377736_cap * 2 : 16;
                    A377736_terms = (uint64_t *)realloc(A377736_terms, A377736_cap * sizeof(uint64_t));
                }
                A377736_terms[A377736_len++] = k;
            }
        }
    }
    A377736_L += A377736_B;
}

seq_t a(idx_t n)
{
    while (A377736_len < (size_t)n)
        A377736_block();
    return (seq_t)A377736_terms[(size_t)n - 1];
}
