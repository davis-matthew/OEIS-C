/*
 * A357695  Cubefree abundant numbers.
 * https://oeis.org/A357695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th cubefree abundant number (search)
 * Range:     u8 n <= 15, u16 n <= 3527, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9452 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Check whether m is cubefree and abundant. */
static bool is_cubefree_abundant(uint64_t m)
{
    uint64_t temp = m;
    seq_t sigma = (seq_t)1;

    /* factor by trial division */
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p != 0)
            continue;

        uint64_t exp = 0;
        while (temp % p == 0)
        {
            temp /= p;
            ++exp;
            if (exp >= 3)
                return false;          /* not cubefree */
        }

        /* sum of powers of p: 1 + p + p^2 + ... + p^exp */
        seq_t term = (seq_t)1;
        seq_t p_power = (seq_t)1;
        for (uint64_t i = 1; i <= exp; ++i)
        {
            p_power *= (seq_t)p;
            term += p_power;
        }
        sigma *= term;
    }

    /* if remaining part > 1 it is a prime factor with exponent 1 */
    if (temp > 1)
    {
        /* sum 1 + temp */
        sigma *= (seq_t)1 + (seq_t)temp;
    }

    seq_t twice = (seq_t)2 * (seq_t)m;
    return sigma > twice;
}

/* Main sequence generator. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cnt = 0;
    static idx_t cache_cap = 0;
    static uint64_t last_checked = 1;

    /* initialise cache on first call */
    if (cache_cnt == 0)
    {
        cache_cap = 128;
        cache = mem_new(seq_t, cache_cap);
    }

    /* generate terms until we have at least n terms */
    while (cache_cnt < n)
    {
        uint64_t cand = last_checked;
        ++last_checked;

        if (is_cubefree_abundant(cand))
        {
            if (cache_cnt == cache_cap)
            {
                cache_cap *= 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_cnt++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
