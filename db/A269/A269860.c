/*
 * A269860  Numbers n such that n and A048673(n) are of the same parity.
 * https://oeis.org/A269860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m >= 1 with m == A048673(m) (mod 2), where
 *            A048673(m) = (A003961(m)+1)/2; found by a cached linear scan
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 8217 (2026-11-07)
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

static uint64_t *A269860_terms = NULL;
static size_t A269860_cnt = 0;
static size_t A269860_cap = 0;
static uint64_t A269860_next = 1;

/* parity of A048673(m): A003961(m) is odd; A048673 is odd iff A003961 == 1 mod 4 */
static int A269860_par(uint64_t m)
{
    if (m == 1) return 1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);
    uint64_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t np = next_prime(primes[i]) & 3u;
        for (uint64_t k = 0; k < exps[i]; ++k)
            prod = (prod * np) & 3u;
    }
    return prod == 1 ? 1 : 0;
}

seq_t a(idx_t n)
{
    size_t need = (size_t)n;
    while (A269860_cnt < need) {
        uint64_t m = A269860_next++;
        if ((int)(m & 1u) == A269860_par(m)) {
            if (A269860_cnt == A269860_cap) {
                size_t nc = A269860_cap ? A269860_cap * 2 : 1024;
                uint64_t *p = (uint64_t *)realloc(A269860_terms, nc * sizeof(uint64_t));
                if (!p) return (seq_t)0;
                A269860_terms = p;
                A269860_cap = nc;
            }
            A269860_terms[A269860_cnt++] = m;
        }
    }
    return (seq_t)A269860_terms[need - 1];
}
