/*
 * A273917  Number of ordered ways to write n as w^2 + 3*x^2 + y^4 + z^5,
 *          where w is a positive integer and x,y,z are nonnegative integers.
 * https://oeis.org/A273917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered ways to write n = w^2 + 3*x^2 + y^4 +
 *            z^5 with w a positive integer and x, y, z non‑negative integers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Static cache for the number of (w,x) solutions of
   k = w^2 + 3*x^2  (w ≥ 1, x ≥ 0).  The array grows
   on demand up to the largest argument seen. */
static seq_t *f_cache = NULL;   /* count array, size f_size+1 */
static idx_t  f_size  = 0;      /* largest k for which f_cache is valid */

/* Integer square root using the double sqrt for a quick initial
   estimate followed by corrections.  Safe for idx_t values up to 2^53. */
static inline idx_t isqrt_idx(idx_t n)
{
    idx_t r = (idx_t) sqrt((double) n);
    while ((r + 1) * (r + 1) <= n)  ++r;
    while (r * r > n)               --r;
    return r;
}

/* Compute base^exp with an upper bound `limit`.  
   If the true value would exceed `limit` the function sets *res
   to limit+1 and returns true.  Otherwise *res receives the value
   and the function returns false. */
static bool pow_u64_limit(idx_t base, int exp, idx_t limit, idx_t *res)
{
    idx_t r = 1;
    for (int i = 0; i < exp; ++i) {
        if (base == 0) { *res = 0; return false; }
        if (r > limit / base) { *res = limit + 1; return true; }
        r *= base;
    }
    *res = r;
    return false;
}

/* Ensure that f_cache has been filled for all k ≤ max_k. */
static void ensure_f_cache(idx_t max_k)
{
    if (max_k <= f_size) return;

    /* Allocate a larger cache. */
    size_t new_size = (size_t)(max_k + 1);
    seq_t *new_f = (seq_t *) calloc(new_size, sizeof(seq_t));
    if (!new_f) return;                 /* very unlikely on a reasonable machine */

    /* Fill the cache up to max_k. */
    idx_t w_limit = isqrt_idx(max_k);
    idx_t x_limit = isqrt_idx(max_k);   /* overestimate; we'll break early inside loop */

    for (idx_t w = 1; w <= w_limit; ++w) {
        idx_t w2 = w * w;
        if (w2 > max_k) break;          /* safety */
        for (idx_t x = 0; x <= x_limit; ++x) {
            idx_t x2 = x * x;
            idx_t val = w2 + 3 * x2;
            if (val > max_k) break;     /* further x will only increase the value */
            new_f[val]++;               /* count another (w,x) pair */
        }
    }

    /* Replace the old cache. */
    free(f_cache);
    f_cache = new_f;
    f_size  = max_k;
}

/* Main sequence function.  The function is thread‑safe for a single
   process as the static cache is only appended. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset 1: undefined for n == 0 */

    /* Prepare the cache of (w,x) counts up to n. */
    ensure_f_cache(n);

    seq_t total = 0;

    /* Iterate over z (the z^5 term). */
    for (idx_t z = 0;; ++z) {
        idx_t z5;
        if (pow_u64_limit(z, 5, n, &z5) || z5 > n) break;
        idx_t rem1 = n - z5;

        /* Iterate over y (the y^4 term). */
        for (idx_t y = 0;; ++y) {
            idx_t y4;
            if (pow_u64_limit(y, 4, rem1, &y4) || y4 > rem1) break;
            idx_t rem2 = rem1 - y4;   /* remaining for w^2 + 3*x^2 */

            /* Add the number of (w,x) solutions for rem2. */
            total += f_cache[rem2];
        }
    }

    return total;
}
