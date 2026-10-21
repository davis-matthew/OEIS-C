/*
 * A245209  Nonprimes such that it takes exactly 4 iterations of
 *          reverse-and-add digits to generate a prime.
 * https://oeis.org/A245209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   is_prime_big, reverse_decimal, seq_alloc_new_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A245209 – Nonprime numbers that take exactly four
   reverse‑and‑add iterations to become a prime.  The function
   a(n) returns the n‑th such number (offset 0).  It searches
   candidates incrementally and memoises the results.  The
   implementation relies on the supplied arbitrary‑precision helper
   functions for memory management. */

#include <stdlib.h>
#include <stdbool.h>

/* Utility functions for prime testing and digit reversal */
#include "../utils/is_prime_big.h"
#include "../utils/reverse_decimal.h"

/* Utility functions for dynamic array management of seq_t */
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free.h"

/* static cache of sequence terms */
static seq_t *cache = NULL;      /* array of discovered terms */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of terms stored */
static uint64_t next_candidate = 1; /* candidate to test next */

/* Append a newly found term to the cache, reallocating if necessary. */
static void append_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_cache = seq_alloc_new_array(new_cap);
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        if (cache)
            seq_free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Ensure that the cache contains at least (n+1) terms. */
static void ensure_terms(idx_t n)
{
    while (cache_len <= (size_t)n) {
        seq_t x = (seq_t)next_candidate;
        ++next_candidate;

        /* Must start as a non‑prime. */
        if (is_prime_big(x))
            continue;

        seq_t y = x;
        bool fail = false;

        /* Perform up to four reverse‑and‑add steps. */
        for (int step = 1; step <= 4; ++step) {
            y = reverse_decimal(y) + y;      /* reverse‑and‑add */

            /* If an intermediate number is prime, this candidate fails. */
            if (step < 4 && is_prime_big(y)) {
                fail = true;
                break;
            }
        }

        /* Fourth step must yield a prime, others must not. */
        if (fail || !is_prime_big(y))
            continue;

        /* Record the term. */
        append_cache(x);
    }
}

/* Public API required by the test harness */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return cache[(size_t)n];
}
