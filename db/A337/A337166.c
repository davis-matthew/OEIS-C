/*
 * A337166  Sum_{n>=0} a(n) * x^n / (n!)^2 = exp(1 + x -
 *          BesselI(0,2*sqrt(x))).
 * https://oeis.org/A337166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A337166
   Recurrence:
     a(0) = 1
     a(1) = 0
     a(n) = -(1/n) * Σ_{k=0..n-2} binomial(n,k)^2 * (n-k) * a(k)   for n ≥ 2
   The code memoises computed values in a static cache so that calls
   may arrive in any order.  All arithmetic uses the build‑supplied
   integer type seq_t; indices are uint64_t (idx_t). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Declaration of the helper that computes binomial coefficients.
   It returns an exact integer of type seq_t. */
extern seq_t binomial(idx_t n, idx_t k);

/* Compute a(n) with memoisation. */
seq_t a(idx_t n)
{
    /* Static cache of already computed a(k). */
    static seq_t *cache      = NULL;  /* pointer to array of seq_t */
    static size_t cache_cap  = 0;     /* allocated capacity */
    static idx_t  cache_len  = 0;     /* number of valid entries */

    /* Number of entries we need.  The array must hold indices 0..n. */
    size_t needed = (size_t)n + 1;
    if (cache_len < needed) {
        /* Allocate or grow the cache. */
        if (cache == NULL) {
            cache = (seq_t *)malloc(needed * sizeof(seq_t));
            cache_cap = needed;
        } else if (cache_cap < needed) {
            seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
            cache = tmp;
            cache_cap = needed;
        }

        /* Compute all missing terms from cache_len up to n. */
        for (idx_t i = cache_len; i < needed; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)0;
            } else {
                seq_t sum = (seq_t)0;
                seq_t bin = (seq_t)1;          /* binomial(i,0) */
                idx_t upper = i - 2;            /* last k value */

                for (idx_t k = 0; k <= upper; ++k) {
                    /* term = binomial(i,k)^2 * (i-k) * a(k) */
                    seq_t term = bin * bin;
                    term = term * (seq_t)(i - k);
                    term = term * cache[k];
                    sum += term;

                    /* Update bin to binomial(i,k+1) for next iteration. */
                    if (k < upper) {
                        bin = bin * (seq_t)(i - k);
                        bin = bin / (seq_t)(k + 1);
                    }
                }
                /* a(i) = -(sum / i); division is exact by definition. */
                cache[i] = -(sum / (seq_t)i);
            }
        }
        cache_len = (idx_t)needed;
    }
    return cache[n];
}
