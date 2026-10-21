/*
 * A251143  Number of (n+1) X (2+1) 0..2 arrays with every 2 X 2 subblock
 *          summing to 4 and no 2 X 2 subblock having exactly two nonzero
 *          entries.
 * https://oeis.org/A251143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A251143 – Number of (n+1) X (2+1) 0..2 arrays with every 2 X 2 subblock
 * summing to 4 and no 2 X 2 subblock having exactly two nonzero entries.
 *
 * Empirical recurrence (offset 1):
 *   a(1)=27, a(2)=49, a(3)=87, a(4)=161, a(5)=299
 *   a(n) = 3*a(n-1) - 5*a(n-3) + a(n-4) + 2*a(n-5)   for n ≥ 6
 */

#include <stddef.h>
#include <stdlib.h>

/* Ensure the cache has capacity for indices up to n (inclusive) */
static void
ensure_cache_capacity(idx_t n, seq_t **cache_p, size_t *cap_p) {
    /* capacity for indices 0 .. n inclusive is n+1 */
    if (n + 1 > *cap_p) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(*cache_p, new_cap * sizeof(seq_t));
        *cache_p = tmp;
        *cap_p   = new_cap;
    }
}

/* memoised array of previously computed terms.
 * cache[0] is unused; cache[1] = a(1), cache[2] = a(2), ...   */
static seq_t *cache      = NULL;              /* cache[0] unused */
static size_t cache_cap  = 0;                 /* current allocated size in seq_t */
static idx_t  cache_last = 0;                 /* maximum index that has been computed */

/* compute the recurrence up to index n (inclusive) */
static void
compute_to(idx_t n)
{
    if (n == 0)
        return;          /* safety – the sequence is defined for n≥1 */

    ensure_cache_capacity(n, &cache, &cache_cap);

    /* iterate from the next uncomputed index to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i == 1)
            cache[1] = (seq_t)27;
        else if (i == 2)
            cache[2] = (seq_t)49;
        else if (i == 3)
            cache[3] = (seq_t)87;
        else if (i == 4)
            cache[4] = (seq_t)161;
        else if (i == 5)
            cache[5] = (seq_t)299;
        else {
            cache[i] =
                (seq_t)3 * cache[i-1] - (seq_t)5 * cache[i-3] +
                cache[i-4] + (seq_t)2 * cache[i-5];
        }
    }
    cache_last = n;
}

/* Public API */
seq_t a(idx_t n)
{
    compute_to(n);
    return cache[n];
}
