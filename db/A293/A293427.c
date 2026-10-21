/*
 * A293427  Squarefree numbers such that there are no adjacent 0's in their
 *          binary expansions.
 * https://oeis.org/A293427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 3398, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   no_adjacent_zeros
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth squarefree integer whose binary representation
   contains no consecutive zeros. The function generates numbers by
   sequentially testing natural numbers, caching the results so that any
   call proceeds in O(1) time beyond the first. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/no_adjacent_zeros.h"

/* ----------  Smallest Prime Factor table  ---------- */
static uint32_t *spf = NULL;      /* array of SPF values   */
static size_t   spf_limit = 0;    /* current upper bound   */

/* Build SPF table up to 'limit' (inclusive) */
static void build_spf(size_t limit)
{
    if (spf) {
        free(spf);
    }
    spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!spf)
        abort();                  /* out of memory       */
    spf_limit = limit;
    for (size_t i = 0; i <= limit; ++i)
        spf[i] = 0;
    spf[0] = 0;
    spf[1] = 1;
    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] != 0)
            continue;             /* already marked      */
        spf[i] = (uint32_t)i;      /* i is prime         */
        if (i <= limit / i) {     /* avoid overflow      */
            for (size_t j = i * i; j <= limit; j += i)
                if (spf[j] == 0)
                    spf[j] = (uint32_t)i;
        }
    }
}

/* ----------  Test whether a number is square‑free  ---------- */
static inline bool is_squarefree(idx_t n)
{
    if (n < 2)
        return true;
    while (n > 1) {
        idx_t p = spf[n];
        if (p == 0)         /* safety for primes > spf_limit */
            p = n;
        int cnt = 0;
        while (n % p == 0) {
            n /= p;
            ++cnt;
            if (cnt > 1)
                return false;
        }
    }
    return true;
}

/* ----------  Cache for sequence values  ---------- */
static seq_t  *cache = NULL;   /* cached sequence values */
static size_t  cache_len = 0;   /* number of cached terms  */
static size_t  cache_cap = 0;   /* capacity of cache array */
static idx_t   last_checked = 0;/* last integer examined   */

/* ----------  Main sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0)                 /* below offset, undefined */
        return 0;

    /* Build SPF table once, 1 million is enough for n <= 10000 */
    if (!spf)
        build_spf(1000000u);

    /* Generate terms until we have the nth one */
    while (cache_len < n) {
        ++last_checked;
        if (!is_squarefree(last_checked))
            continue;
        if (!no_adjacent_zeros(last_checked))
            continue;

        /* Store the value, growing the cache if necessary */
        if (cache_len == cache_cap) {
            cache_cap = (cache_cap == 0) ? 1024 : cache_cap * 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!cache)
                abort();
        }
        cache[cache_len++] = (seq_t)last_checked;
    }
    return cache[n - 1];
}
