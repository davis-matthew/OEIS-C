/*
 * A322926  The successive approximations up to 2^n for 2-adic integer
 *          5^(1/3).
 * https://oeis.org/A322926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0,
   for n > 0: a(n) = a(n-1) if (a(n-1)^3 - 5) is divisible by 2^n,
   otherwise a(n-1) + 2^(n-1).  This implements the unique solution to
   x^3 ≡ 5 (mod 2^n) in [0,2^n−1]. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of computed terms and of powers of two */
    static seq_t *cache      = NULL; /* a(k) values */
    static size_t cache_len  = 0;    /* number of filled entries   */
    static size_t cache_cap  = 0;    /* allocated capacity         */

    static seq_t *pow2       = NULL; /* 2^k values */
    static size_t pow2_len   = 0;    /* number of filled entries */
    static size_t pow2_cap   = 0;    /* allocated capacity         */

    /* Convert idx_t to size_t for array indexing (n <= 10000 is safe) */
    size_t N = (size_t)n;

    /* Initialise caches on first call */
    if (cache_cap == 0) {
        cache_cap   = 2;
        cache_len   = 1;
        cache      = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));

        pow2_cap   = 2;
        pow2_len   = 1;
        pow2       = (seq_t *)realloc(NULL, pow2_cap * sizeof(seq_t));

        cache[0]   = (seq_t)0;      /* a(0) = 0 */
        pow2[0]    = (seq_t)1;      /* 2^0 = 1 */
    }

    /* Grow capacity if needed */
    if (N >= cache_len) {
        if (N + 1 > cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap <= N) new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            pow2  = (seq_t *)realloc(pow2,  new_cap * sizeof(seq_t));
            cache_cap = new_cap;
            pow2_cap  = new_cap;
        }

        /* Compute terms up to N */
        for (size_t k = cache_len; k <= N; ++k) {
            seq_t prev = cache[k - 1];

            /* Compute 2^k if not yet ready */
            if (k >= pow2_len) {
                pow2[k] = pow2[pow2_len - 1] << 1;
                pow2_len = k + 1;
            }
            seq_t mod = pow2[k];

            /* a(k) = a(k-1) if (a(k-1)^3 - 5) mod 2^k == 0,
               else a(k-1) + 2^(k-1) */
            seq_t diff = prev * prev * prev - (seq_t)5;
            if (diff % mod == (seq_t)0)
                cache[k] = prev;
            else
                cache[k] = prev + pow2[k - 1];
        }
        cache_len = N + 1;
    }

    return cache[N];
}
