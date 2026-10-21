/*
 * A276928  Number of ordered set partitions of [n] with at most eight
 *          elements per block.
 * https://oeis.org/A276928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1..min(8,n)} binom(n,i) * a(n-i), a(0)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Each binomial coefficient is computed multiplicatively to avoid large
   intermediate factorials. The result is cached so subsequent calls
   with arbitrary indices are O(n). */

#include <stdlib.h>
#include <stddef.h>

/* The caller supplies seq_t and idx_t.  The following helper computes
   binomial coefficients for small k (k ≤ 8). */
static seq_t binom_small(idx_t n, idx_t k)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Static cache of all previously computed a(k). */
    static seq_t *cache   = NULL;      /* array of a[0..computed-1]   */
    static idx_t computed = 0;         /* number of entries computed   */
    static size_t capacity = 0;        /* allocated size of cache      */

    /* Ensure the cache can hold indices up to n. */
    if (n + 1 > capacity) {
        size_t new_capacity = capacity ? capacity * 2 : 1;
        if (new_capacity < (size_t)(n + 1)) {
            new_capacity = (size_t)(n + 1);
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure */
        cache = tmp;
        capacity = new_capacity;
    }

    /* Compute all missing values up to n. */
    for (idx_t idx = computed; idx <= n; ++idx) {
        if (idx == 0) {                       /* a(0) = 1 */
            cache[0] = (seq_t)1;
            continue;
        }
        seq_t sum = 0;
        idx_t max_i = idx < 8 ? idx : 8;
        seq_t binom = 1;                       /* binom(idx,0) */
        for (idx_t i = 1; i <= max_i; ++i) {
            /* Update binom to binom(idx,i) */
            binom = binom * (seq_t)(idx - i + 1) / (seq_t)i;
            sum += binom * cache[idx - i];
        }
        cache[idx] = sum;
    }
    computed = n + 1;

    return cache[n];
}
