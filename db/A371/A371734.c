/*
 * A371734  Maximal length of a factorization of n into factors > 1 all having
 *          different sums of prime indices.
 * https://oeis.org/A371734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7892, u16 n <= 7892, u32 n <= 7892, u64 n <= 7892,
 *            u128 n <= 7892, big any n
 * Helpers:   build_primes_upto, dfs_feasible, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = maximal number of factors >1 of n with pairwise distinct sums
   of prime indices.  Computed by enumerating factorizations via a
   search over the multiset of prime‑index weights.   */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/dfs_feasible.h"

/* Cache of primes up to the largest n requested. */
static uint64_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;
static idx_t prime_cache_limit = 0;

/* Ensure that prime_cache contains all primes ≤ limit. */
static void ensure_prime_cache(idx_t limit)
{
    if (limit <= prime_cache_limit)
        return;
    free(prime_cache);
    prime_cache = NULL;
    prime_cache_cnt = 0;
    build_primes_upto(limit, &prime_cache, &prime_cache_cnt);
    prime_cache_limit = limit;
}

seq_t a(idx_t n)
{
    if (n <= 1)                 /* a(1) = 0 */
        return 0;

    /* Factor n into prime powers. */
    uint64_t primes[20], exps[20];
    size_t prime_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &prime_cnt);

    /* Obtain prime list up to n (cached). */
    ensure_prime_cache(n);

    /* Build the multiset of prime‑indices weighted by exponents. */
    int weights[64];            /* enough for n ≤ 10000 */
    int E = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        /* binary search for index of p in prime_cache */
        size_t lo = 0, hi = prime_cache_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (prime_cache[mid] < p)
                lo = mid + 1;
            else
                hi = mid;
        }
        /* prime_cache[lo] == p; its 1‑based index is lo+1. */
        int idx = (int)(lo + 1);

        for (size_t j = 0; j < exps[i]; ++j)
            weights[E++] = idx;
    }

    /* Pre‑compute sums of all submasks. */
    int fullMask = (1 << E) - 1;
    int *sumArray = (int *)malloc((fullMask + 1) * sizeof(int));
    sumArray[0] = 0;
    for (int m = 1; m <= fullMask; ++m) {
        int lsb = m & -m;
        int bit = __builtin_ctz(lsb);
        sumArray[m] = sumArray[m ^ lsb] + weights[bit];
    }

    /* Search for the largest feasible k. */
    int best = 0;
    for (int k = E; k >= 1; --k) {
        if (dfs_feasible(fullMask, k, 0, sumArray)) {
            best = k;
            break;
        }
    }

    free(sumArray);
    return (seq_t)best;
}
