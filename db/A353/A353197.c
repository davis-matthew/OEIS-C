/*
 * A353197  Numbers k such that k + s + k*s is prime, where s is the sum of
 *          digits of k.
 * https://oeis.org/A353197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer k such that k + s + k*s is prime, where
 *            s = digit_sum(k)
 * Range:     u8 n <= 2, u16 n <= 1780, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9704 (2026-11-03)
 * Helpers:   digit_sum, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;
    static idx_t next_k = 1;

    /* Ensure cache has enough capacity */
    if (cache == NULL)
    {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n)
    {
        seq_t k = (seq_t)next_k;
        seq_t s = digit_sum(k);
        seq_t p = k + s + k * s;

        if (isPrimeGeneric(p))
        {
            if (cache_len == cache_cap)
            {
                cache_cap *= 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = k;
        }
        next_k++;
    }

    return cache[n - 1];
}
