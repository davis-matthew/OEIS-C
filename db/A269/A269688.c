/*
 * A269688  Number of length-n 0..6 arrays with no repeated value differing
 *          from the previous repeated value by plus or minus one modulo 6+1.
 * https://oeis.org/A269688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 31*a(n-2) - 30*a(n-3) with initial values
 *            a(1)=7, a(2)=49, a(3)=343
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Persistent cache for memoisation */
static seq_t *cache = NULL;       /* Array of computed values, indexed by n (1-based) */
static idx_t cache_cap = 0;      /* Current allocated capacity */
static idx_t cache_max = 0;      /* Largest n whose value is computed */

/* Ensure the cache has space for indices up to at least n (inclusive) */
static void ensure_cache_cap(idx_t n)
{
    if (n + 1 > cache_cap) {
        /* grow capacity (double strategy) */
        idx_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        if (new_cap <= n + 1) new_cap = n + 1;
        seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (cache) {
            memcpy(new_arr, cache, cache_cap * sizeof(seq_t));
            free(cache);
        }
        cache = new_arr;
        cache_cap = new_cap;
    }
}

/* Initialise base terms if not already done */
static void init_base_terms(void)
{
    if (cache_max >= 3) return; /* already initialised */
    cache[1] = (seq_t)7;
    cache[2] = (seq_t)49;
    cache[3] = (seq_t)343;
    cache_max = 3;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n<1, return 0 for safety */
    ensure_cache_cap(n);
    init_base_terms();

    /* compute missing values up to n (if any) */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* a(i) = 12*a(i-1) - 31*a(i-2) - 30*a(i-3) */
        cache[i] = ((seq_t)12) * cache[i-1] - ((seq_t)31) * cache[i-2] - ((seq_t)30) * cache[i-3];
    }
    cache_max = (n > cache_max) ? n : cache_max;
    return cache[n];
}
