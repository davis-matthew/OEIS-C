/*
 * A386531  E.g.f. A(x) satisfies A(x) = exp(x + x^3/6 * A''(x)).
 * https://oeis.org/A386531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(n)=a(n-1)+(1/6)*Σ_{k=0}^{n-1}(-k + k^3)*C(n-1,k)*a(k)*a(n-1-k) */
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Cached sequence values */
static seq_t *cache = NULL;          /* pointer to the array of values   */
static size_t cache_capacity = 0;    /* allocated length of the array    */
static idx_t max_computed = 0;       /* largest n that has already been computed */

seq_t a(idx_t n)
{
    /* Allocate or grow the cache if this is the first call or if
       we need more room for the requested index n. */
    if (cache == NULL) {
        /* Allocate at least two elements to avoid re‑alloc in the
           very first computation of a(1). */
        cache_capacity = (n > 1) ? (n + 1) : 2;
        cache = seq_alloc(cache_capacity);
        cache[0] = (seq_t)1;        /* a(0) = 1 */
        max_computed = 0;
    } else if ((size_t)(n + 1) > cache_capacity) {
        size_t new_cap = cache_capacity * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
        cache = seq_realloc(cache, cache_capacity, new_cap);
        cache_capacity = new_cap;
    }

    /* Compute any missing terms up to and including n */
    while (max_computed < n) {
        idx_t idx = max_computed + 1;                 /* new term index */
        seq_t sum = (seq_t)0;                        /* Σ term */
        seq_t binom = (seq_t)1;                      /* C(idx-1,0) */

        /* Sum over k = 0 .. idx-1 */
        for (idx_t k = 0; k <= idx - 1; ++k) {
            /* factor = k^3 - k = k*(k^2-1); for k = 0,1 this is 0
               which is fine because term will be 0. */
            seq_t kseq = (seq_t)k;
            seq_t factor = kseq * (kseq * kseq - (seq_t)1);

            /* term = factor * binom * a(k) * a(idx-1-k) */
            seq_t term = factor;
            term *= binom;
            term *= cache[k];
            term *= cache[idx - 1 - k];
            sum += term;

            /* Update binom to C(idx-1, k+1) for next iteration
               (unless we are at the last k). */
            if (k < idx - 1) {
                seq_t num = (seq_t)(idx - 1 - k);
                seq_t den = (seq_t)(k + 1);
                binom = binom * num;
                binom = binom / den;  /* exact division */
            }
        }

        /* Increment a(idx) by sum/6 (sum is always divisible by 6). */
        seq_t inc = sum / (seq_t)6;
        cache[idx] = cache[idx - 1] + inc;
        max_computed = idx;
    }

    return cache[n];
}
