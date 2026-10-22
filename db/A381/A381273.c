/*
 * A381273  Expansion of e.g.f. exp(x * cosh(2*x)).
 * https://oeis.org/A381273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A381273: a(n) = Σ_{k=0..⌊(n-1)/2⌋} 4^k * (2k+1) * binomial(n-1,2k) * a(n-2k-1),
 *  with a(0)=1.  The function caches already-computed terms for
 *  arbitrary-order calls. */
#include <stdlib.h>  /* realloc, abort */
#include <stddef.h>  /* size_t */

/* Multiply two seq_t values, then divide by an idx_t divisor.
 *  Division is exact by construction of the binomial coefficient. */
static seq_t
_binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;   /* exact division */
    }
    return res;
}

seq_t
a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Ensure cache large enough for index n. */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();          /* allocation failure */
        /* Initialise new entries to 0 (uncomputed). */
        for (size_t i = cache_size; i < new_size; ++i) new_cache[i] = 0;
        cache = new_cache;
        cache_size = new_size;
    }

    /* Set a(0) if not already stored. */
    if (cache[0] == 0) cache[0] = 1;

    /* Compute any missing terms up to n. */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != 0) continue;   /* already computed */

        seq_t sum = 0;
        idx_t maxk = (i - 1) >> 1;     /* floor((i-1)/2) */
        seq_t pow4 = 1;                /* 4^k, k = 0 */

        for (idx_t k = 0; k <= maxk; ++k) {
            idx_t twice = k << 1;                      /* 2k */
            seq_t bin = _binom(i - 1, twice);        /* C(i-1,2k) */
            seq_t term = bin * (seq_t)(twice + 1);    /* bin * (2k+1) */
            term = term * cache[i - twice - 1];       /* * a(i-2k-1) */
            term = term * pow4;                       /* * 4^k */
            sum += term;
            pow4 *= 4;                                 /* next power of 4 */
        }

        cache[i] = sum;
    }

    return cache[n];
}
