/*
 * A316792  a(n) is the least prime p such that the second forward difference
 *          of three consecutive primes p, q and r is n = -(p - 2q + r)/2.
 * https://oeis.org/A316792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   next_prime, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A316792: a(n) = the least prime p such that
   n = -(p − 2q + r)/2 for the three consecutive primes p,q,r. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/next_prime.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static void ensure_size(seq_t **array, size_t *cap, size_t need)
{
    if (need <= *cap) return;
    size_t new_cap = need;
    seq_t *tmp = seq_realloc(*array, *cap, new_cap);
    if (!tmp) abort();
    for (size_t i = *cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
    *array = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* static state shared across calls */
    static seq_t *cache = NULL;          /* cache[i] = a(i) or 0 if unknown */
    static size_t cache_size = 0;        /* allocated size */
    static idx_t max_seen_idx = (idx_t)-1; /* largest n we have processed */
    static uint64_t p1 = 0, p2 = 0, p3 = 0; /* current consecutive primes */

    /* initialise on first invocation */
    if (cache == NULL) {
        cache_size = 1;                 /* we need at least index 0 */
        cache = seq_alloc(cache_size);   /* zero initialised */
        if (!cache) abort();
        cache[0] = (seq_t)3;             /* a(0) = 3, triple {3,5,7} */
        p1 = 3; p2 = 5; p3 = 7;          /* start window */
        max_seen_idx = 0;
    }

    /* expand cache so that it contains index n at least */
    ensure_size(&cache, &cache_size, n + 1);

    /* count missing values among 0..n */
    idx_t missing = 0;
    for (idx_t i = 0; i <= n; ++i)
        if (cache[i] == (seq_t)0) ++missing;

    /* walk primes until all required indices are found */
    while (missing > 0) {
        uint64_t next = next_prime(p3);
        p1 = p2;
        p2 = p3;
        p3 = next;

        /* compute sfd: -(p1 - 2*p2 + p3)/2  */
        long long diff = (long long)p1 - 2LL * (long long)p2 + (long long)p3;
        if (diff % 2 != 0) continue;          /* should not happen, but safe */
        long long sfd = -diff / 2LL;           /* sfd value */
        if (sfd < 0) continue;                /* we only store non‑negative n */

        idx_t idx = (idx_t)sfd;
        if (idx >= cache_size) {
            size_t new_size = (size_t)idx + 1;
            ensure_size(&cache, &cache_size, new_size);
        }
        if (cache[idx] == (seq_t)0) {
            cache[idx] = (seq_t)p1;            /* first prime producing this sfd */
            if (idx <= n) --missing;          /* filled one of the required slots */
        }
    }

    max_seen_idx = (n > max_seen_idx) ? n : max_seen_idx;
    return cache[n];
}
