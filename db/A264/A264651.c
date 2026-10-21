/*
 * A264651  Number of (n+1)X(4+1) arrays of permutations of 0..n*5+4 with each
 *          element having directed index change 1,1 2,2 -1,0 or 0,-1.
 * https://oeis.org/A264651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows a linear recurrence a(n) = 3*a(n-1) + 36*a(n-3) -
 *            18*a(n-4) + 186*a(n-5) + 83*a(n-6) + 144*a(n-7) + 684*a(n-8) -
 *            1872*a(n-9) + 508*a(n-10) - 5048*a(n-11) - 3240*a(n-12) +
 *            720*a(n-13) - 95040*a(n-14) + 3072*a(n-15) + 3456*a(n-16) +
 *            13824*a(n-18) for n > 21 with the first 22 terms list...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[22] = {
    (seq_t)0ULL,
    (seq_t)9ULL,
    (seq_t)22ULL,
    (seq_t)112ULL,
    (seq_t)716ULL,
    (seq_t)3100ULL,
    (seq_t)14871ULL,
    (seq_t)73900ULL,
    (seq_t)349328ULL,
    (seq_t)1679736ULL,
    (seq_t)8094842ULL,
    (seq_t)38736132ULL,
    (seq_t)186037049ULL,
    (seq_t)893328303ULL,
    (seq_t)4285139965ULL,
    (seq_t)20570385615ULL,
    (seq_t)98729249057ULL,
    (seq_t)473793905479ULL,
    (seq_t)2274044573512ULL,
    (seq_t)10914002589581ULL,
    (seq_t)52379748049810ULL,
    (seq_t)251394035040131ULL
};

static seq_t *cache = NULL;               /* 1‑based index array */
static size_t cache_cap = 0;              /* allocated elements */
static idx_t  cache_len = 0;              /* highest n computed */

/* Ensure the cache can hold indices up to n.  Use exponential growth. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap < (size_t)n + 1) new_cap <<= 1;
    seq_t *new_buf = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_buf) { /* fall back to static allocation if realloc fails */ }
    cache = new_buf;
    cache_cap = new_cap;
    /* initialise new slots (though they will be overwritten anyway) */
}

seq_t a(idx_t n)
{
    /* lazily initialise cache */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
    }

    if (n > cache_len) {
        /* compute all missing values up to n */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            ensure_cache(i);
            if (i <= 22) {
                cache[i] = init[i-1];
                continue;
            }

            /* compute via the recurrence; add positives first, then subtract negatives */
            seq_t t = (seq_t)0;
            t += (seq_t)3   * cache[i-1];
            t += (seq_t)36  * cache[i-3];
            t += (seq_t)186 * cache[i-5];
            t += (seq_t)83  * cache[i-6];
            t += (seq_t)144 * cache[i-7];
            t += (seq_t)684 * cache[i-8];
            t += (seq_t)508 * cache[i-10];
            t += (seq_t)720 * cache[i-13];
            t += (seq_t)3072 * cache[i-15];
            t += (seq_t)3456 * cache[i-16];
            t += (seq_t)13824 * cache[i-18];

            /* subtract the negative‑coefficient terms */
            t -= (seq_t)18    * cache[i-4];
            t -= (seq_t)1872  * cache[i-9];
            t -= (seq_t)5048  * cache[i-11];
            t -= (seq_t)3240  * cache[i-12];
            t -= (seq_t)95040 * cache[i-14];

            cache[i] = t;
        }
        cache_len = n;
    }

    return cache[n];
}
