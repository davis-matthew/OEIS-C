/*
 * A355327  Number of ways to tile a 2 X n board with squares and dominoes
 *          where vertical dominoes are only allowed in even-numbered
 *          locations.
 * https://oeis.org/A355327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 9*a(n-2) - 7*a(n-4) + a(n-6).
 * Initial values: a(0)=1, a(1)=1, a(2)=5, a(3)=10, a(4)=39, a(5)=83
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Base values up to index 5 */
static const seq_t init[6] = {
    (seq_t)1, (seq_t)1, (seq_t)5, (seq_t)10, (seq_t)39, (seq_t)83
};

/* Ensure cache contains values up to index n. */
static void extend_cache(idx_t n)
{
    if (cache_len > n)
        return;

    size_t new_len = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)
        abort(); /* allocation failed */

    cache = new_cache;

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 6) {
            cache[i] = init[i];
        } else {
            /* Recurrence: a(n) = 9*a(n-2) - 7*a(n-4) + a(n-6) */
            cache[i] = 9 * cache[i-2] - 7 * cache[i-4] + cache[i-6];
        }
    }

    cache_len = new_len;
}

seq_t a(idx_t n)
{
    extend_cache(n);
    return cache[n];
}
