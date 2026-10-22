/*
 * A368719  a(n) = n! * Sum_{k=0..n} k^5 / k!.
 * https://oeis.org/A368719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + n^5 (a(0) = 0)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = n*a(n-1) + n^5, for n >= 1 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_size = 0;

/* Compute n^5 using only integer operations. */
static seq_t pow5(idx_t n)
{
    seq_t x = (seq_t)n;
    x = x * x;                    /* n^2 */
    x = x * (seq_t)n;             /* n^3 */
    x = x * (seq_t)n;             /* n^4 */
    x = x * (seq_t)n;             /* n^5 */
    return x;
}

seq_t a(idx_t n)
{
    if (cache_size == 0) {
        cache = (seq_t *)calloc(1, sizeof(seq_t));
        if (!cache) return 0;          /* OOM handling */
        cache[0] = 0;                  /* a(0) */
        cache_size = 1;
    }

    if (n >= cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;            /* OOM handling */
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t term = (seq_t)i * cache[i-1] + pow5(i);
            cache[i] = term;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
