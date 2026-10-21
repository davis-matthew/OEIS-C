/*
 * A275319  Numbers n such that n concatenated with n+1 is not a prime.
 * https://oeis.org/A275319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1006, u32 n <= 1006, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   concat_parts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Condition: a(n) = n such that the concatenation of n and n+1 is composite.
   This implements OEIS sequence A275319. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*  Utility functions provided by the build environment.  */
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"

/*  Cache for computed terms.  */
static seq_t *cache = NULL;      /* array of sequence values */
static size_t cache_size = 0;    /* number of elements stored */
static size_t cache_cap   = 0;    /* allocated capacity */
static idx_t next_candidate = 1; /* next integer to test for inclusion */

/*  Ensure cache has at least `needed` capacity.  */
static void ensure_capacity(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                /* allocation failure -> terminate */
    cache = tmp;
    cache_cap = new_cap;
}

/*  Generate terms until we have at least `n` elements stored.  */
static void fill_cache_to(idx_t n) {
    if (n <= cache_size) return;
    /*  We may need to test many candidates; continue from last state.  */
    while (cache_size < n) {
        /*  Compute concatenation of candidate and candidate+1.  */
        seq_t concatenated = concat_parts(next_candidate, next_candidate + 1);
        /*  If concatenated is not prime, store the candidate.  */
        if (!is_prime_big(concatenated)) {
            ensure_capacity(cache_size + 1);
            cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/*  OEIS sequence A275319: numbers n such that concat(n, n+1) is composite.  */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for offset 0, but return 0 */
    fill_cache_to(n);
    return cache[n - 1];
}
