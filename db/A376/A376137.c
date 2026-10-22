/*
 * A376137  a(0) = 1; a(n) = Sum_{k=0..n-1} (-1)^k * (k+1)^2 * a(k) *
 *          a(n-k-1).
 * https://oeis.org/A376137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = Σ_{k=0}^{n-1} (-1)^k * (k+1)^2 * a(k) *
 *            a(n-k-1); offset 0
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 12, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simple memoised convolution implementation.  The sequence can be
   computed in O(n²) time and O(n) memory.  All arithmetic is performed
   in the type seq_t, so it works for 8‑, 16‑, 32‑, 64‑bit signed integers
   and for the arbitrary‑precision types supplied by the build. */

#include <stdlib.h>   /* for realloc, exit, size_t */

static seq_t *cache = NULL;      /* array of computed terms */
static size_t cache_cap = 0;     /* allocated size */
static size_t cache_used = 0;    /* number of computed elements */

/* Enlarge the cache so that it can hold indices up to 'needed-1'. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!tmp && needed != 0)
        exit(EXIT_FAILURE);   /* out of memory */
    cache = tmp;
    cache_cap = needed;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;

    /* Allocate enough space for indices 0..N. */
    ensure_cache(N + 1);

    /* Initialise a(0) if it hasn't been done yet. */
    if (cache_used == 0) {
        cache[0] = 1;
        cache_used = 1;
    }

    /* Compute any missing terms up to a(N). */
    for (size_t i = cache_used; i <= N; ++i) {
        seq_t sum = 0;
        for (size_t k = 0; k < i; ++k) {
            seq_t ak    = cache[k];
            seq_t aik   = cache[i - 1 - k];
            seq_t coeff = (seq_t)(k + 1);    /* (k+1)  */
            coeff *= coeff;                  /* (k+1)^2*/
            seq_t term = ak * aik;           /* a(k)*a(n-k-1) */
            term *= coeff;                   /* multiply by (k+1)^2 */
            if (k & 1)                       /* apply (-1)^k */
                term = -term;
            sum += term;
        }
        cache[i] = sum;
    }

    cache_used = N + 1;
    return cache[N];
}
