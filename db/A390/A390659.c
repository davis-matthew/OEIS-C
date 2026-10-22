/*
 * A390659  G.f. A(x) satisfies A(x) = 1 / ((1 - x) * (1 - 2 * x * A(x^2))).
 * https://oeis.org/A390659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2 * Sum_{k=0..⌊(n-1)/2⌋} a(k) * a(n-1-2k)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed terms */
static size_t cap   = 0;         /* allocated capacity */
static size_t len   = 0;         /* number of terms stored (0..len-1) */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache_n(idx_t needed)
{
    size_t n = (size_t)needed;
    if (cap < n) {
        size_t new_cap = cap > 0 ? cap : 1;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Allocate or grow cache to fit indices up to n */
    ensure_cache_n(n + 1);

    /* Base case a(0) = 1 */
    if (len == 0) {
        cache[0] = 1;
        len = 1;
    }

    /* Compute any missing terms up to n */
    while (len <= n) {
        idx_t i = len;                     /* index of the term to compute */
        seq_t sum = 0;                      /* accumulator */
        idx_t limit = (i - 1) / 2;          /* floor((i-1)/2) */
        for (idx_t k = 0; k <= limit; ++k) {
            seq_t prod = cache[k] * cache[i - 1 - 2 * k];
            sum += prod;
        }
        cache[i] = 1 + 2 * sum;              /* recurrence */
        ++len;
    }

    return cache[n];
}
