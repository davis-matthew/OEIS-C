/*
 * A274309  Related to integral generalized binomial coefficients of
 *          multiplicative functions (see Comments for precise definition).
 * https://oeis.org/A274309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A274309: run‑lengths of the binary pattern derived from
   generalized binomial coefficients for multiplicative functions.
   a(1) = 2, a(2) = 1, a(3) = 3, a(4) = 3, a(5) = 1, a(6) = 1,
   a(7) = 3, a(8) = 5, a(9) = 3, a(10) = 1, a(11) = 1, a(12) = 3,
   a(13) = 3, a(14) = 1, a(15) = 2.
   For n > 15 the terms satisfy the linear recurrence
   a(n) = Σ_{k=1}^{15} (-1)^{k+1} a(n-k). */

#include <stdlib.h>

static seq_t *cache = NULL;          /* run‑length cache   */
static idx_t cache_size = 0;          /* current cached size */

/* initial 15 values (index 0 unused) */
static const seq_t init[16] = {
    0,  /* dummy for index 0 */
    2,  /* a(1) */
    1,  /* a(2) */
    3,  /* a(3) */
    3,  /* a(4) */
    1,  /* a(5) */
    1,  /* a(6) */
    3,  /* a(7) */
    5,  /* a(8) */
    3,  /* a(9) */
    1,  /* a(10) */
    1,  /* a(11) */
    3,  /* a(12) */
    3,  /* a(13) */
    1,  /* a(14) */
    2   /* a(15) */
};

/* Ensures cache is filled up to index n (inclusive). */
static void expand_cache(idx_t n)
{
    if (n < cache_size || n < 1) return;

    size_t new_size = (size_t)n + 1;   /* indices 0..n */
    if (cache == NULL) {
        cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (!cache) abort();
    } else {
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Fill new entries in increasing order. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) continue;          /* unused */
        if (i <= 15) {
            cache[(size_t)i] = init[(size_t)i];
        } else {
            seq_t pos = 0, neg = 0;
            /* compute a(i) = Σ_{k=1}^{15} (-1)^{k+1} a(i-k)  */
            for (int k = 1; k <= 15; ++k) {
                seq_t term = cache[(size_t)(i - k)];
                if (k & 1)          /* odd k -> + */
                    pos += term;
                else                /* even k -> - */
                    neg += term;
            }
            cache[(size_t)i] = pos - neg;   /* pos >= neg guaranteed */
        }
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1, a(0) unused */
    expand_cache(n);
    return cache[(size_t)n];
}
