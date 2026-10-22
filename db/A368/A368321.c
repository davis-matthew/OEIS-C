/*
 * A368321  Expansion of e.g.f. exp(4*x) / (3 - 2*exp(x)).
 * https://oeis.org/A368321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A368321
   Recurrence:  a(0) = 1,
                a(n) = 4^n + 2 * Σ_{k=1..n} binomial(n,k) * a(n-k)  (n ≥ 1) */
#include <stddef.h>
#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache      = NULL;  /* array of computed a(n)          */
static size_t cache_cap  = 0;     /* allocated size of cache         */
static idx_t  cache_used = 0;     /* number of elements already cached (1 = a(0) present) */
static seq_t  pow4_next  = 1;     /* 4^i for the next index to compute */

/* main function returning a(n) */
seq_t a(idx_t n)
{
    /* allocate or grow cache to hold values up to n */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
    } else if ((size_t)n + 1 > cache_cap) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* compute any missing terms up to n */
    for (idx_t i = cache_used; i <= n; ++i) {
        if (i == 0) {                     /* base case */
            cache[0] = 1;
        } else {
            seq_t sum = 0;
            seq_t bin = 1; /* binomial(i,0) */

            for (idx_t j = 0; j < i; ++j) {
                sum += bin * cache[j];
                if (j < i - 1) {
                    seq_t num = (seq_t)(i - j);
                    seq_t den = (seq_t)(j + 1);
                    bin = (bin * num) / den;   /* exact integer division */
                }
            }
            cache[i] = pow4_next + 2 * sum;
        }
        /* prepare pow4_next for next i (i+1) */
        pow4_next *= (seq_t)4;
    }

    cache_used = n + 1;
    return cache[n];
}
