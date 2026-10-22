/*
 * A345077  a(0) = 1; a(n) = 6 * Sum_{k=1..n} binomial(n,k) * a(k-1).
 * https://oeis.org/A345077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6 * Sum_{k=1..n} binomial(n,k) * a(k-1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0;

/* Ensure the cache can hold indices up to 'needed' (exclusive) */
static void grow_cache(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Return a(n) for the sequence A345077 */
seq_t a(idx_t n)
{
    /* Compute up to n if necessary */
    if (n >= cache_len) {
        grow_cache(n + 1);
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == (idx_t)0) {
                cache[0] = (seq_t)1;
                continue;
            }
            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1; /* C(i,0) */
            for (idx_t k = 1; k <= i; ++k) {
                /* Update binomial coefficient: C(i,k) = C(i,k-1)*(i-k+1)/k */
                binom = binom * (seq_t)(i - k + 1);
                binom = binom / k;
                sum += binom * cache[k - 1];
            }
            cache[i] = (seq_t)6 * sum;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
