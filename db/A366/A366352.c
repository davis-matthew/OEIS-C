/*
 * A366352  Lesser of 2 successive primes (k, k+4) sandwiching 3 consecutive
 *          nonsquarefree numbers.
 * https://oeis.org/A366352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth prime p such that p+4 is prime and * the three
 *            consecutive numbers p+1, p+2, p+3 * are all nonsquarefree
 * Range:     u8 n <= 1, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime, is_squarefree, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Convenient utilities supplied by the build system */
#include "../utils/is_squarefree.h"   /* bool is_squarefree(seq_t x); */
#include "../utils/is_prime.h"        /* bool is_prime(idx_t x); */
#include "../utils/next_prime.h"      /* uint64_t next_prime(uint64_t start); */

/* ---------- Static state for memoisation ---------- */
static seq_t *cache = NULL;          /* array of computed terms   */
static size_t cache_len = 0;         /* how many terms are stored */
static size_t cache_cap = 0;         /* current allocation size   */
static uint64_t search_start = 2;    /* next number to examine as a possible prime */

/* Ensure the cache can hold 'need' terms. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap * 2U : 16U;
    while (newcap < need) newcap <<= 1;
    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache_cap = newcap;
}

/* Search for the next qualifying prime.  Caller should call this
 * only when more terms are needed.  It advances the global
 * search_start marker past the prime that it finds. */
static uint64_t find_next_candidate(void)
{
    uint64_t p = search_start;
    for (;;)
    {
        /* Advance to the next prime >= p. */
        p = next_prime(p);

        /* The twin‑prime condition: p+4 must also be prime. */
        if (!is_prime(p + 4))
        {
            ++p;            /* skip this prime and retry */
            continue;
        }

        /* Verify that p+1, p+2, p+3 are all nonsquarefree. */
        if (is_squarefree((seq_t)(p + 1))) { ++p; continue; }
        if (is_squarefree((seq_t)(p + 2))) { ++p; continue; }
        if (is_squarefree((seq_t)(p + 3))) { ++p; continue; }

        /* Found a valid candidate. */
        search_start = p + 1;   /* start next search after this prime */
        return p;
    }
}

/* ------------------------------------------------------------------ */
/* Public sequence function                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* offset 1 but harmless if called with 0 */
    ensure_cache_capacity((size_t)n);
    while (cache_len < (size_t)n)
    {
        uint64_t candidate = find_next_candidate();
        cache[cache_len++] = (seq_t)candidate;
    }
    return cache[(size_t)n - 1];
}
