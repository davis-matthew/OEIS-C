/*
 * A374917  Inverse of the Fibonacci sequence beginning 1,1 with respect to
 *          binomial convolution.
 * https://oeis.org/A374917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -Sum_{k=1..n} binomial(n,k)*a(n-k)*F(k+1) with a(0) = 1.
 *            Here F(k) is the Fibonacci sequence starting 1,1
 * Range:     i8 n <= 7, i16 n <= 9, i32 n <= 14, i64 n <= 23, i128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;           /* a(0..max_computed) */
    static size_t cache_cap = 0;
    static idx_t max_computed = (idx_t)-1;
    static bool cache_initialized = false;

    static seq_t *fib_cache = NULL;       /* fib_cache[i] = F(i)  (F(0)=0, F(1)=1) */
    static size_t fib_cap = 0;
    static idx_t fib_max = 0;             /* highest i computed in fib_cache */

    /* Ensure cache for a(n) */
    if (!cache_initialized) {
        ensure_cache(n, &cache, &cache_cap);
        cache[0] = 1;
        max_computed = 0;
        cache_initialized = true;
    } else if (n > cache_cap - 1) {
        ensure_cache(n, &cache, &cache_cap);
    }

    /* Compute Fibonacci cache up to n+1 */
    if (!fib_cache) {
        ensure_cache(n + 1, &fib_cache, &fib_cap);
        fib_cache[0] = 0;
        fib_cache[1] = 1;
        fib_cache[2] = 1;
        fib_max = 2;
    } else if (n + 1 > fib_cap - 1) {
        ensure_cache(n + 1, &fib_cache, &fib_cap);
    }
    for (idx_t i = fib_max + 1; i <= n + 1; ++i) {
        fib_cache[i] = fib_cache[i - 1] + fib_cache[i - 2];
    }
    fib_max = n + 1;

    /* Compute values up to n if not already done */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t binom = binomial(i, k);
            seq_t prod = binom * cache[i - k];
            prod *= fib_cache[k + 1];
            sum += prod;
        }
        cache[i] = -sum;
    }
    max_computed = n;
    return cache[n];
}
