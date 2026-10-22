/*
 * A367545  a(n) = Sum_{k=0..n} (-2)^k * |(n - k | k)|, where (a | b) denotes
 *          the Kronecker symbol.
 * https://oeis.org/A367545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} [gcd(k,n) = 1] * (-2)^k
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Compute gcd of two unsigned 64‑bit values */
static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Static cache for memoisation */
static seq_t *cache_vals = NULL;          /* computed values */
static uint8_t *cache_computed = NULL;    /* cache flag: 0 = not computed, 1 = computed */
static size_t cache_cap = 0;              /* current allocated capacity */

/* Grow cache to accommodate index n (inclusive) */
static void ensure_cache(idx_t n) {
    if (n < cache_cap)
        return;
    size_t new_cap = n + 1;
    seq_t *new_vals   = (seq_t *)realloc(cache_vals, new_cap * sizeof(seq_t));
    uint8_t *new_flag = (uint8_t *)realloc(cache_computed, new_cap * sizeof(uint8_t));
    if (!new_vals || !new_flag) {
        /* Allocation failed – abort */
        abort();
    }
    /* Initialise new slots as not computed */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_flag[i] = 0;
    cache_vals     = new_vals;
    cache_computed = new_flag;
    cache_cap      = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Return cached result if available */
    if (n < cache_cap && cache_computed[n])
        return cache_vals[n];

    /* Ensure cache capacity */
    ensure_cache(n);

    seq_t sum = 0;
    seq_t pow = 1;  /* (-2)^0 */

    for (idx_t k = 0; k <= n; ++k) {
        if (gcd_idx(k, n) == 1)
            sum += pow;
        if (k < n)                    /* prepare (-2)^(k+1) for next step */
            pow *= (seq_t)-2;
    }

    /* Store result in cache */
    cache_vals[n]     = sum;
    cache_computed[n] = 1;
    return sum;
}
