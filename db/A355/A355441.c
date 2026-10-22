/*
 * A355441  Numbers k such that the sum of the least prime factors of i=2..k
 *          is prime.
 * https://oeis.org/A355441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 340, u32 n <= 58, u64 n <= 5676,
 *            u128 n <= 5676, big any n
 * Helpers:   _smallest_prime_factor, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A355441: Numbers k such that the sum of the least prime factors of i=2..k is prime.
   The function builds the list incrementally, caching results so that the
   sequence can be asked in any order. */

/* The function uses the helpers _smallest_prime_factor(uint64_t) to obtain
   least prime factors and is_prime_big(seq_t) to test primality of the
   accumulated sum.  It dynamically grows a cache of the discovered
   k‑values. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/is_prime_big.h"

static seq_t *term_cache = NULL;   /* array of discovered k-values      */
static size_t term_cap   = 0;      /* capacity of term_cache            */
static size_t term_len   = 0;      /* number of stored terms            */

static idx_t next_i       = 2;     /* next i to process                 */
static seq_t current_sum  = 0;     /* sum of lpf up to next_i-1         */

/* Grow the cache to at least new_cap elements. */
static void grow_terms(void)
{
    size_t new_cap = term_cap ? term_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(term_cache,
                                   new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* out of memory – fatal              */
    term_cache = tmp;
    term_cap   = new_cap;
}

/* Expand the cache until it contains at least 'need' terms. */
static void ensure_terms(idx_t need)
{
    if (term_len >= need) return;

    /* Allocate initial cache if necessary. */
    if (!term_cache) grow_terms();

    for (;;) {
        uint64_t i = (uint64_t)next_i;
        uint64_t lpf = _smallest_prime_factor(i);
        current_sum += (seq_t)lpf;

        if (is_prime_big(current_sum)) {
            if (term_len == term_cap) grow_terms();
            term_cache[term_len++] = (seq_t)next_i;
        }
        ++next_i;

        if (term_len >= need) break;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1; guard case   */
    ensure_terms(n);
    return term_cache[n - 1];
}
