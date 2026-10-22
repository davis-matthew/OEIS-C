/*
 * A307117  Numbers x that are equal to lpf(x)*gpf(x)*(lpf(x)+gpf(x))/2, where
 *          lpf(x) and gpf(x) are the least and the greatest prime factors of
 *          x: A020639 and A006530.
 * https://oeis.org/A307117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer x>1 such that x = lpf(x) * gpf(x) *
 *            (lpf(x)+gpf(x)) / 2 (lpf = least prime factor, gpf = greatest
 *            prime factor)
 * Range:     u8 n <= 5, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   gpf, lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/lpf.h"
#include "../utils/gpf.h"

/* Cache of already found terms.  Index n in the sequence is 1‑based. */
static seq_t *cache     = NULL;   /* array of terms          */
static size_t cache_cap = 0;      /* allocated capacity      */
static size_t cache_len = 0;      /* number of terms stored  */

/* Largest integer tested so far.  The first tested value is 2. */
static idx_t tested = 1;

/* Grow the cache array when more room is needed. */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Return the nth term (n >= 1).  The function works in any call order. */
seq_t a(idx_t n)
{
    /* If we already have the required terms, return the cached value. */
    if (cache_len >= n) return cache[n - 1];

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        tested++;                    /* next integer to examine   */
        idx_t x = tested;            /* candidate integer         */

        idx_t lp = lpf(x);           /* least prime factor (idx_t) */
        uint64_t gp = gpf((uint64_t)x);  /* greatest prime factor        */

        /* Compute the expression using 128‑bit arithmetic to avoid overflow. */
        unsigned __int128 prod = (unsigned __int128)lp * (unsigned __int128)gp;
        prod *= (unsigned __int128)(lp + gp);
        prod /= 2;

        /* If the value matches the candidate, store it. */
        if (prod == (unsigned __int128)x) {
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = (seq_t)prod;    /* prod fits into seq_t here   */
        }
    }

    return cache[n - 1];
}
