/*
 * A398569  Odd numbers of the form p^2*s, where p is prime, s is squarefree,
 *          gcd(p,s) = 1, and A001221(s) >= 2.
 * https://oeis.org/A398569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   odd numbers of the form p^2*q1*q2*...*qr, r≥2, where
 *            p,q1,q2,…,qr are distinct odd primes. The function uses a sieve
 *            based on smallest prime factors to test each odd number and
 *            keeps a cache of the terms found
 * Range:     u8 none, u16 n <= 2346, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_spf.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;        /* cached terms */
static size_t cache_len = 0;       /* number of cached terms */
static size_t cache_cap = 0;       /* allocated capacity */
static idx_t current_limit = 0;    /* highest number processed */

/* Test if num satisfies the OEIS definition, using the precomputed
   smallest prime factor array spf.  */
static bool qualifies(idx_t num, idx_t *spf)
{
    idx_t n = num;
    bool seen_square = false;
    int squarefree_cnt = 0;

    while (n > 1) {
        idx_t p = spf[n];
        int e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }

        if (e == 1) {
            ++squarefree_cnt;
        } else if (e == 2) {
            if (seen_square) return false;   /* second squared prime */
            seen_square = true;
        } else {                               /* exponent > 2 */
            return false;
        }
    }

    return seen_square && squarefree_cnt >= 2;
}

/* Extend the cache until it contains at least n terms.  */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n) return;

    idx_t limit = current_limit ? current_limit : 20;

    /* Grow the sieve limit until enough terms are found. */
    while (cache_len < n) {
        if (current_limit == 0) {               /* first run – allocate cache */
            cache_cap = 64;
            cache     = seq_alloc(cache_cap);
        }

        /* Obtain smallest prime factor array up to 'limit'. */
        idx_t *spf;
        build_spf(limit, &spf);

        /* Scan odd numbers from 'start' to 'limit'. */
        idx_t start = (current_limit == 0) ? 3 : current_limit + 1;
        for (idx_t m = start; m <= limit; m += 2) {
            if (qualifies(m, spf)) {
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    cache = seq_realloc(cache, cache_cap, new_cap);
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)m;
            }
        }
        free(spf);            /* free the SPF array */

        current_limit = limit;
        limit *= 2;           /* double the bound for the next pass */
    }
}

/* Return the n-th term of sequence A398569. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 just in case */

    ensure_cache(n);
    return cache[n - 1];
}
