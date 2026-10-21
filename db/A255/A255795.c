/*
 * A255795  Number of (n+2)X(2+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum 1 and no antidiagonal sum 2 and no row sum 0 and no column sum
 *          3.
 * https://oeis.org/A255795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 49, u64 n <= 107, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A255795
 *
 *  Empirical recurrence (valid for n > 22):
 *      a(n) = a(n-2) + a(n-4) + 5*a(n-6)
 *             - 2*a(n-8) - 2*a(n-10) - 7*a(n-12)
 *             + a(n-14) + a(n-16) + 3*a(n-18)
 *
 *  Initial values (a(1) … a(24)):
 *    134, 166, 178, 262, 340, 472, 600, 856, 1172, 1713,
 *    2474, 3613, 5136, 7550, 10830, 15905, 23000, 33843,
 *    48844, 71957, 103968, 153034, 221354, 325990
 *
 *  The implementation keeps a memoised array so that calls in
 *  arbitrary order and for n up to several thousand have
 *  O(n) total cost.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init_vals[25] = {
    0ULL,          /* unused, index 0 */
    134ULL, 166ULL, 178ULL, 262ULL, 340ULL, 472ULL, 600ULL, 856ULL,
    1172ULL, 1713ULL, 2474ULL, 3613ULL, 5136ULL, 7550ULL, 10830ULL,
    15905ULL, 23000ULL, 33843ULL, 48844ULL, 71957ULL, 103968ULL,
    153034ULL, 221354ULL, 325990ULL
};

/* static cache for memoised values */
static seq_t *cache = NULL;      /* 1‑based indexing */
static size_t cache_cap = 0;     /* current allocated size */
static idx_t  last_computed = 0; /* largest n for which cache[n] is valid */

/* grow the cache to at least len elements (index 0 unused) */
static void grow_cache(idx_t len)
{
    if (cache_cap >= (size_t)len + 1)
        return;
    size_t new_cap = (cache_cap < 32) ? 32 : cache_cap;
    while (new_cap <= (size_t)len)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0)
        abort();                 /* unrecoverable allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* initialise cache with the 24 known terms */
static void init_cache(void)
{
    if (cache == NULL) {
        grow_cache(24);
        for (idx_t i = 1; i <= 24; ++i)
            cache[i] = init_vals[i];
        last_computed = 24;
    } else if (last_computed < 24) {
        /* fill any missing initial values */
        for (idx_t i = last_computed + 1; i <= 24; ++i)
            cache[i] = init_vals[i];
        last_computed = 24;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* undefined for n < 1 */

    if (n <= 24)
        return init_vals[n];

    if (cache == NULL)
        init_cache();

    if (n > last_computed) {
        grow_cache(n);
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* use the recurrence, all indices are >= 1 here */
            seq_t val = cache[i - 2] + cache[i - 4]
                      + 5 * cache[i - 6]
                      - 2 * cache[i - 8]
                      - 2 * cache[i -10]
                      - 7 * cache[i -12]
                      + cache[i -14]
                      + cache[i -16]
                      + 3 * cache[i -18];
            cache[i] = val;
        }
        last_computed = n;
    }

    return cache[n];
}
