/*
 * A282856  Number of nX2 0..1 arrays with no 1 equal to more than one of its
 *          horizontal, vertical and antidiagonal neighbors.
 * https://oeis.org/A282856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence: A282856
   Empirical recurrence: a(n) = a(n-1) + 4*a(n-2) + 3*a(n-3) + a(n-4) + a(n-5)
   Offset: 1  (a(1) = 4, a(2) = 11, a(3) = 31, a(4) = 89, a(5) = 251)
*/

#include <stdlib.h>

/* Seed values for the first five terms */
static const seq_t seeds[5] = {4, 11, 31, 89, 251};

/* Static cache for computed terms (1‑based). */
static seq_t *cache      = NULL; /* allocated array, index 0 unused */
static idx_t  cache_cap  = 0;    /* number of allocated slots (excluding 0) */
static idx_t  cache_last = 0;    /* largest index that has been computed */

/* Ensure the cache has space for at least `need` slots (≥ need). */
static void ensure_cache(idx_t need)
{
    if (need <= cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need)
        new_cap <<= 1;           /* double until big enough */

    /* realloc may return NULL on failure; abort in that case */
    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp)
        abort();

    cache     = tmp;
    cache_cap = new_cap;
}

/* Initialize the first five terms in the cache, if not already done. */
static void init_cache(void)
{
    if (cache_last >= 5)
        return;

    ensure_cache(5);
    cache[1] = seeds[0];
    cache[2] = seeds[1];
    cache[3] = seeds[2];
    cache[4] = seeds[3];
    cache[5] = seeds[4];
    cache_last = 5;
}

/*
   Public accessor: returns the nth term (n ≥ 1).
   Does not assume increasing order of calls.
   Undefined for n < 1; returns 0 in that case.
*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for n<1 */

    /* For n ≤ 5, use the seed table directly. */
    if (n <= 5)
        return seeds[n - 1];

    /* Ensure that the cache contains the first 5 terms. */
    init_cache();

    /* Extend the cache up to n. */
    if (n > cache_last) {
        ensure_cache(n);
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            cache[i] = cache[i - 1]
                     + (seq_t)4 * cache[i - 2]
                     + (seq_t)3 * cache[i - 3]
                     + cache[i - 4]
                     + cache[i - 5];
        }
        cache_last = n;
    }

    return cache[n];
}
