/*
 * A369272  a(1) = 1; for n > 1, a(n) is the smallest positive number that has
 *          not yet appeared that is a multiple of the smallest prime that
 *          does not divide n*a(n-1).
 * https://oeis.org/A369272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) is the smallest positive number not
 *            yet appeared that is a multiple of the smallest prime that does
 *            not divide n*a(n-1)
 * Range:     u8 n <= 15, u16 n <= 965, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Utilities from the build environment */
#include "../utils/ensure_cache.h"   /* void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr); */
#include "../utils/is_prime_small.h" /* bool is_prime_small(seq_t n); */

/* --------------------------------------------------------------------- */
/* Static state ---------------------------------------------------------- */
static seq_t *cache = NULL;          /* cache of a(n) with 0-based index */
static size_t cache_cap = 0;         /* capacity of cache */

static uint8_t *seen = NULL;         /* bitmap of used integers */
static size_t seen_cap = 0;          /* size of seen bitmap in bytes */

static uint64_t *primes = NULL;      /* list of discovered primes */
static size_t primes_cnt = 0;        /* number of primes stored */

static seq_t *next_mult = NULL;      /* next multiple to try for each prime */

/* --------------------------------------------------------------------- */
/* Helper: enlarge the seen bitmap so that it can address index v          */
static void
ensure_seen_capacity(idx_t v)
{
    if ((size_t)v < seen_cap) return;

    size_t new_cap = seen_cap ? seen_cap * 2 : 1;
    while (new_cap <= (size_t)v) new_cap <<= 1;

    uint8_t *tmp = (uint8_t *)realloc(seen, new_cap);
    if (!tmp) abort();  /* allocation failure */

    /* zero newly allocated part */
    memset(tmp + seen_cap, 0, new_cap - seen_cap);

    seen = tmp;
    seen_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Helper: extend the prime list to at least 'new_cnt' entries            */
static void
extend_primes_to(size_t new_cnt)
{
    uint64_t cand = primes_cnt ? primes[primes_cnt - 1] + 1 : 2;

    while (primes_cnt < new_cnt) {
        /* find next prime */
        while (!is_prime_small((seq_t)cand)) ++cand;

        /* grow primes array */
        uint64_t *tmp = (uint64_t *)realloc(primes, (primes_cnt + 1) * sizeof(uint64_t));
        if (!tmp) abort();
        primes = tmp;
        primes[primes_cnt] = cand;

        /* grow next_mult array */
        seq_t *tmp2 = (seq_t *)realloc(next_mult,
            (primes_cnt + 1) * sizeof(seq_t));
        if (!tmp2) abort();
        next_mult = tmp2;
        next_mult[primes_cnt] = 1;  /* first unused multiple starts at 1 */

        ++primes_cnt;
        ++cand;
    }
}

/* --------------------------------------------------------------------- */
/* Sequence function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Ensure cache is large enough */
    ensure_cache(n, &cache, &cache_cap);

    /* Return cached value if already computed */
    if (cache[n - 1] != 0) return cache[n - 1];

    seq_t result;

    if (n == 1) {
        result = 1;
    } else {
        /* Compute previous term */
        seq_t prev = a(n - 1);

        /* Compute product n * a(n-1) using seq_t */
        seq_t prod = (seq_t)n * prev;

        /* Find smallest prime that does not divide prod */
        size_t p_idx = 0;
        while (true) {
            if (p_idx >= primes_cnt) extend_primes_to(p_idx + 1);
            uint64_t p = primes[p_idx];
            if (prod % (seq_t)p != 0) break;
            ++p_idx;
        }
        uint64_t prime = primes[p_idx];

        /* Find smallest unused multiple of 'prime' */
        seq_t k = next_mult[p_idx];
        seq_t cand;
        while (true) {
            cand = (seq_t)prime * k;
            ensure_seen_capacity((idx_t)cand);
            if (!seen[(idx_t)cand]) break;
            ++k;
        }

        /* Record the result */
        result = cand;
        seen[(idx_t)cand] = 1;
        next_mult[p_idx] = k + 1;
    }

    cache[n - 1] = result;
    return result;
}
