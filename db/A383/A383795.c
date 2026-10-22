/*
 * A383795  Dirichlet g.f.: zeta(2*s-2) * zeta(s)^2.
 * https://oeis.org/A383795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} σ₂(gcd(d, n/d)) (OEIS A383795)
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper utilities provided by the build environment */
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b) */
#include "../utils/sigma_k.h"      /* seq_t sigma_k(idx_t m, uint8_t k) */

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* The sequence is defined for n ≥ 1; guard against accidental zero. */

    /* Step 1: factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t factor_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &factor_cnt);

    /* Step 2: generate all divisors of n */
    /* For n up to about 10 000 the number of divisors is < 300. */
    uint64_t divs[4096];
    size_t div_cnt = 1;
    divs[0] = 1;

    for (size_t i = 0; i < factor_cnt; ++i) {
        uint64_t p = primes[i];
        uint8_t  e = (uint8_t)exps[i];
        size_t base_cnt = div_cnt;
        for (size_t j = 0; j < base_cnt; ++j) {
            uint64_t base = divs[j];
            uint64_t val  = base;
            for (uint8_t k = 1; k <= e; ++k) {
                val *= p;
                divs[div_cnt++] = val;
            }
        }
    }

    /* Step 3: compute the sum over all divisors */
    seq_t sum = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t d = divs[i];
        uint64_t m = n / d;                 /* complementary divisor */
        seq_t  g_big = gcd((seq_t)d, (seq_t)m);
        idx_t  g_small = (idx_t)g_big;       /* gcd ≤ n fits in idx_t */
        seq_t  s2     = sigma_k(g_small, 2); /* σ₂(g) */
        sum += s2;
    }

    return sum;
}
