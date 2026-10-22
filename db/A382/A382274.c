/*
 * A382274  Expansion of 1/(1 - 4*x/(1-x)^2)^(5/2).
 * https://oeis.org/A382274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A382274 (OEIS)
   a(0) = 1
   a(1) = 10
   a(2) = 90
   a(n) = ((7*n+3)*a(n-1) - (7*n-24)*a(n-2) + (n-3)*a(n-3)) / n   for n > 2   */
#include <stdlib.h>
#include <stddef.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity in elements       */
static idx_t cache_len = 0;    /* next index that must be computed   */

/* Ensure the cache has enough space for indices 0..new_len-1 */
static void expand_cache(size_t new_len)
{
    if (new_len <= cache_cap)
        return;
    size_t new_cap = cache_cap;
    if (new_cap == 0)
        new_cap = 4;
    while (new_cap < new_len)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failure – terminate the program */
        exit(EXIT_FAILURE);
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Ensure space for indices 0..n */
    expand_cache((size_t)n + 1);

    /* Compute values up to n if not already cached */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)10;
            } else if (i == 2) {
                cache[i] = (seq_t)90;
            } else { /* i >= 3 */
                seq_t a1 = cache[i - 1];
                seq_t a2 = cache[i - 2];
                seq_t a3 = cache[i - 3];

                seq_t term1 = (seq_t)(7 * i + 3) * a1;
                seq_t term3 = (seq_t)(i - 3) * a3;

                /* coefficient for a(i-2) may be negative for i==3 */
                int64_t c2 = 7 * (int64_t)i - 24;
                seq_t numerator;
                if (c2 >= 0) {
                    seq_t term2 = (seq_t)c2 * a2;
                    numerator = term1 - term2 + term3;
                } else {
                    seq_t abs_c2 = (seq_t)(-c2);
                    numerator = term1 + abs_c2 * a2 + term3;
                }

                cache[i] = numerator / (seq_t)i;   /* exact division */
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
