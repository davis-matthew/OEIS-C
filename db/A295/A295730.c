/*
 * A295730  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = -1,
 *          a(1) = 0, a(2) = 0, a(3) = 1.
 * https://oeis.org/A295730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4), a(0) = -1, a(1)
 *            = 0, a(2) = 0, a(3) = 1
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 43, i64 n <= 89, i128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_size = 0;
static idx_t  last_computed = 0;

/* Return the n‑th term of A295730 */
seq_t a(idx_t n)
{
    /* Handle small indices directly (no recurrence needed) */
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)1;

    /* Initialize cache on the first call */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)-1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)1;
        last_computed = 3;
    }

    /* Grow the cache if needed */
    if (cache_size <= n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute any missing terms using the recurrence */
    if (last_computed < n) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            cache[i] = cache[i-1] + 3*cache[i-2] - 2*cache[i-3] - 2*cache[i-4];
        }
        last_computed = n;
    }

    return cache[n];
}
