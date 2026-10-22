/*
 * A300989  O.g.f. A(x) satisfies: A(x) = x*(1 - 3*x*A'(x)) / (1 - 4*x*A'(x)).
 * https://oeis.org/A300989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A300989 – OEIS sequence
 *
 * Recurrence:   a(1) = 1
 *                a(k) = 4 * Σ_{i=1}^{k-1}  i * a(i) * a(k-i)
 *                      – 3 * (k-1) * a(k-1)          for k ≥ 2
 *
 * The function memoizes results so calls may come in any order.
 */

#include <stdlib.h>

/* static cache: cache[n] holds a(n) (n starts at 0, but we never use index 0) */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of allocated elements */

/* make sure cache can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;

    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache)
        abort();   /* out of memory */

    /* copy old data if present */
    if (cache) {
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    /* initialise new slots to 0 (sentinel for “uncomputed”) */
    for (idx_t i = cache_size; i < new_size; ++i)
        new_cache[i] = (seq_t)0;

    cache = new_cache;
    cache_size = new_size;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    ensure_cache(n);

    /* initialise a(1) if it has not been computed yet */
    if (cache[1] == (seq_t)0)
        cache[1] = (seq_t)1;

    /* compute all a(k) up to n that are still uncomputed */
    for (idx_t k = 2; k <= n; ++k) {
        if (cache[k] == (seq_t)0) {
            seq_t sum = (seq_t)0;
            for (idx_t i = 1; i < k; ++i) {
                sum += (seq_t)i * cache[i] * cache[k - i];
            }
            cache[k] = (seq_t)4 * sum -
                       (seq_t)3 * ((seq_t)(k - 1) * cache[k - 1]);
        }
    }

    return cache[n];
}
