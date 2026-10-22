/*
 * A357308  a(0) = a(1) = 0, a(2) = 1; a(n) = a(n-1) + Sum_{k=0..n-3} a(k) *
 *          a(n-k-3).
 * https://oeis.org/A357308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 26, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A357308
   Recurrence: a(0)=a(1)=0, a(2)=1;
                a(n) = a(n-1) + sum_{k=0}^{n-3} a(k)*a(n-k-3)
   Note: the formula follows the gcc/clang-compatible C11/C++17 rules
*/

#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Cached evaluation of the sequence (memoisation)                      */
/* --------------------------------------------------------------------- */

/* The sequence cache: cache[i] holds a(i) for 0 ≤ i ≤ cache_cap-1          */
static seq_t *cache     = NULL;   /* array of computed values           */
static size_t cache_cap = 0;       /* current capacity (number of slots) */
static idx_t max_comp  = 2;         /* highest index whose value is valid */
static bool   initialized = false; /* whether base terms have been set    */

/* Ensure the cache has capacity for indices 0..n inclusive.             */
static void ensure_cache(idx_t n)
{
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        /* realloc may return NULL on failure; we quit if that happens */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            exit(1);               /* unrecoverable – abort   */
        }
        cache     = tmp;
        cache_cap = new_cap;
    }
}

/* Initialise the cache with the three known terms.                     */
static void init_cache(void)
{
    if (!initialized) {
        ensure_cache(2);
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 1;
        initialized = true;
    }
}

/* --------------------------------------------------------------------- */
/*  Public sequence function                                             */
/* --------------------------------------------------------------------- */

/* Computes a(n) for any n >= 0.
///
/// Memoises computed values so that repeated calls, in any order,
/// are efficient.  The implementation uses the recurrence shown above
/// and runs in O(n) time per call when the largest argument so far is
/// n (overall O(max_n)).
*/
seq_t a(idx_t n)
{
    init_cache();

    if (n <= max_comp)
        return cache[n];

    /* Extend the cache to hold the new element(s).                     */
    ensure_cache(n);

    /* Compute all missing values up to n.                              */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t sum = 0;
        idx_t limit = i - 3;
        /* sum_{k=0}^{limit} a(k)*a(limit-k) */
        for (idx_t k = 0; k <= limit; ++k)
            sum += cache[k] * cache[limit - k];

        cache[i] = cache[i - 1] + sum;
    }
    max_comp = n;
    return cache[n];
}
