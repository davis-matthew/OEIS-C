/*
 * A246898  Number of length 6+4 0..n arrays with some pair in every
 *          consecutive five terms totalling exactly n.
 * https://oeis.org/A246898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+6*a(n-2)-14*a(n-3)-14*a(n-4)+42*a(n-5)+14*a(n-6)
 *            -70*a(n-7)+70*a(n-9)-14*a(n-10)-42*a(n-11)+14*a(n-12)+14*a(n-13)
 *            -6*a(n-14)-2*a(n-15)+a(n-16) Offset: 1 (a(1) is the first term)
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Initial terms (1‑based).  cache[0] is unused. */
static const seq_t init_terms[17] = {
    0ULL,          /* cache[0] unused */
    802ULL,        /* a(1)  */
    48393ULL,      /* a(2)  */
    643204ULL,     /* a(3)  */
    5592169ULL,    /* a(4)  */
    26346486ULL,   /* a(5)  */
    107053441ULL,  /* a(6)  */
    319477960ULL,  /* a(7)  */
    884303793ULL,  /* a(8)  */
    2058978250ULL, /* a(9)  */
    4577948761ULL, /* a(10) */
    9125970252ULL, /* a(11) */
    17627483353ULL,/* a(12) */
    31593727774ULL,/* a(13) */
    55331109009ULL,/* a(14) */
    91774208656ULL,/* a(15) */
    149536831201ULL/* a(16) */
};

/* Cached sequence: 1‑based.  cache[0] is unused. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* highest n that has been computed */
static size_t cache_cap = 0;   /* allocated capacity */

/* Offsets and coefficients for the recurrence (positive part). */
static const idx_t pos_off[8] =  {1, 2, 5, 6, 9, 12, 13, 16};
static const int    pos_coef[8] = {2, 6, 42, 14, 70, 14, 14, 1};

/* Offsets and coefficients for the recurrence (negative part). */
static const idx_t neg_off[7] =  {3, 4, 7, 10, 11, 14, 15};
static const int    neg_coef[7] = {14, 14, 70, 14, 42, 6, 2};

/* Ensure the cache holds values up to n (inclusive). */
static void
ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Allocate initial capacity large enough for the first 16 terms. */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 1; i <= 16; ++i)
            cache[i] = init_terms[i];
        cache_size = 16;
    }

    while (cache_size < n) {
        /* Grow capacity if necessary. */
        if (cache_size + 1 >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        idx_t i = cache_size + 1;   /* next index to compute   */

        /* Positive part: sum pos_coef[k] * a(i - pos_off[k]) */
        seq_t pos = 0;
        for (int k = 0; k < 8; ++k) {
            pos += cache[i - pos_off[k]] * (seq_t)pos_coef[k];
        }

        /* Negative part: sum neg_coef[k] * a(i - neg_off[k]) */
        seq_t neg = 0;
        for (int k = 0; k < 7; ++k) {
            neg += cache[i - neg_off[k]] * (seq_t)neg_coef[k];
        }

        /* The recurrence guarantees the result is non‑negative. */
        cache[i] = pos - neg;
        cache_size = i;
    }
}

seq_t
a(idx_t n)
{
    if (n < 1) {
        /* The sequence is defined for n ≥ 1.  Return 0 for invalid indices. */
        return (seq_t)0;
    }
    ensure_cache(n);
    return cache[n];
}
