/*
 * A274112  Number of equivalence classes of ballot paths of length n for the
 *          string ddu.
 * https://oeis.org/A274112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(0) = 1, a(n) = 2*a(n-1) + conv[n-2] - conv[n-1] + conv[n-4]
 *            (n≥1) * where conv[k] = Σ_{i=0}^k a[i]*a[k-i] (convolution of
 *            the sequence with itself). * The function memoises the sequence
 *            and the convolution array, so that each * index is computed at
 *            most once, and respects the required types
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 60, u64 n <= 118,
 *            u128 n <= 235, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *a_cache = NULL;           /* a[0..max_cached]          */
static seq_t *conv_cache = NULL;        /* conv[0..2*max_cached]     */
static idx_t  cached_max = 0;           /* highest index computed    */
static size_t cap_a      = 0;           /* capacity of a_cache       */
static size_t cap_conv   = 0;           /* capacity of conv_cache    */

static void
ensure_capacity(seq_t **cache, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t old_cap = *cap;
        size_t new_cap = old_cap ? old_cap << 1 : needed;
        if (new_cap < needed) new_cap = needed;
        seq_t *newp = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
        if (!newp) abort();          /* allocation failure – terminate */
        *cache = newp;
        *cap   = new_cap;
        /* zero newly allocated elements (only needed for conv_cache) */
        if (new_cap > old_cap) {
            memset(newp + old_cap, 0, (new_cap - old_cap) * sizeof(seq_t));
        }
    }
}

/* Main entry: compute a(n). */
seq_t a(idx_t n)
{
    /* Initialise base case on the first call. */
    if (cached_max == 0) {
        ensure_capacity(&a_cache,  &cap_a,      1);
        ensure_capacity(&conv_cache, &cap_conv, 1);
        a_cache[0]        = (seq_t)1;
        conv_cache[0]     = a_cache[0] * a_cache[0];   /* conv[0] = 1   */
        /* Note: conv_cache[1] and higher are zero by ensure_capacity. */
        cached_max = 0;
    }

    if (n <= cached_max) {
        return a_cache[n];
    }

    /* Extend memoised range if the requested index is larger. */
    if (n > cached_max) {
        /* Allocate space for new indices. */
        ensure_capacity(&a_cache,  &cap_a,      n + 1);
        ensure_capacity(&conv_cache, &cap_conv, 2 * n + 1);

        /* Compute new terms. */
        for (idx_t k = cached_max + 1; k <= n; ++k) {
            /* Compute the convolution sums needed for a_k. */
            seq_t C = 0;
            if (k >= 2) C += conv_cache[k - 2];
            if (k >= 1) C -= conv_cache[k - 1];
            if (k >= 4) C += conv_cache[k - 4];

            /* a_k = 2*a_{k-1} + C. */
            seq_t a_k = a_cache[k - 1] + a_cache[k - 1] + C;
            a_cache[k] = a_k;

            /* Update convolution cache for indices ≥ k. */
            /* For each j < k add 2*a_k*a_j to conv[k+j]. */
            for (idx_t j = 0; j < k; ++j) {
                seq_t prod = a_k * a_cache[j];
                conv_cache[k + j] += prod;   /* first copy */
                conv_cache[k + j] += prod;   /* second copy (doubling) */
            }
            /* Finally add a_k*a_k to conv[2*k] (previous contributions
               may already be present). */
            conv_cache[2 * k] += a_k * a_k;
        }
        cached_max = n;
    }

    return a_cache[n];
}
