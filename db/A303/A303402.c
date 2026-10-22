/*
 * A303402  Expansion of Product_{k>=1} (1 - 4*x^k)/(1 + 4*x^k).
 * https://oeis.org/A303402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 15, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = coefficient of x^n in the infinite product
   ∏_{k=1..∞} (1 - 4x^k)/(1 + 4x^k) */

/* The algorithm keeps a cache of the coefficients up to the largest
   n requested so far.  For a new n it rebuilds the series from
   scratch by processing the factors one by one, truncating all
   intermediate polynomials to degree n.  The complexity is O(n²) in
   the worst case, which is fast enough for n ≤ 10 000.  */

/* Standard C headers – they are available in both C and C++ builds */
#include <stdlib.h>

/* seq_t and idx_t are supplied by the build system.  They are
   signed integer types of arbitrary width and unsigned 64‑bit
   indices, respectively. */

seq_t a(idx_t n)
{
    /* static cache of already computed coefficients */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of terms stored (<=) */

    /* simple case: n is already available */
    if (n + 1 <= cache_len && cache != NULL)
        return cache[n];

    /* allocate arrays for degree n all the way up to n (inclusive) */
    size_t N = (size_t)n;                 /* assume n fits into size_t */
    size_t len = N + 1;                   /* indices 0..N */

    seq_t *new_cache = (seq_t*)calloc(len, sizeof(seq_t));
    if (!new_cache)                       /* out of memory */
        return (seq_t)0;

    seq_t *b = (seq_t*)malloc(len * sizeof(seq_t));
    if (!b) {
        free(new_cache);
        return (seq_t)0;
    }

    /* initial polynomial: only a_0 = 1 */
    new_cache[0] = (seq_t)1;

    /* process factors (1 - 4x^k)/(1 + 4x^k) for k = 1..N */
    for (size_t k = 1; k <= N; ++k) {
        /* ---------- division step:  new_cache / (1 + 4x^k)  -> b  ---------- */
        /* for degrees < k, the result is unchanged */
        size_t lim = k < len ? k : len;
        for (size_t i = 0; i < lim; ++i)
            b[i] = new_cache[i];
        /* for degrees >= k, use recurrence  b[i] = new_cache[i] - 4*b[i-k] */
        for (size_t i = k; i < len; ++i)
            b[i] = new_cache[i] - (seq_t)4 * b[i - k];

        /* ---------- multiplication step:  b * (1 - 4x^k)  -> new_cache  ---------- */
        for (size_t i = 0; i < lim; ++i)
            new_cache[i] = b[i];
        for (size_t i = k; i < len; ++i)
            new_cache[i] = b[i] - (seq_t)4 * b[i - k];
    }

    /* remember the newly computed series */
    free(cache);
    free(b);
    cache = new_cache;
    cache_len = (idx_t)len;

    return cache[n];
}
