/*
 * A373569  Expansion of x + 1/(-x - 1/(-x - 1/(-x - 1/(-x - 1/(-x + 1))))).
 * https://oeis.org/A373569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^n] (x^6 - x^5 - 5*x^4 + 4*x^3 + 6*x^2 - 3*x - 1) / *
 *            (x^5 - x^4 - 4*x^3 + 3*x^2 + 3*x - 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* array of computed terms */
static size_t cache_len = 0;    /* number of terms stored in cache */

/* 7 initial terms of the sequence, a(0) .. a(6) */
static const seq_t init_terms[7] = {
    (seq_t)1,
    (seq_t)6,
    (seq_t)15,
    (seq_t)55,
    (seq_t)190,
    (seq_t)671,
    (seq_t)2353
};

/* Ensure that cache contains at least terms up to idx n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* Allocate cache if it does not exist yet */
    if (cache == NULL) {
        size_t cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) exit(1);   /* allocation failure */
        /* initialise the first 7 entries */
        for (size_t i = 0; i < 7; ++i) cache[i] = init_terms[i];
        cache_len = 7;
    }

    /* Grow cache if needed */
    if ((size_t)n >= cache_len) {
        size_t new_cap = cache_len ? cache_len * 2 : 8;
        while ((size_t)n >= new_cap) new_cap *= 2;

        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) exit(1);
        cache = new_cache;

        /* Compute new terms up to n using the recurrence */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i < 7) {
                cache[i] = init_terms[i];
            } else {
                /* a(n) = 3*a(n-1) + 3*a(n-2) - 4*a(n-3) - a(n-4) + a(n-5) */
                cache[i] =
                    (seq_t)3 * cache[i - 1] +
                    (seq_t)3 * cache[i - 2] -
                    (seq_t)4 * cache[i - 3] -
                    (seq_t)1 * cache[i - 4] +
                    (seq_t)1 * cache[i - 5];
            }
        }
        cache_len = (size_t)n + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
