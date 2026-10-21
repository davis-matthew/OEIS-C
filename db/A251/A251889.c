/*
 * A251889  Number of (n+2) X (3+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum not 2 3 6 or 7 and every diagonal and antidiagonal
 *          sum 2 3 6 or 7.
 * https://oeis.org/A251889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-3) - 6*a(n-4) - 8*a(n-6) + 8*a(n-7) for n
 *            > 12 Initial values (n = 1 … 12): 1280, 602, 556, 778, 1260,
 *            1636, 2478, 3940, 5958, 9178, 14534, 22666 Implementation
 *            memoises computed terms and grows the buffer as needed
 * Range:     u8 none, u16 n <= 14, u32 n <= 38, u64 n <= 86, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Initial terms (index 0 unused) */
static const seq_t _init[13] = {
    0ULL,      /* unused */
    1280ULL, 602ULL, 556ULL, 778ULL, 1260ULL, 1636ULL,
    2478ULL, 3940ULL, 5958ULL, 9178ULL, 14534ULL, 22666ULL
};

/* Cache for computed terms */
#if defined(__cplusplus)
static seq_t *cache = nullptr;
#else
static seq_t *cache = NULL;
#endif
static size_t cache_cap = 0;          /* allocated capacity (indices 0 … cache_cap-1) */
static idx_t  last_computed = 0;     /* largest index that has a valid value */

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) {
        return 0; /* the sequence is defined for n >= 1 */
    }

    /* Ensure the cache can hold n+1 entries (index 0 unused) */
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < need) new_cap = need;

#if defined(__cplusplus)
        /* Allocate new array using C++ new. */
        seq_t *new_arr = new seq_t[new_cap];
        if (cache) {
            for (size_t i = 0; i < cache_cap; ++i) {
                new_arr[i] = cache[i];
            }
            delete[] cache;
        }
#else
        /* Allocate or reallocate using C malloc/realloc. */
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_arr) abort();
#endif
        cache = new_arr;
        cache_cap = new_cap;
    }

    /* If this is the first time we use the cache, initialise the first
       twelve terms from the constant table. */
    if (last_computed == 0) {
        for (idx_t i = 1; i <= 12 && i < (idx_t)cache_cap; ++i) {
            cache[i] = _init[i];
        }
        last_computed = 12;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 12) {                    /* still within the table */
            cache[i] = _init[i];
            continue;
        }
        seq_t val = cache[i - 1];
        val += (seq_t)6 * cache[i - 3];
        val -= (seq_t)6 * cache[i - 4];
        val -= (seq_t)8 * cache[i - 6];
        val += (seq_t)8 * cache[i - 7];
        cache[i] = val;
    }

    if (last_computed < n) last_computed = n;
    return cache[n];
}
