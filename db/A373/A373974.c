/*
 * A373974  a(n) is the number of divisors of n that can be expressed as a sum
 *          of 2 distinct positive cubes.
 * https://oeis.org/A373974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100080, u16 n <= 100080, u32 n <= 100080,
 *            u64 n <= 100080, u128 n <= 100080, big any n
 * Validated: standard, pass, 142 terms through n = 9984 (2026-11-01)
 * Helpers:   factor_u64, icbrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): number of divisors of n that can be expressed as a sum
   of two distinct positive cubes.
   Formula: a(n) = Σ_{d|n} [d is a sum of two distinct positive cubes] */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/factor_u64.h"
#include "../utils/icbrt_u64.h"

/* ---------- test whether a number is a sum of two distinct positive cubes ---------- */
static bool _is_sum_two_cubes(uint64_t d)
{
    if (d < 9) return false;          /* smallest sum is 1³+2³ = 9 */
    uint64_t max_u = icbrt_u64(d);
    for (uint64_t u = 1; u <= max_u; ++u)
    {
        uint64_t u3 = u * u * u;
        if (u3 >= d) break;
        uint64_t rem = d - u3;
        uint64_t v = icbrt_u64(rem);
        if (v == u)            continue;   /* distinctness */
        if (v * v * v == rem)  return true;
    }
    return false;
}

/* ---------- return the sequence value a(n) --------------------------------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* not used, but keeps the function safe */

    uint64_t nn = (uint64_t)n;

    /* factorization: primes[] and exps[] hold the prime factors and exponents */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    /* generate all divisors of nn */
    size_t div_cap = 64;
    uint64_t *divs = (uint64_t *)malloc(div_cap * sizeof(uint64_t));
    if (!divs) return (seq_t)0;          /* allocation failure, return 0 */
    size_t div_cnt = 1;
    divs[0] = 1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t p_pow = 1;
        size_t old_cnt = div_cnt;
        for (int e = 0; e < (int)exps[i]; ++e)
        {
            p_pow *= p;
            for (size_t j = 0; j < old_cnt; ++j)
            {
                uint64_t val = divs[j] * p_pow;
                if (div_cnt == div_cap)
                {
                    div_cap <<= 1;
                    uint64_t *tmp = (uint64_t *)realloc(divs, div_cap * sizeof(uint64_t));
                    if (!tmp) { free(divs); return (seq_t)0; }
                    divs = tmp;
                }
                divs[div_cnt++] = val;
            }
        }
    }

    /* count divisors that are sums of two distinct positive cubes */
    size_t count = 0;
    for (size_t i = 0; i < div_cnt; ++i)
        if (_is_sum_two_cubes(divs[i])) ++count;

    free(divs);
    return (seq_t)count;
}
