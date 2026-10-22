/*
 * A384015  Primes with at least two identical trailing digits and at least
 *          two identical leading digits.
 * https://oeis.org/A384015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 45, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A384015: Primes with at least two identical trailing digits
 * and at least two identical leading digits.
 * The sequence is computed by sieving primes up to an expanding
 * limit and selecting those that satisfy the two‑digit equality
 * property.  Term a(n) is the n‑th such prime (a(1)=11). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Utility functions supplied by the build system */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */
#include "../utils/grow_seq.h"          /* void grow_seq(seq_t **ptr, size_t *cap, size_t needed); */

#define INITIAL_LIMIT 2000000ULL       /* starting sieve limit */

/* Thread‑safe, lazily initialized cache of the sequence */
static seq_t *cache      = NULL;   /* array of found primes (in seq_t form) */
static size_t cache_len  = 0;      /* number of elements stored */
static size_t cache_cap  = 0;      /* current capacity of cache */
static uint64_t processed_limit = 0; /* largest prime limit already processed */
static bool ready = false;          /* init flag */

/* Pre‑computed powers of ten up to 10^19 (fits in uint64_t) */
static uint64_t pow10[20] = {0};

/* ------------------------------------------------------------------ */
/* Helper to append a value to the cache, growing as necessary */
static void append_to_cache(seq_t val)
{
    grow_seq(&cache, &cache_cap, cache_len + 1);
    cache[cache_len++] = val;
}

/* ------------------------------------------------------------------ */
/* Test whether a given prime satisfies the property
 * (two identical trailing digits and two identical leading digits). */
static bool is_candidate(uint64_t p)
{
    if (p == 11ULL)
        return true;                 /* 11 is a valid term */

    if (p < 100ULL)
        return false;                /* cannot have two leading digits identical */

    /* Trailing two digits identical? */
    uint64_t low = p % 100ULL;
    if ((low / 10ULL) != (low % 10ULL))
        return false;

    /* Length of the prime (all digits) */
    size_t len = 0;
    uint64_t tmp = p;
    do { len++; tmp /= 10ULL; } while (tmp != 0);

    if (len < 4)                /* need at least two leading digits */
        return false;

    /* Leading two digits identical? */
    uint64_t leading = p / pow10[len - 2];
    if ((leading / 10ULL) != (leading % 10ULL))
        return false;

    return true;
}

/* ------------------------------------------------------------------ */
/* Run the sieve up to 'limit', test all newly discovered primes and
 * store those that are candidates.  Primes <= processed_limit are
 * ignored since they were already examined. */
static void build_and_scan(uint64_t limit)
{
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;

    build_primes_upto(limit, &primes, &primes_cnt);

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p <= processed_limit)
            continue;            /* already processed */

        if (is_candidate(p))
            append_to_cache((seq_t)p);
    }

    free(primes);                /* memory allocated by build_primes_upto */
    processed_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Initialise power‑of‑ten table, and perform an initial sieve. */
static void init_seq(void)
{
    if (ready)
        return;

    pow10[0] = 1ULL;
    for (size_t i = 1; i < 20; ++i)
        pow10[i] = pow10[i - 1] * 10ULL;

    build_and_scan(INITIAL_LIMIT);

    ready = true;
}

/* ------------------------------------------------------------------ */
/* Public function requested by the exercise */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for indices < 1 */

    init_seq();

    /* Extend the sieve until we have at least 'n' terms */
    while (cache_len < (size_t)n) {
        uint64_t next_limit = processed_limit * 2ULL;
        if (next_limit == 0)      /* overflow protection */
            next_limit = INITIAL_LIMIT;
        build_and_scan(next_limit);
    }

    return cache[(size_t)n - 1];
}
