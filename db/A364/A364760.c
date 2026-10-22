/*
 * A364760  G.f. satisfies A(x) = 1 / (1 + x*(1 + x*A(x))^2).
 * https://oeis.org/A364760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..n} (-1)^k * binomial(n+1,k) *
 *            binomial(2*k,n-k)
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 20, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;
static idx_t max_computed = 0;

/* Allocate or grow the cache to hold values up to n (inclusive). */
static void grow_cache(idx_t n)
{
    if (cache == NULL) {
        cache = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
        if (cache == NULL)
            abort();
        /* Compute the first value a(0) = 1 */
        cache[0] = (seq_t)1;
        max_computed = 0;
        if (n == 0) {
            return;
        }
    } else if (n > max_computed) {
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL)
            abort();
        cache = tmp;
    }
}

/* Return the n‑th value of the sequence. */
seq_t a(idx_t n)
{
    /* Ensure the cache is large enough. */
    grow_cache(n);

    /* If the requested value has already been computed, return it. */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Compute values from max_computed+1 up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k <= i; ++k) {
            seq_t term = binomial(i + 1, k);
            term = term * binomial(2 * k, i - k);
            if (k & 1) {
                term = -term;
            }
            sum += term;
        }
        cache[i] = sum / (seq_t)(i + 1);
    }
    max_computed = n;
    return cache[n];
}
