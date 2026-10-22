/*
 * A341880  Number of ordered factorizations of n into 4 factors > 1.
 * https://oeis.org/A341880
 *
 * Offset:    16    (a(16) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered factorizations of n into 4 factors > 1
 *            (OEIS A341880)
 * Range:     u8 n <= 479, u16 n <= 9800, u32 n <= 9800, u64 n <= 9800,
 *            u128 n <= 9800, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"      /* factor_u64 */

/* Generate all divisors of n (including 1 and n) into out[0..*cnt-1].
   The caller must provide an array large enough to hold divisor_count_u64(n)
   elements.  cnt is set to the number of divisors produced. */
static void _generate_divisors(uint64_t n, uint64_t out[], size_t *cnt)
{
    /* Factor n.  For 64‑bit n the number of primes is <64. */
    uint64_t primes[64], exps[64];
    size_t pcnt = 0;
    factor_u64(n, primes, exps, &pcnt);

    /* Start with divisor 1. */
    size_t cur_len = 1;
    out[0] = 1U;

    /* For each prime power, multiply existing divisors by all powers of the prime. */
    for (size_t pi = 0; pi < pcnt; ++pi) {
        uint64_t p = primes[pi];
        uint64_t pow = 1U;
        size_t old_len = cur_len;
        for (uint64_t k = 1; k <= exps[pi]; ++k) {
            pow *= p;
            for (size_t j = 0; j < old_len; ++j) {
                out[cur_len++] = out[j] * pow;
            }
        }
    }
    *cnt = cur_len;
}

seq_t a(idx_t n)
{
    /* For indices below the offset the value is undefined; return 0. */
    if (n < 16) {
        return (seq_t)0;
    }

    /* Work with 64‑bit values for divisors and products. */
    uint64_t un = (uint64_t)n;

    /* Compute the total number of divisors (needed to allocate storage). */
    uint64_t primes[64], exps[64];
    size_t pcnt = 0;
    factor_u64(un, primes, exps, &pcnt);

    /* Compute divisor count as product of (exponent+1). */
    size_t divisor_cnt = 1U;
    for (size_t i = 0; i < pcnt; ++i) {
        divisor_cnt *= (size_t)(exps[i] + 1U);
    }

    /* Allocate array for all divisors. */
    uint64_t *divs = (uint64_t*)malloc(divisor_cnt * sizeof(uint64_t));
    if (divs == NULL) {            /* fallback to zero if allocation fails */
        return (seq_t)0;
    }

    size_t actual_cnt;
    _generate_divisors(un, divs, &actual_cnt);

    /* Count ordered quadruples (a,b,c,d) with a*b*c*d = n, all >1. */
    seq_t result = 0U;

    for (size_t i = 0; i < actual_cnt; ++i) {
        uint64_t a_part = divs[i];
        if (a_part <= 1ULL) continue;

        for (size_t j = 0; j < actual_cnt; ++j) {
            uint64_t b_part = divs[j];
            if (b_part <= 1ULL) continue;

            __uint128_t prod_ab = (__uint128_t)a_part * b_part;
            if (prod_ab > un) continue;
            if (un % (uint64_t)prod_ab != 0ULL) continue;

            for (size_t k = 0; k < actual_cnt; ++k) {
                uint64_t c_part = divs[k];
                if (c_part <= 1ULL) continue;

                __uint128_t prod_abc = prod_ab * c_part;
                if (prod_abc > un) continue;
                if (un % (uint64_t)prod_abc != 0ULL) continue;

                uint64_t d_part = un / (uint64_t)prod_abc;
                if (d_part > 1ULL) {
                    result += (seq_t)1;
                }
            }
        }
    }

    free(divs);
    return result;
}
