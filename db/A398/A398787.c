/*
 * A398787  Even numbers m such that A001221(m) >= 3 and A001222(m) =
 *          A001221(m) + 1.
 * https://oeis.org/A398787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A398787 – even numbers m with A001221(m) ≥ 3 and
 *  A001222(m) = A001221(m) + 1.
 *  Implementation uses a growing sieve of smallest prime factors (SPF)
 *  and a cache of generated terms.  The cache is expanded only as
 *  many terms as are requested, so subsequent calls are fast.
 *
 *  The algorithm:
 *    • Keep a static cache array `cache[]` holding terms that have
 *      already been found.
 *    • Keep a static array `spf[]` containing the smallest prime factor
 *      for every integer up to the current limit.
 *    • For each new term requested, extend the SPF array if necessary,
 *      then scan successive integers until a qualifying number is
 *      found; add it to the cache.
 *
 *  The code is valid C11 and C++17.  Casts from `void *` to pointer
 *  types are done explicitly with C‑style casts, which are accepted
 *  by both C and C++.  No features such as templates or `new` are used.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_spf.h"   /* void build_spf(idx_t limit, idx_t **spf_out); */

static seq_t *cache      = NULL;   /* cached sequence values          */
static size_t cache_cap  = 0;      /* allocated capacity of cache      */
static idx_t  cache_len  = 0;      /* number of values stored in cache */

static idx_t  spf_limit = 0;       /* current upper bound of SPF array */
static idx_t *spf       = NULL;    /* smallest prime factor table      */

static idx_t last_m = 1;           /* last integer examined            */

/* --------------------------------------------------------------------- */
/* Expand the SPF table so that indices up to `limit` are valid.         */
/* --------------------------------------------------------------------- */
static void
ensure_spf(idx_t limit)
{
    if (limit <= spf_limit)
        return;
    if (spf != NULL)
        free(spf);
    build_spf(limit, &spf);
    spf_limit = limit;
}

/* --------------------------------------------------------------------- */
/* Grow the cache array if necessary and append a new value.             */
/* --------------------------------------------------------------------- */
static void
append_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* program cannot continue           */
        cache     = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* --------------------------------------------------------------------- */
/* Test whether integer `m` satisfies the definition of the sequence.   */
/* --------------------------------------------------------------------- */
static bool
is_term(idx_t m)
{
    idx_t distinct  = 0;   /* number of distinct prime factors   */
    idx_t squarecnt = 0;   /* how many primes appear with exp 2  */
    bool   has2      = false; /* does 2 appear as a factor?        */

    while (m > 1) {
        idx_t p = spf[m];
        idx_t exp = 0;
        do {
            m /= p;
            ++exp;
        } while (m % p == 0);

        ++distinct;
        if (exp > 2)            /* a prime exponent > 2 disqualifies */
            return false;
        if (exp == 2) {
            ++squarecnt;
            if (squarecnt > 1)   /* more than one squared prime */
                return false;
        }
        if (p == 2)
            has2 = true;
    }
    /* At least three distinct primes, exactly one squared prime, and 2 present. */
    return (distinct >= 3) && (squarecnt == 1) && has2;
}

/* --------------------------------------------------------------------- */
/* Return the n‑th term of sequence A398787 (1‑based index).             */
/* --------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for index 0                 */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        /* Ensure SPF coverage for the next candidate. */
        idx_t next_m = last_m + 1;
        ensure_spf(next_m + 100);  /* add a small slack            */

        /* Find the next qualifying integer. */
        while (!is_term(next_m)) {
            ++next_m;
            if (next_m > spf_limit)
                ensure_spf(next_m + 100);
        }

        last_m = next_m;
        append_cache((seq_t)next_m);
    }

    return cache[n - 1];
}
