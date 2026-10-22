/*
 * A329162  a(n) = Sum_{k<n} ((2^n-1) mod (2^k-1)).
 * https://oeis.org/A329162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k<n} (2^(n mod k) - 1)
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 254, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached powers of two: pow2_cache[i] = 2^i, i >= 0. */
static seq_t *pow2_cache = NULL;   /* array of seq_t */
static idx_t   pow2_cache_n = 0;   /* last valid index in the cache */

/* Ensure the cache contains all powers up to exp (inclusive). */
static void
ensure_pow2_cache(idx_t exp)
{
    if (pow2_cache_n >= exp)                 /* nothing to do */
        return;

    /* Grow the array to hold indices 0..exp. */
    seq_t *new_cache = (seq_t *)realloc(pow2_cache,
                                        (exp + 1) * sizeof(seq_t));
    if (!new_cache)                         /* allocation failure */
        exit(1);

    /* Initialise the new slice of the array. */
    idx_t start = pow2_cache ? pow2_cache_n + 1 : 1;
    if (!pow2_cache)                        /* brand‑new array */
        new_cache[0] = 1;                   /* 2^0 */

    /* Compute the missing powers of two. */
    for (idx_t i = start; i <= exp; ++i)
        new_cache[i] = new_cache[i - 1] * 2; /* multiply by 2 */

    pow2_cache = new_cache;
    pow2_cache_n = exp;
}

/* OEIS A329162: sum_{k<n} ((2^n-1) mod (2^k-1)) = sum_{k<n} (2^{n mod k}-1) */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;               /* a(1) = 0 */

    /* We need powers 2^0 … 2^(n-1). */
    ensure_pow2_cache(n - 1);

    seq_t sum = 0;
    for (idx_t k = 1; k < n; ++k) {
        idx_t r = n % k;               /* n mod k, 0 <= r < k */
        seq_t term = pow2_cache[r] - 1;/* 2^r - 1 */
        sum += term;
    }

    return sum;
}
