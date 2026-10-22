/*
 * A385952  a(0) = 1; a(n) = Sum_{k=0..n-1} binomial(k+3,3) * a(k) * a(n-1-k).
 * https://oeis.org/A385952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=0..n-1} binomial(k+3,3) * a(k) *
 *            a(n-1-k) (OEIS A385952)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamic array of computed terms */
static idx_t cache_size = 0;  /* number of elements currently stored (reaching cache_size-1) */

seq_t a(idx_t n)
{
    /* Extend the cache up to index n if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        /* Reallocate memory, preserving existing values */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) { /* allocation failure – abort */
            /* In a contest environment, we may not need elaborate error handling.
               For simplicity, just exit. */
            exit(1);
        }
        cache = tmp;
        /* Compute new terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else {
                seq_t sum = (seq_t)0;
                for (idx_t k = 0; k < i; ++k) {
                    /* binomial(k+3,3) = (k+3)(k+2)(k+1)/6 */
                    seq_t w = ((seq_t)(k + 3) * (k + 2) * (k + 1)) / 6;
                    seq_t prod = cache[k] * cache[i - 1 - k];
                    prod *= w;
                    sum += prod;
                }
                cache[i] = sum;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
