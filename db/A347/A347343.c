/*
 * A347343  Positive integers k such that k with the last digit repeated is
 *          prime.
 * https://oeis.org/A347343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 2147, u32 n <= 344, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: compute A347343 by incremental search and cache.
   A(n) = the n-th positive integer k such that the number obtained by
   concatenating k with its last digit is prime. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* cached sequence values */
static seq_t *cache = NULL;     /* array of stored terms   */
static size_t cache_len = 0;    /* number of terms stored */
static size_t cache_cap = 0;    /* allocated capacity */

/* expand the cache to contain at least n terms */
static inline void ensure_terms(idx_t n)
{
    if (cache_len >= (size_t)n) return;

    /* determine starting candidate */
    idx_t cand = cache_len ? (idx_t)cache[cache_len - 1] + (idx_t)1 : (idx_t)1;

    /* maximum value that can be represented in seq_t (for fixed‑width types) */
    idx_t seqt_max = (idx_t)((seq_t)-1);
    bool seqt_fixed = ((seq_t)-1)    /* non‑zero for unsigned types */
                      && !((seq_t)-1 > seqt_max); /* avoid overflow for big ints */

    for (; cache_len < (size_t)n; ++cand) {
        /* stop if candidate would exceed the representable range of seq_t */
        if (seqt_fixed && cand > seqt_max) break;

        /* construct candidate * 10 + last digit */
        uint64_t last = cand % 10;
        seq_t concat = (seq_t)(cand * 10 + last);

        if (is_prime_big(concat)) {
            /* grow storage if needed */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { cache = NULL; return; }
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cand;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offsets allowed but ignored */
    ensure_terms(n);
    if (cache_len < (size_t)n) return 0; /* no more terms (overflowed seq_t) */
    return cache[(size_t)n - 1];
}
