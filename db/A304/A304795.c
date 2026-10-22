/*
 * A304795  Number of positive special sums of the integer partition with
 *          Heinz number n.
 * https://oeis.org/A304795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9110, u16 n <= 9110, u32 n <= 9110, u64 n <= 9110,
 *            u128 n <= 9110, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of positive special sums of the integer partition with Heinz number n. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto */

static uint64_t *primes = NULL;      /* global prime table */
static size_t  prime_count = 0;      /* number of primes in primes[] */

/* Ensure primes[] contains all primes <= limit. */
static void ensure_primes_upto(idx_t limit)
{
    if (prime_count && primes[prime_count - 1] >= limit)
        return;                      /* already sufficient */

    free(primes);
    prime_count = 0;
    build_primes_upto(limit, &primes, &prime_count);
}

/* Return the 1‑based index of prime p in the global primes[] table. */
static idx_t prime_index(uint64_t p)
{
    /* linear search is fine for the small tables involved (n≤10⁴). */
    for (idx_t i = 0; i < prime_count; ++i)
        if (primes[i] == p)
            return i + 1;
    return 0;                       /* should never happen */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1)          /* empty partition -> no positive sums */
        return 0;

    /* Build prime table up to n to factorize and to get prime indices. */
    ensure_primes_upto(n);

    /* Store distinct primes and their exponents. */
    struct { idx_t idx; idx_t exp; } parts[64];
    size_t partcnt = 0;

    idx_t temp = n;
    for (idx_t i = 0; i < prime_count &&
                     (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i)
    {
        uint64_t p = primes[i];
        if (temp % p == 0)
        {
            idx_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            parts[partcnt].idx = i + 1;   /* 1‑based prime index */
            parts[partcnt].exp = exp;
            ++partcnt;
        }
    }
    if (temp > 1)                /* remaining prime factor > sqrt(original n) */
    {
        parts[partcnt].idx = prime_index(temp);
        parts[partcnt].exp = 1;
        ++partcnt;
    }

    /* Compute maximum possible subset sum (sum of all indices). */
    idx_t maxSum = 0;
    for (size_t i = 0; i < partcnt; ++i)
        maxSum += parts[i].idx * parts[i].exp;

    size_t len = (size_t)maxSum + 1;
    uint32_t *cnt = (uint32_t *)calloc(len, sizeof(uint32_t));
    uint32_t *next = (uint32_t *)calloc(len, sizeof(uint32_t));
    if (!cnt || !next)
    {
        free(cnt);
        free(next);
        return 0;                 /* memory failure -> return 0 */
    }

    cnt[0] = 1;                   /* empty subset */

    /* Process each distinct part (index with multiplicity). */
    for (size_t pi = 0; pi < partcnt; ++pi)
    {
        idx_t idx = parts[pi].idx;
        idx_t exp = parts[pi].exp;

        /* clear next array */
        for (size_t i = 0; i < len; ++i)
            next[i] = 0;

        for (size_t s = 0; s < len; ++s)
        {
            uint32_t val = cnt[s];
            if (!val)
                continue;
            for (idx_t k = 0; k <= exp; ++k)
            {
                size_t ns = s + k * idx;
                if (ns >= len)
                    break;
                next[ns] += val;
            }
        }

        /* swap cnt and next for next iteration */
        uint32_t *tmp = cnt;
        cnt = next;
        next = tmp;
    }

    /* Count special sums: positive sums with exactly one subset. */
    uint32_t specials = 0;
    for (size_t s = 1; s < len; ++s)
        if (cnt[s] == 1)
            ++specials;

    free(cnt);
    free(next);
    return (seq_t)specials;
}
