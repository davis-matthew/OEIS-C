/*
 * A367655  G.f. A(x) satisfies A(x) = 1 / (1 - x - x * (1 + x) * A(x^2)).
 * https://oeis.org/A367655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + Sum_{k=0..n-1} a(floor(k/2)) * a(n-1-k), a(0) =
 *            1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static size_t a_cache_max = 0;

/* Return the n-th term of sequence A367655. */
seq_t a(idx_t n) {
    /* Ensure the cache exists. */
    if (a_cache == NULL) {
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        if (a_cache == NULL) abort();
        a_cache[0] = (seq_t)1;   /* a(0) */
        a_cache_max = 0;
    }

    /* If the desired term is already cached, return it. */
    if ((size_t)n <= a_cache_max) {
        return a_cache[(size_t)n];
    }

    /* Resize the cache to accommodate up to index n. */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    if (tmp == NULL) abort();
    a_cache = tmp;

    /* Compute terms from the next unused index up to n. */
    for (size_t i = a_cache_max + 1; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        for (size_t k = 0; k < i; ++k) {
            sum += a_cache[k >> 1] * a_cache[i - 1 - k];
        }
        a_cache[i] = a_cache[i - 1] + sum;
    }
    a_cache_max = (size_t)n;

    return a_cache[(size_t)n];
}
