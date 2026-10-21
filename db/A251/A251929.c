/*
 * A251929  Number of length n+2 0..2 arrays with the sum of the maximum minus
 *          the median of adjacent triples multiplied by some arrangement of
 *          +-1 equal to zero.
 * https://oeis.org/A251929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by the empirical linear recurrence a(n) =
 *            10*a(n-1) - 43*a(n-2) + 112*a(n-3) - 212*a(n-4) + 314*a(n-5) -
 *            371*a(n-6) + 366*a(n-7) - 292*a(n-8) + 172*a(n-9) - 78*a(n-10) +
 *            2*a(n-11) + 25*a(n-12) - 16*a(n-13) + 12*a(n-14) for n > 17. The
 *            first 17 terms are provided in the OEIS entry. T...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;      /* number of terms currently cached (max index)   */
static size_t cache_cap = 0;      /* capacity of the cache array                    */

static const seq_t init_terms[17] = {
    12ULL, 43ULL, 115ULL, 339ULL, 1047ULL, 3185ULL, 9614ULL, 28997ULL,
    87432ULL, 263315ULL, 792052ULL, 2380535ULL, 7151068ULL, 21473847ULL,
    64465845ULL, 193491115ULL, 580670152ULL
};

/* Expand the cache so that it contains values up to and including [n] */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;                /* undefined, but protect against overflow */

    /* Allocation / resizing */
    if (cache == NULL) {
        cache_cap = (size_t)n > 32 ? (size_t)n : 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialise first 17 terms if not yet done */
    if (cache_len == 0) {
        for (size_t i = 0; i < 17; ++i) cache[i] = init_terms[i];
        cache_len = 17;
    }

    /* Compute missing terms up to n */
    if ((size_t)n > cache_len) {
        for (size_t k = cache_len + 1; k <= (size_t)n; ++k) {
            if (k <= 17) continue;       /* already initialised */

            idx_t kk = k;                /* keep as idx_t to avoid signed/unsigned mix */
            seq_t val = 0;
            val += 10 * cache[kk - 2];   /* a(k-1) */
            val -= 43 * cache[kk - 3];   /* a(k-2) */
            val += 112 * cache[kk - 4];  /* a(k-3) */
            val -= 212 * cache[kk - 5];  /* a(k-4) */
            val += 314 * cache[kk - 6];  /* a(k-5) */
            val -= 371 * cache[kk - 7];  /* a(k-6) */
            val += 366 * cache[kk - 8];  /* a(k-7) */
            val -= 292 * cache[kk - 9];  /* a(k-8) */
            val += 172 * cache[kk -10];  /* a(k-9) */
            val -= 78 * cache[kk -11];   /* a(k-10) */
            val += 2  * cache[kk -12];   /* a(k-11) */
            val += 25 * cache[kk -13];   /* a(k-12) */
            val -= 16 * cache[kk -14];   /* a(k-13) */
            val += 12 * cache[kk -15];   /* a(k-14) */

            cache[k - 1] = val;
        }
        cache_len = (size_t)n;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is defined for n>=1 */
    ensure_cache(n);
    return cache[n - 1];
}
