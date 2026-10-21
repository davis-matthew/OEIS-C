/*
 * A268066  Even numbers coprime to the number of their divisors.
 * https://oeis.org/A268066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 82, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268066: even numbers coprime to the number of their divisors
   (output is the square of the qualifying even numbers).
   Algorithm: scan even integers, test the coprimality condition
   by factorising via a smallest‑prime‑factor table, cache the results
   in the order requested. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"
#include "../utils/seq_realloc.h"

static idx_t *spf = NULL;          /* smallest prime factor table */
static idx_t spf_limit = 0;        /* current table size limit */
static size_t cache_cap = 0;       /* allocated capacity of cache array */
static size_t cache_size = 0;      /* number of stored terms */
static seq_t *cache = NULL;        /* cached terms */
static idx_t next_even = 2;        /* next even candidate to test */

/* initialise the SPF table once, with a modest starting limit */
static void init_spf(void)
{
    if (spf != NULL)
        return;                       /* already initialised */
    idx_t limit = 200000;              /* enough for the first 10k terms. */
    build_spf(limit, &spf);
    spf_limit = limit;
}

/* enlarge the SPF table to cover at least 'limit'.  The new table
   replaces the old one and frees its memory. */
static void ensure_spf(idx_t limit)
{
    if (spf_limit >= limit)
        return;
    free(spf);
    build_spf(limit, &spf);
    spf_limit = limit;
}

/* add the next qualifying even number to the cache if it has not
   already been stored, otherwise simply increment next_even. */
static void cache_next(void)
{
    /* make sure the SPF table is large enough for the current candidate */
    if (next_even > spf_limit)
        ensure_spf(next_even * 2 + 1);   /* a small safety margin */

    /* factorise the candidate using the SPF table */
    idx_t x = next_even;
    idx_t primes[64];
    uint16_t exps[64];
    size_t pc = 0;
    while (x > 1) {
        idx_t p = spf[x];
        uint16_t a = 0;
        while (x % p == 0) {
            x /= p;
            ++a;
        }
        primes[pc] = p;
        exps[pc] = a;
        ++pc;
    }

    /* test the coprimality condition:
       for every prime factor p_j of m, p_j must not divide any (2*e_i+1). */
    bool ok = true;
    for (size_t i = 0; ok && i < pc; ++i) {
        for (size_t j = 0; j < pc; ++j) {
            idx_t val = 2 * exps[j] + 1;   /* odd integer */
            if (val % primes[i] == 0) {
                ok = false;
                break;
            }
        }
    }

    if (ok) {
        /* store m^2 in the cache, growing the array if needed */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            if (!tmp)
                exit(1);                    /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_size++] = (seq_t)next_even * (seq_t)next_even;
    }
    /* prepare for the next candidate */
    next_even += 2;
}

/* extend the cache until it contains at least 'n' terms */
static void fill_cache(size_t n)
{
    if (cache_size >= n)
        return;
    init_spf();
    while (cache_size < n) {
        cache_next();
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* undefined for the offset < 1 */
    fill_cache((size_t)n);
    return cache[n - 1];
}
