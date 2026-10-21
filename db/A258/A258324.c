/*
 * A258324  Least common multiple of all n - d, where d < n and d is a divisor
 *          of n.
 * https://oeis.org/A258324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm{ n - d : d < n, d | n }
 * Range:     u8 n <= 9, u16 n <= 23, u32 n <= 59, u64 n <= 239,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

/* Computes a(n) for the OEIS sequence A258324. */
seq_t a(idx_t n)
{
    /* n = 1 has no proper divisor – the definition gives a(1) = 1. */
    if (n == 1)
        return (seq_t)1;

    /* ==== Prime factorisation of n ==== */
    /* The maximum number of distinct prime factors of a 64‑bit integer is 16. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t pf_cnt;
    factor_u64((uint64_t)n, primes, exps, &pf_cnt);

    /* ==== Generate all positive divisors of n ==== */
    /* The divisor count grows slowly for 64‑bit integers; we start with a small
       buffer and grow it if necessary. */
    size_t divs_cap = 64;
    uint64_t *divs = (uint64_t *)malloc(divs_cap * sizeof(uint64_t));
    if (!divs) /* out of memory – abort gracefully */
        return (seq_t)0;

    size_t divs_cnt = 0;
    divs[divs_cnt++] = 1;                /* divisor 1 is always present */

    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t exp = exps[i];

        size_t prev_cnt = divs_cnt;      /* divisors already known */
        uint64_t cur = 1;                /* p^e */

        for (size_t e = 1; e <= exp; ++e) {
            cur *= p;
            for (size_t j = 0; j < prev_cnt; ++j) {
                /* grow the buffer if needed */
                if (divs_cnt >= divs_cap) {
                    divs_cap *= 2;
                    uint64_t *tmp = (uint64_t *)realloc(divs,
                                                       divs_cap * sizeof(uint64_t));
                    if (!tmp) {              /* realloc failed */
                        free(divs);
                        return (seq_t)0;
                    }
                    divs = tmp;
                }
                divs[divs_cnt++] = divs[j] * cur;
            }
        }
    }

    /* ==== Compute the LCM of all (n - d) for proper divisors d ==== */
    seq_t l      = (seq_t)1;
    seq_t seq_n  = (seq_t)n;

    for (size_t i = 0; i < divs_cnt; ++i) {
        uint64_t d = divs[i];
        if (d == (uint64_t)n)          /* skip the divisor n itself */
            continue;

        seq_t diff = seq_n - (seq_t)d;
        seq_t g    = gcd(l, diff);
        l = l / g * diff;              /* lcm(l, diff) */
    }

    free(divs);
    return l;
}
