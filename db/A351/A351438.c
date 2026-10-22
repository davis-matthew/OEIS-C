/*
 * A351438  G.f. A(x) satisfies: A(x) = 1 + x + x^2 * A(x/(1 - x)) / (1 -
 *          x)^3.
 * https://oeis.org/A351438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..n-2} binomial(n,k+2) * a(k) with a(0) = a(1) =
 *            1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static cache for memoised values */
static seq_t *cache   = NULL;          /* computed values a(0) .. a(cache_last) */
static idx_t  cache_size  = 0;         /* size of allocated array */
static idx_t  cache_last  = (idx_t)-1; /* last index with a valid value */
static bool   cache_inited = false;    /* whether base cases have been set */

/* Grow the cache array to hold indices up to “need” (inclusive) */
static void
ensure_cache(idx_t need)
{
    /* Resize the array if it is too small */
    if (need + 1 > cache_size) {
        idx_t new_size = need + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp && new_size > 0) exit(1);   /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise base cases on first use */
    if (!cache_inited) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_last = 1;
        cache_inited = true;
    }

    /* Compute all missing terms up to “need” */
    for (idx_t i = cache_last + 1; i <= need; ++i) {
        seq_t sum = (seq_t)0;

        /* Current binomial coefficient C(i,2) */
        seq_t binom = (seq_t)i * ((seq_t)i - (seq_t)1) / (seq_t)2;

        /* j runs from 2 to i inclusive: C(i,j)*a(j-2) */
        for (idx_t j = 2; j <= i; ++j) {
            sum += binom * cache[j - 2];

            /* Advance binom to C(i,j+1) */
            if (j < i) {
                binom = binom * ((seq_t)(i - j)) / (seq_t)(j + 1);
            }
        }
        cache[i] = sum;
    }

    cache_last = need;
}

/* Public function required by the build environment */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return (seq_t)1;
    ensure_cache(n);
    return cache[n];
}
