/*
 * A351733  Expansion of e.g.f. exp( 2 * x * (exp(x) - 1) ).
 * https://oeis.org/A351733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} 2^k * Stirling2(n-k,k)/(n-k)!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Compute OEIS A351733 (exp(2*x*(exp(x)-1))) */
seq_t a(idx_t n)
{
    /* memoized cache for already-computed terms */
    static seq_t *cache = NULL;
    static idx_t cap = 0;        /* current capacity of cache */

    /* initialise for a(0) */
    if (cache == NULL) {
        cache = seq_alloc(1);    /* one element for a[0] */
        cache[0] = 1;            /* a(0) = 1 */
        cap = 1;
    }

    /* extend cache up to n if necessary */
    if (n >= cap) {
        idx_t new_cap = n + 1;
        cache = seq_realloc(cache, cap, new_cap);

        for (idx_t i = cap; i <= n; ++i) {
            seq_t sum = 0;
            seq_t binom = 1;   /* binom(i-1,0) for k=1 */

            for (idx_t k = 2; k <= i; ++k) {
                /* update binom = binom(i-1, k-1) from previous value   */
                binom = binom * (seq_t)(i - k + 1) / (seq_t)(k - 1);

                seq_t term = cache[i - k];
                term *= (seq_t)k;
                term *= 2;
                term *= binom;

                sum += term;
            }
            cache[i] = sum;
        }
        cap = new_cap;
    }
    return cache[n];
}
