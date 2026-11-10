/*
 * A348943  Odd numbers k for which A348942(A064989(k)) = 1.
 * https://oeis.org/A348943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd k such that A064989(sigma(k)) divides A064989(k)
 *            (search over odd k)
 * Range:     u8 n <= 16, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-10)
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

static uint64_t A348943_prevprime(uint64_t n)
{
    uint64_t p = n - 1;
    for (;; --p)
        if (is_prime((idx_t)p)) return p;
}

static uint64_t A348943_A064989(uint64_t n)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    uint64_t res = 1;
    factor_u64(n, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        if (pr[i] == 2) continue;
        uint64_t b = A348943_prevprime(pr[i]);
        for (uint64_t e = 0; e < ex[i]; ++e) res *= b;
    }
    return res;
}

static uint64_t A348943_sigma(uint64_t n)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    uint64_t res = 1;
    factor_u64(n, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t s = 1, t = 1;
        for (uint64_t e = 0; e < ex[i]; ++e) {
            t *= pr[i];
            s += t;
        }
        res *= s;
    }
    return res;
}

seq_t a(idx_t n)
{
    static uint64_t terms[4096];
    static size_t have = 0;
    static uint64_t next_k = 1;
    if (n < 1 || n > 4096) return 0;
    while (have < (size_t)n) {
        uint64_t k = next_k;
        next_k += 2;
        uint64_t x = A348943_A064989(k);
        uint64_t y = A348943_A064989(A348943_sigma(k));
        if (x % y == 0) terms[have++] = k;
    }
    return (seq_t)terms[n - 1];
}
