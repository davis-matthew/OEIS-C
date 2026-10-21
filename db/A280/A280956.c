/*
 * A280956  Number of nX3 0..2 arrays with no element equal to more than one
 *          of its horizontal, vertical and antidiagonal neighbors and with
 *          new values introduced in order 0 sequentially upwards.
 * https://oeis.org/A280956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A280956: Number of n×3 arrays with the stated constraints.
   Recurrence (for n > 16):
       a(n) = 5*a(n-1) + 8*a(n-2) + 5*a(n-3) + 4*a(n-4)
              - 35*a(n-5) + 2*a(n-6) + 71*a(n-7) - 11*a(n-8)
              - 38*a(n-9) + 6*a(n-10) - 8*a(n-11) - 8*a(n-12)
   Initial values (1–16):
       4, 42, 241, 1554, 9899, 63085, 402077, 2562733,
       16334111, 104108376, 663554669, 4229292041,
       26956196587, 171810442122, 1095066516245, 6979614612419 */

#include <stdlib.h>

/* ------------------------------------------------------------ */
/* C++Ｏptions (unordered above, but same code works in C++)  */
/* ------------------------------------------------------------ */
#ifdef __cplusplus
static const seq_t init_values[17] = {
    0ULL,                       /* unused index 0 */
    4ULL, 42ULL, 241ULL, 1554ULL, 9899ULL, 63085ULL, 402077ULL,
    2562733ULL, 16334111ULL, 104108376ULL, 663554669ULL,
    4229292041ULL, 26956196587ULL, 171810442122ULL,
    1095066516245ULL, 6979614612419ULL
};

/* Simple cache for memoisation */
static seq_t *cache   = NULL;
static size_t cache_size = 0;  /* largest n for which cache[n] is valid */
static size_t cache_cap  = 0;  /* allocated size of cache (elements) */

seq_t a(idx_t n)
{
    if (n == 0) {               /* outside offset – undefined */
        return 0;
    }
    if (n <= 16) {              /* use initial values directly */
        return init_values[n];
    }

    /* lazily allocate and initialise cache */
    if (!cache) {
        cache_cap  = 18;            /* need indices 0 .. 17 for n = 17 */
        cache      = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 1; i <= 16; ++i)
            cache[i] = init_values[i];
        cache[0] = 0;
        cache_size = 16;
    }

    /* enlarge cache if needed: need capacity >= n+1 */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing terms up to n */
    if ((size_t)n > cache_size) {
        for (size_t i = cache_size + 1; i <= (size_t)n; ++i) {
            /* Use a temporary sequence type that can hold intermediate
               results.  For unsigned seq_t this is still safe because
               the "positive part" is always >= the "negative part". */
            seq_t val = 5 * cache[i-1]
                      + 8 * cache[i-2]
                      + 5 * cache[i-3]
                      + 4 * cache[i-4]
                      + 2 * cache[i-6]
                      + 71 * cache[i-7]
                      + 6 * cache[i-10];
            val -= 35 * cache[i-5];
            val -= 11 * cache[i-8];
            val -= 38 * cache[i-9];
            val -= 8  * cache[i-11];
            val -= 8  * cache[i-12];
            cache[i] = val;
        }
        cache_size = n;
    }

    return cache[(size_t)n];
}

#else /* Plain C implementation (requires unsigned __int128) */

/* ------------------------------------------------------------ */
/* Initial values for 1..16 */
static const seq_t init_values[17] = {
    0ULL,
    4ULL, 42ULL, 241ULL, 1554ULL, 9899ULL, 63085ULL, 402077ULL,
    2562733ULL, 16334111ULL, 104108376ULL, 663554669ULL,
    4229292041ULL, 26956196587ULL, 171810442122ULL,
    1095066516245ULL, 6979614612419ULL
};

static seq_t *cache   = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n <= 16) return init_values[n];

    if (!cache) {
        cache_cap  = 18;
        cache      = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 1; i <= 16; ++i)
            cache[i] = init_values[i];
        cache[0] = 0;
        cache_size = 16;
    }

    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    if ((size_t)n > cache_size) {
        for (size_t i = cache_size + 1; i <= (size_t)n; ++i) {
            /* Use unsigned __int128 to avoid overflow of intermediate sums */
            unsigned __int128 v = 0;
            v += (unsigned __int128)cache[i-1] * 5;
            v += (unsigned __int128)cache[i-2] * 8;
            v += (unsigned __int128)cache[i-3] * 5;
            v += (unsigned __int128)cache[i-4] * 4;
            v += (unsigned __int128)cache[i-6] * 2;
            v += (unsigned __int128)cache[i-7] * 71;
            v += (unsigned __int128)cache[i-10]* 6;
            v -= (unsigned __int128)cache[i-5] * 35;
            v -= (unsigned __int128)cache[i-8] * 11;
            v -= (unsigned __int128)cache[i-9] * 38;
            v -= (unsigned __int128)cache[i-11]* 8;
            v -= (unsigned __int128)cache[i-12]* 8;
            cache[i] = (seq_t)v;
        }
        cache_size = n;
    }

    return cache[(size_t)n];
}
#endif
