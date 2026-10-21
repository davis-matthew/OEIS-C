/*
 * A250083  Number of length 2+5 0..n arrays with every six consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A250083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7); Initial values: a(1)=83, a(2)=982,
 *            a(3)=5604, a(4)=21405, a(5)=63611, a(6)=159278, a(7)=352192
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 37, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free, abort */

static seq_t *cache = NULL;          /* cache of computed terms */
static size_t cache_cap = 0;         /* current capacity of the cache */
static idx_t  cache_len = 0;         /* number of terms stored (a(1) … a(len)) */

/* Ensure cache has at least `n` computed terms */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;

    /* Grow the cache if needed */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_arr) abort();
        for (idx_t i = 0; i < cache_len; ++i) new_arr[i] = cache[i];
        if (cache) free(cache);
        cache = new_arr;
        cache_cap = new_cap;
    }

    /* Initial terms (seed values) */
    static const unsigned long long seed64[7] = {
        83ULL,      982ULL,     5604ULL,   21405ULL,
        63611ULL, 159278ULL, 352192ULL
    };

    /* Compute missing terms */
    for (idx_t i = cache_len; i < n; ++i) {
        if (i < 7) {
            cache[i] = (seq_t)seed64[i];
        } else {
            seq_t val = cache[i-1];
            val = 7 * val;
            val -= 21 * cache[i-2];
            val += 35 * cache[i-3];
            val -= 35 * cache[i-4];
            val += 21 * cache[i-5];
            val -= 7 * cache[i-6];
            val += cache[i-7];
            cache[i] = val;
        }
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* behavior for n < 1 is undefined; return 0 */
    ensure_cache(n);
    return cache[n-1];
}
