/*
 * A329589  Odd integers which are not a positive power of a single prime and
 *          have at least one prime divisor 1 (mod 4).
 * https://oeis.org/A329589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   has_single_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A329589: Odd integers which are not a positive power of a single prime
   and have at least one prime divisor ≡ 1 (mod 4).
   The function generates terms on demand and caches them, so calls may come
   in any order.  It uses simple trial‑division to test each odd candidate
   until the required number of terms has been found. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_single_prime_factor.h"

/* Helper: determine whether the odd integer n has at least one prime divisor ≡ 1 (mod 4)
   and at least two distinct prime divisors. */
static bool valid_candidate(uint64_t n)
{
    if (has_single_prime_factor(n))
        return false;            /* prime power – reject */

    bool has1mod4 = false;
    uint64_t temp = n;
    for (uint64_t p = 3; p * p <= temp; p += 2)
    {
        if (temp % p == 0)
        {
            if (p % 4 == 1)
                has1mod4 = true;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1 && temp % 4 == 1)
        has1mod4 = true;

    /* Count distinct primes via trial division and the remaining factor */
    int distinct = 0;
    temp = n;
    for (uint64_t p = 3; p * p <= temp; p += 2)
    {
        if (temp % p == 0)
        {
            distinct++;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1)
        distinct++;

    return has1mod4 && distinct >= 2;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (n < 1)
        return (seq_t)0;            /* undefined for 0, safe fallback */

    /* Extend the cache until it contains at least n terms */
    while (cache_len < n)
    {
        uint64_t cand;
        if (cache_len == 0)
            cand = 15;              /* first term (odd, composite, 1 mod 4 factor) */
        else
            cand = (uint64_t)cache[cache_len - 1] + 2; /* next odd candidate */

        while (!valid_candidate(cand))
            cand += 2;              /* skip even numbers */

        /* Grow cache if necessary */
        if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
            {
                /* Allocation failed – return 0 as a safe fallback */
                free(cache);
                cache = NULL;
                cache_len = cache_cap = 0;
                return (seq_t)0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)cand;
    }

    return cache[n - 1];
}
