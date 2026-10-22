/*
 * A346022  Primes that are the first in a run of exactly 2 emirps.
 * https://oeis.org/A346022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 29, u32 n <= 29, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime_big, rev_decimal, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the nth prime that starts a run of exactly two emirps. */
/* The definition is implemented by enumerating primes and tracking */
/* consecutive emirps. The implementation is order‑independent and */
/* memoizes results by a static array of type seq_t. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"
#include "../utils/rev_decimal.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Helper that tests whether a prime p is an emirp. */
static inline bool is_emirp(seq_t p)
{
    /* Single‑digit primes are not emirps (palindromic). */
    if ((uint64_t)p < 10) return false;

    seq_t revp = rev_decimal(p);
    if (revp == p) return false;            /* palindrome */

    /* The reversal must be prime. */
    return is_prime_big(revp);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for offset < 1 */

    /* Static cache for discovered terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Static state for prime enumeration and emirp run detection. */
    static uint64_t cur = 2;          /* current candidate prime */
    static size_t run_count = 0;      /* consecutive emirps so far */
    static seq_t run_start = 0;       /* start of current emirp run */

    /* Grow cache if necessary. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = seq_alloc(cache_cap);
    }

    /* Generate primes until we have at least n terms. */
    while (cache_len < n) {
        /* Find next prime. */
        while (!is_prime_big((seq_t)cur)) cur++;

        seq_t p = (seq_t)cur;
        bool emirp = is_emirp(p);

        if (emirp) {
            if (run_count == 0) run_start = p;
            run_count++;
        } else {
            if (run_count == 2) {
                /* Record the start of a run of exactly two emirps. */
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    cache = seq_realloc(cache, cache_cap / 2, cache_cap);
                }
                cache[cache_len++] = run_start;
            }
            run_count = 0;
        }

        cur++;  /* move to next candidate for next loop */
    }

    return cache[n-1];
}
