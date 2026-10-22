/*
 * A357863  Numbers whose prime indices do not have strictly increasing
 *          run-sums. Heinz numbers of the partitions not counted by A304428.
 * https://oeis.org/A357863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A357863: Numbers whose prime indices do not have strictly increasing
   run‑sums.  The function below searches for the n‑th such number
   (1‑based).  It builds a prime table once and then tests each integer
   in turn until n numbers have been found.  Since the terms grow
   moderately, this runs in well under 2 s for n up to ~10 000. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;          /* primes array, 1‑based index */
static size_t    primes_cnt = 0;

/* initialise the prime list once */
static void init_primes(void)
{
    if (primes != NULL)
        return;
    /* limit large enough to cover the primes needed for the first 10 000
       terms (≈2 M is sufficient). */
    const idx_t LIMIT = 2000000;
    build_primes_upto(LIMIT, &primes, &primes_cnt);
}

/* binary search for a prime in primes[0..primes_cnt-1];
   returns 1‑based index, or 0 if not found (should not happen). */
static uint32_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] == p)
            return (uint32_t)(mid + 1);
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0;
}

/* test whether m has non‑strictly increasing run‑sums of prime indices. */
static bool qualifies(uint64_t m)
{
    uint64_t n = m;
    uint64_t sums[64];
    size_t   rcnt = 0;

    /* factor n using primes up to sqrt(n) */
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= n; ++i) {
        uint64_t p = primes[i];
        if (n % p == 0) {
            uint32_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            uint32_t idx = prime_index(p);        /* 1‑based prime index */
            sums[rcnt++] = (uint64_t)idx * exp;   /* run‑sum for this index */
        }
    }

    if (n > 1) {                                 /* remaining prime factor */
        uint32_t idx = prime_index(n);
        sums[rcnt++] = (uint64_t)idx;
    }

    if (rcnt <= 1)
        return false;                            /* single run cannot violate */

    for (size_t i = 0; i + 1 < rcnt; ++i)
        if (sums[i] >= sums[i + 1])
            return true;                        /* non‑strictly increasing */

    return false;
}

/* cache of found sequence values */
static seq_t *cache      = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static idx_t  checked_up_to = 1;               /* last integer examined */

/* grow cache until it contains at least target values */
static void ensure_cache(idx_t target)
{
    if (cache_size >= (size_t)target)
        return;
    while (cache_size < (size_t)target) {
        ++checked_up_to;
        if (qualifies((uint64_t)checked_up_to)) {
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 64;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)checked_up_to;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                         /* undefined for 0, safe return */
    init_primes();                               /* prime table ready */
    ensure_cache(n);                             /* generate terms up to n */
    return cache[n - 1];
}
