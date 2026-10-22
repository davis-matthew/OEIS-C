/*
 * A400871  The number of ordered pairs of exponential divisors (d_1, d_2) of
 *          n^2 such d_1*d_2 is also an exponential divisor of n^2.
 * https://oeis.org/A400871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy, new
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a is multiplicative. For n = ∏ p_i^{e_i},
 * a(n) = ∏ A385429(e_i),
 * where A385429(e) = |{(d1,d2) : d1|2e, d2|2e, d1+d2|2e }|.
 * That counts ordered pairs of exponential divisors of p_i^{2e_i}
 * whose product is again an exponential divisor of p_i^{2e_i}.
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Cache for A385429(e) up to 1024; eager computation on demand. */
#define CACHE_SIZE 1024
static seq_t cache_A[CACHE_SIZE];
static uint8_t cache_set[CACHE_SIZE] = {0};

/* Compute A385429(e).  e is at least 1. */
static seq_t calc_A385429(idx_t e)
{
    if (e < CACHE_SIZE && cache_set[e]) {
        return cache_A[e];
    }

    idx_t m = e << 1;           /* m = 2e */
    /* The number of divisors of m is small; 256 is plenty for e < 10^9. */
    idx_t divs[256];
    size_t dcnt = 0;

    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            divs[dcnt++] = d;
            if (d != m / d) {
                divs[dcnt++] = m / d;
            }
        }
    }

    seq_t cnt = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        for (size_t j = 0; j < dcnt; ++j) {
            idx_t sum = divs[i] + divs[j];
            if (sum <= m && m % sum == 0) {
                ++cnt;
            }
        }
    }

    if (e < CACHE_SIZE) {
        cache_A[e] = cnt;
        cache_set[e] = 1;
    }
    return cnt;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1) {
        return 1;
    }

    /* Factorize n.  Use a small fixed buffer. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t contrib = calc_A385429((idx_t)exps[i]);
        result = result * contrib;
    }

    return result;
}
