/*
 * A368696  Numbers whose arithmetic derivative (A003415) is a squarefree
 *          number of the form 4k+2.
 * https://oeis.org/A368696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   build_primes_upto, is_squarefree, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A368696: numbers n whose arithmetic derivative
   (A003415) is squarefree and congruent to 2 modulo 4.  The function searches
   numbers sequentially, factorising each with a pre‑computed prime table, and
   caches the results so that subsequent calls are fast even if the index
   order is arbitrary. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/seq_alloc.h"       /* seq_alloc, seq_realloc */
#include "../utils/is_squarefree.h"   /* is_squarefree(seq_t) */
#include "../utils/build_primes_upto.h" /* build_primes_upto */
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;          /* cached sequence values */
static size_t cache_cap = 0;         /* allocated capacity of cache */
static size_t cache_len = 0;         /* number of stored values */
static seq_t last_checked = 0;       /* last integer examined */

static uint64_t *prime_table = NULL; /* list of primes for factorisation */
static size_t prime_cnt = 0;
static bool primes_loaded = false;

/* Load prime table once, up to an arbitrary but generous limit. */
static void ensure_primes(void)
{
    if (!primes_loaded) {
        build_primes_upto(1000000, &prime_table, &prime_cnt);
        primes_loaded = true;
    }
}

/* Compute the arithmetic derivative of n (sequence type) using trial division. */
static seq_t arithmetic_derivative(seq_t n)
{
    uint64_t n_ll = (uint64_t)n;      /* assumption: fits in 64 bits */
    uint64_t remaining = n_ll;
    seq_t d = 0;
    for (size_t i = 0; i < prime_cnt && prime_table[i] * prime_table[i] <= remaining; ++i) {
        uint64_t p = prime_table[i];
        if (remaining % p == 0) {
            unsigned exp = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++exp;
            }
            /* term = n / p * exp */
            seq_t term = n / (seq_t)p;
            term *= (seq_t)exp;
            d += term;
        }
    }
    if (remaining > 1) {            /* remaining is a prime factor */
        seq_t term = n / (seq_t)remaining;  /* remaining fits in 64 bits */
        d += term;   /* exponent is 1 */
    }
    return d;
}

/* Ensure cache has at least n entries, extending it by searching more numbers. */
static void ensure_cache(idx_t n)
{
    ensure_primes();
    if (cache == NULL) {
        cache_cap = 128;
        cache = seq_alloc(cache_cap);
        cache_len = 0;
        last_checked = 0;          /* start checking from 1 (to be incremented) */
    }
    while (cache_len < (size_t)n) {
        ++last_checked;
        seq_t d = arithmetic_derivative(last_checked);
        if ((d & 3U) == 2 && is_squarefree(d)) {
            /* cache full? grow */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (tmp == NULL) abort(); /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_checked;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined under offset 1, but safe fallback */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
