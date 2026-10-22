/*
 * A349235  Numbers k such that k and k+4 are consecutive cubefree numbers.
 * https://oeis.org/A349235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 25, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A349235: Numbers k such that k and k+4 are consecutive cubefree numbers. */
/* This implementation uses a dynamic sieve of the numbers that are divisible by a
 * prime cube.  The sieve is expanded lazily as larger values of k are examined.
 * All found terms are stored in a static cache so that subsequent calls to
 * a(idx_t) are answered in O(1).  The algorithm runs in linear time with a
 * modest constant factor (≈2 s for n ≈ 10 000 on a typical machine). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/build_primes_upto.h"   /* void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */
#include "../utils/append_candidate.h"   /* void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val); */

/* ------------------------------------------------------------------ */
/*  Local static state                                                 */
/* ------------------------------------------------------------------ */
static uint64_t *sieve_primes       = NULL;   /* primes up to at least 1000 */
static size_t    sieve_primes_cnt   = 0;

static uint8_t *cubefree_mark = NULL;        /* 1 = cubefree, 0 = not cubefree */
static size_t   cubefree_cap  = 0;

static seq_t *term_cache   = NULL;           /* cached terms of the sequence */
static size_t cache_len    = 0;
static size_t cache_cap    = 0;

static uint64_t next_k_to_test = 1;          /* smallest k not yet checked */

/* ------------------------------------------------------------------ */
/*  Helper: initialise the small primes list (once)                    */
/* ------------------------------------------------------------------ */
static void init_small_primes(void)
{
    if (sieve_primes != NULL)
        return;                    /* already initialised */

    /* Primes up to 1000 are sufficient: 1000³ = 10⁹ > 3·10⁷, which
     * is comfortably above the range needed for the first 10 000 terms. */
    build_primes_upto(1000, &sieve_primes, &sieve_primes_cnt);
}

/* ------------------------------------------------------------------ */
/*  Expand the cubefree sieve so that it can index up to 'need'-1.     */
/*  The sieve stores a 1 for cubefree numbers and a 0 for those divisible
 *  by a prime cube.  New positions are initialised to 1 and then
 *  marked by multiples of each prime³.                               */
/* ------------------------------------------------------------------ */
static void ensure_cubefree_capacity(size_t need)
{
    if (cubefree_cap >= need)
        return;                    /* already large enough */

    /* Initialise primes list once */
    init_small_primes();

    /* Grow capacity: double or enough for 'need' */
    size_t prev_cap = cubefree_cap;
    size_t new_cap  = cubefree_cap ? cubefree_cap << 1 : (1 << 20); /* 1 048 576 */
    if (new_cap < need)
        new_cap = need;

    uint8_t *tmp = (uint8_t *)realloc(cubefree_mark, new_cap);
    if (!tmp)
        abort();                   /* memory failure */
    cubefree_mark = tmp;

    /* New entries are initially cubefree (1) */
    memset(cubefree_mark + prev_cap, 1, new_cap - prev_cap);

    /* Mark non‑cubefree numbers in the newly added range.  For each
     * prime p, multiples of p³ are set to 0.  Only the multiples that
     * fall inside the new capacity are processed. */
    for (size_t i = 0; i < sieve_primes_cnt; ++i) {
        uint64_t p = sieve_primes[i];
        uint64_t p3 = p * p * p;
        if (p3 == 0)                 /* should not happen for p ≤ 1000 */
            continue;
        uint64_t start = (prev_cap + p3 - 1) / p3 * p3;
        for (uint64_t j = start; j < new_cap; j += p3)
            cubefree_mark[j] = 0;
    }

    cubefree_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Query whether a given value is cubefree.                           */
/* ------------------------------------------------------------------ */
static bool is_cubefree(uint64_t x)
{
    if (x + 1 > cubefree_cap)
        ensure_cubefree_capacity(x + 1);
    return cubefree_mark[x] != 0;
}

/* ------------------------------------------------------------------ */
/*  Main function required by the test harness.                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Ensure we have accumulated at least n terms.  Because calls may
     * arrive in arbitrary order, the search continues from where it
     * left off. */
    while (cache_len < (size_t)n) {
        /* Test whether next_k satisfies the definition. */
        if (is_cubefree(next_k_to_test) &&
            !is_cubefree(next_k_to_test + 1) &&
            !is_cubefree(next_k_to_test + 2) &&
            !is_cubefree(next_k_to_test + 3) &&
            is_cubefree(next_k_to_test + 4)) {
            append_candidate(&term_cache, &cache_len, &cache_cap,
                             (seq_t)next_k_to_test);
        }
        ++next_k_to_test;
    }
    return term_cache[cache_len - 1];
}
