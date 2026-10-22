/*
 * A301301  Coordination sequence for node of type V2 in "krq" 2-D tiling (or
 *          net).
 * https://oeis.org/A301301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n‑5) – a(n‑10) + b(n) where b(n) =
 *            {1,4,8,12,16,18,17,14,10,7,4,1,1,0,-2,-1,-1,-1} for n=0..17 and
 *            b(n)=0 for n≥18. a(0)=1, a(1)=4, ... are produced automatically
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Signed coefficients of the numerator –P(x) for n = 0..17 */
static const int16_t B[18] = {
    1,  /* n = 0 */
    4,  /* n = 1 */
    8,  /* n = 2 */
    12, /* n = 3 */
    16, /* n = 4 */
    18, /* n = 5 */
    17, /* n = 6 */
    14, /* n = 7 */
    10, /* n = 8 */
    7,  /* n = 9 */
    4,  /* n =10 */
    1,  /* n =11 */
    1,  /* n =12 */
    0,  /* n =13 */
   -2, /* n =14 */
   -1, /* n =15 */
   -1, /* n =16 */
   -1  /* n =17 */
};

/* Dynamic cache of computed values */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;

/* Grow the cache to hold values up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;

    /* Allocate new storage */
    size_t new_len = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;

    /* Compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = 0;

        /* 2*a(i-5)  (terms with negative index are treated as 0) */
        if (i >= 5) {
            val = cache[i-5] + cache[i-5];
        }

        /* –a(i-10) */
        if (i >= 10) {
            val -= cache[i-10];
        }

        /* Add b(i) if i ≤ 17 */
        if (i < 18) {
            int coeff = B[i];
            if (coeff >= 0) {
                val += (seq_t)coeff;
            } else {
                val -= (seq_t)(-coeff);
            }
        }

        cache[i] = val;
    }
    cache_len = new_len;
}

/* Requested sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
