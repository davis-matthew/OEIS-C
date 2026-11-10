/*
 * A282940  Largest non-infinitary divisor of A162643(n) having no
 *          non-infinitary divisors.
 * https://oeis.org/A282940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A372379(A162643(n)); A162643 by incremental search
 *            (tau(m) not a power of 2), A372379 multiplicative with a(p^e) =
 *            p^(2^floor(log_2(e+1))-1)
 * Range:     u8 n <= 156, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 8637 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* returns tau(x) and fills primes/exponents of x */
static uint64_t A282940_factor(uint64_t x, uint64_t *pr, unsigned *ex, size_t *cnt)
{
    uint64_t tau = 1;
    size_t c = 0;
    for (uint64_t p = 2; p * p <= x; p += (p == 2 ? 1 : 2)) {
        if (x % p == 0) {
            unsigned e = 0;
            while (x % p == 0) { x /= p; e++; }
            if (pr) { pr[c] = p; ex[c] = e; }
            c++;
            tau *= (e + 1);
        }
    }
    if (x > 1) {
        if (pr) { pr[c] = x; ex[c] = 1; }
        c++;
        tau *= 2;
    }
    if (cnt) *cnt = c;
    return tau;
}

static uint64_t A282940_term(uint64_t n)
{
    static uint64_t *cache = NULL;
    static size_t len = 0, cap = 0;
    static uint64_t cur = 1;

    while (len < n) {
        uint64_t t = A282940_factor(cur, NULL, NULL, NULL);
        if ((t & (t - 1)) != 0) {
            if (len + 1 > cap) {
                size_t nc = cap ? cap * 2 : 1024;
                uint64_t *tmp = (uint64_t *)realloc(cache, nc * sizeof(uint64_t));
                if (!tmp) abort();
                cache = tmp;
                cap = nc;
            }
            cache[len++] = cur;
        }
        cur++;
    }
    return cache[n - 1];
}

seq_t a(idx_t n)
{
    uint64_t m = A282940_term((uint64_t)n);
    uint64_t pr[64];
    unsigned ex[64];
    size_t cnt = 0;
    A282940_factor(m, pr, ex, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        unsigned k = 0;
        while (((uint64_t)2 << k) <= (uint64_t)ex[i] + 1) k++;
        uint64_t e_pow = ((uint64_t)1 << k) - 1;
        for (uint64_t j = 0; j < e_pow; ++j)
            result *= (seq_t)pr[i];
    }
    return result;
}
