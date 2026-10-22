/*
 * A345105  a(n) = 1 + 3 * Sum_{k=0..n-1} binomial(n-1,k) * a(k) * a(n-k-1).
 * https://oeis.org/A345105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS recurrence:
   a(0) = 1
   a(n) = 1 + 3 * Sum_{k=0..n-1} binomial(n-1,k) * a(k) * a(n-1-k)   (n ≥ 1) */

/* We memoize results in a static array so that any order of calls is handled
   efficiently.  The recurrence is evaluated iteratively and uses an
   efficient multiplicative computation of the binomial coefficients.  The
   helper function binomial() is provided by the common utilities. */

#include <stddef.h>          /* for size_t */
#include <stdlib.h>          /* for malloc, realloc, free */
#include "../utils/binomial.h"

static seq_t *cache = NULL;     /* cached a(n) values   */
static size_t cache_cap = 0;    /* current capacity in bytes   */
static idx_t cache_size = 0;    /* number of computed entries   */

/* Ensure the cache can hold indices up to n (inclusive).  Capacity is
   doubled as needed. */
static void ensure_capacity(idx_t n) {
    if ((idx_t)cache_cap > n)
        return;
    idx_t cap = cache_cap ? cache_cap : 16;
    while ((idx_t)cap <= n)
        cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = cap;
}

/* Return the n-th term of the sequence.  Results are memoized. */
seq_t a(idx_t n) {
    /* Base case: a(0) = 1 */
    if (n == 0) {
        if (cache_size == 0) {
            ensure_capacity(0);
            cache[0] = (seq_t)1;
            cache_size = 1;
        }
        return cache[0];
    }

    /* Expand cache as necessary */
    if (n >= cache_size) {
        ensure_capacity(n);
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {             /* already handled but keep for completeness */
                cache[0] = (seq_t)1;
                continue;
            }
            seq_t sum = 0;
            seq_t bin = 1;                /* binomial(i-1, 0) */
            idx_t m = i - 1;
            for (idx_t k = 0; k < i; ++k) {
                seq_t term = bin * cache[k];
                term *= cache[m - k];
                sum += term;
                if (k + 1 < i) {
                    seq_t num = m - k;
                    bin = bin * num;
                    bin = bin / (k + 1);
                }
            }
            cache[i] = (seq_t)1 + (seq_t)3 * sum;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
