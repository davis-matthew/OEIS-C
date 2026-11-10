/*
 * A269861  Numbers n such that n and A048673(n) are of opposite parity.
 * https://oeis.org/A269861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m >= 1 such that m and A048673(m) have opposite
 *            parity; A048673(m) = (A003961(m)+1)/2, parity found from
 *            A003961(m) mod 4 (search/scan)
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 8830 (2026-11-07)
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

static uint64_t *A269861_terms = NULL;
static size_t A269861_cnt = 0, A269861_cap = 0;
static uint64_t A269861_next = 1;

/* parity of A048673(m): returns 1 if odd */
static int A269861_b_odd(uint64_t m)
{
    if (m == 1) return 1; /* A048673(1)=1 */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);
    unsigned x = 1;
    for (size_t i = 0; i < cnt; ++i) {
        unsigned np = (unsigned)(next_prime(primes[i]) & 3u);
        for (uint64_t k = 0; k < exps[i]; ++k)
            x = (x * np) & 3u;
    }
    return x == 1; /* (X+1)/2 odd iff X = 1 mod 4 */
}

seq_t a(idx_t n)
{
    while (A269861_cnt < (size_t)n) {
        uint64_t m = A269861_next++;
        int mo = (int)(m & 1u);
        if (A269861_b_odd(m) != mo) {
            if (A269861_cnt == A269861_cap) {
                A269861_cap = A269861_cap ? A269861_cap * 2 : 1024;
                A269861_terms = (uint64_t *)realloc(A269861_terms, A269861_cap * sizeof(uint64_t));
            }
            A269861_terms[A269861_cnt++] = m;
        }
    }
    return (seq_t)A269861_terms[(size_t)n - 1];
}
