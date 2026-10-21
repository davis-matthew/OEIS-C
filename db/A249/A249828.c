/*
 * A249828  a(n+1) is the next smallest nontrivial cube beginning with a(n),
 *          initial term is 6.
 * https://oeis.org/A249828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the next smallest non‑trivial cube that begins with the
   previous term.  a(1)=6.  The algorithm scans increasing cube
   roots, testing for the required prefix, and memoises all terms
   for fast subsequent queries. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper: integer cube root floor for a seq_t value. */
static seq_t floor_cube_root(seq_t x)
{
    if (x == (seq_t)0)
        return (seq_t)0;
    seq_t low  = (seq_t)1;
    seq_t high = x;
    while (low <= high) {
        seq_t mid = low + (high - low) / (seq_t)2;
        /* If mid^2 > x then mid^3 > x */
        if (mid > x / mid) {
            high = mid - (seq_t)1;
            continue;
        }
        seq_t m2 = mid * mid;              /* mid^2 fits in seq_t */
        if (mid > x / m2) {                /* mid^3 > x */
            high = mid - (seq_t)1;
        } else {
            low = mid + (seq_t)1;
        }
    }
    return high;
}

/* Global cache for computed terms and their cube roots. */
static seq_t *seq_cache   = NULL;
static seq_t *root_cache  = NULL;
static size_t  cache_cap  = 0;
static idx_t   cache_size = 0;

/* Grow cache to accommodate at least 'need' elements. */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
    seq_t *tmp   = (seq_t *)realloc(seq_cache, sizeof(seq_t) * new_cap);
    seq_t *rtmp  = (seq_t *)realloc(root_cache, sizeof(seq_t) * new_cap);
    if (!tmp || !rtmp) {   /* allocation failed – abort further growth */
        return;
    }
    seq_cache  = tmp;
    root_cache = rtmp;
    cache_cap  = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;

    if (!seq_cache) {
        grow_cache();                    /* initialise caches */
    }

    /* Compute terms up to n if needed. */
    while (cache_size < n) {
        if (cache_size == (idx_t)0) {
            /* a(1) = 6 */
            seq_cache[0]   = (seq_t)6;
            root_cache[0]  = floor_cube_root(seq_cache[0]);  /* 1 */
            cache_size = (idx_t)1;
            continue;
        }

        seq_t prefix      = seq_cache[cache_size - (idx_t)1];
        seq_t root_prev   = root_cache[cache_size - (idx_t)1];
        seq_t pow10k      = (seq_t)1;      /* 10^k for current k */

        /* Search over increasing k until a suitable cube is found. */
        while (1) {
            seq_t low  = prefix * pow10k;
            seq_t high = (prefix + (seq_t)1) * pow10k - (seq_t)1;

            /* Start root search just above the previous root. */
            seq_t start = floor_cube_root(low);
            seq_t min_root = root_prev + (seq_t)1;
            if (start < min_root) start = min_root;

            /* Iterate cube roots until cube falls outside [low,high]. */
            for (seq_t root = start; ; ++root) {
                seq_t cube = root * root * root;
                if (cube > high)       break;
                if (cube >= low && (cube % (seq_t)10) != (seq_t)0) {
                    /* Found the next term.  Store and return. */
                    if (cache_size >= cache_cap)
                        grow_cache();
                    seq_cache[cache_size]  = cube;
                    root_cache[cache_size] = root;
                    ++cache_size;
                    break;
                }
            }
            if (cache_size >= n)
                break;   /* desired term has been computed */

            /* Increase k for the next iteration. */
            pow10k *= (seq_t)10;
        }
    }

    return seq_cache[n - (idx_t)1];
}
