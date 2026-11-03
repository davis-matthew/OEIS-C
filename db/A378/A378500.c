/*
 * A378500  a(1) = 2, then a(n) = a(n-1) - 2 for n even, a(n) = a(n-1) + 3 for
 *          n an odd prime or odd prime power, and a(n) = a(n-1) + 2
 *          otherwise.
 * https://oeis.org/A378500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1486, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9862 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 2;
   a(n) = a(n‑1) – 2            if n is even;
   a(n) = a(n‑1) + 3            if n is an odd prime or odd prime power;
   a(n) = a(n‑1) + 2            otherwise. */

/* Necessary standard headers */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* ========== Helper: test whether an odd n ≥ 3 is an odd prime or odd prime power ========== */
static bool is_prime_or_power_odd(idx_t n)
{
    /* n is odd and n ≥ 3 */
    for (idx_t d = 3; d * d <= n; d += 2)
    {
        if (n % d == 0)
        {
            /* d is the smallest odd divisor; factor out all copies of d */
            idx_t tmp = n;
            while (tmp % d == 0)
                tmp /= d;
            return tmp == 1;          /* True iff n = d^k */
        }
    }
    /* No divisor found → n is an odd prime */
    return true;
}

/* ========== Memoised cache for sequence values ========== */
static seq_t *cache = NULL;        /* array, 1‑based (cache[0] unused) */
static size_t cache_cap = 0;       /* current capacity of cache */
static idx_t  cache_max = 0;       /* largest index already computed */

/* Ensure cache can hold indices 1 … n */
static void grow_cache(idx_t n)
{
    /* Need capacity at least n+1 (index 0 unused) */
    if (cache_cap >= (size_t)(n + 1))
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < (size_t)(n + 1))
        new_cap <<= 1;

    seq_t *new_cache = mem_new(seq_t, new_cap);
    if (!new_cache)
        return;   /* allocation failed – unlikely in normal use */

    /* copy old data */
    for (size_t i = 0; i < cache_cap; ++i)
        new_cache[i] = cache[i];

    /* free old cache */
    mem_free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n<1, but safe */

    grow_cache(n);

    if (n > cache_max)
    {
        /* compute missing terms incrementally */
        for (idx_t i = cache_max + 1; i <= n; ++i)
        {
            if (i == 1)
            {
                cache[i] = (seq_t)2;
            }
            else
            {
                seq_t prev = cache[i - 1];
                if ((i & 1) == 0)          /* i even */
                {
                    cache[i] = prev - (seq_t)2;
                }
                else                       /* i odd */
                {
                    if (is_prime_or_power_odd(i))
                        cache[i] = prev + (seq_t)3;
                    else
                        cache[i] = prev + (seq_t)2;
                }
            }
        }
        cache_max = n;
    }

    return cache[n];
}
