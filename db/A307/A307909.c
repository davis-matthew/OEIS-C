/*
 * A307909  Number of binary strings of length n whose only palindromic
 *          prefixes are "1" and "11".
 * https://oeis.org/A307909
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A307909 – Number of binary strings of length n whose only palindromic
    prefixes are “1” and “11”.  The recurrence used is:

        a(2)  = 1
        a(3)  = 1
        a(2k)   = 2*a(2k-1) - a(k+1) + a(k)
        a(2k+1) = 2*a(2k)   - a(k+1)   (k ≥ 2)
*/

#include <stdlib.h>
#include <stddef.h>

/* Static cache for memoisation ------------------------------------------------*/
static seq_t *cache      = NULL;   /* array a[0..max_cap]  */
static idx_t  cache_cap  = 0;      /* allocated size (>= n+1) */
static idx_t  max_computed = 3;    /* we know a[2] and a[3] */

/* Initialise base values and allocate cache if needed */
static void init_cache(idx_t n)
{
    if (cache)
        return;        /* already initialised */

    /* choose initial capacity large enough to hold the requested n at once */
    size_t init_cap = (n >= 4 ? n * 2 : 8);   /* arbitrary growth factor */
    cache = (seq_t *)malloc(sizeof(seq_t) * (init_cap + 1));
    if (!cache) exit(1);                      /* cannot recover */

    cache_cap = init_cap;

    /* set the prescribed initial values */
    cache[2] = (seq_t)1;
    cache[3] = (seq_t)1;
    max_computed = 3;
}

/* Ensure that a[0..n] are computed and stored in cache */
static void ensure_cache(idx_t n)
{
    if (n <= max_computed)
        return;

    /* Expand cache capacity if required */
    if (n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < n)
            new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (new_cap + 1));
        if (!tmp) exit(1);      /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute successive terms until n is reached */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 2) { cache[i] = (seq_t)1; continue; }
        if (i == 3) { cache[i] = (seq_t)1; continue; }

        if ((i & 1) == 0) {           /* i = 2k  */
            idx_t k = i >> 1;
            cache[i] = 2 * cache[i-1] - cache[k+1] + cache[k];
        } else {                       /* i = 2k+1 */
            idx_t k = (i - 1) >> 1;
            cache[i] = 2 * cache[i-1] - cache[k+1];
        }
    }
    max_computed = n;
}

/* --------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;         /* undefined behaviour for n < 2 */

    /* initialise cache on first call */
    if (!cache)
        init_cache(n);

    /* enlarge cache and compute missing values */
    ensure_cache(n);

    return cache[n];
}
