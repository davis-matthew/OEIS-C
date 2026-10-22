/*
 * A370202  a(n) = a(n-3) + a(n-2) + gcd(a(n-2), a(n-1)) with a(1) = a(2) =
 *          a(3) = 1.
 * https://oeis.org/A370202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + a(n-2) + gcd(a(n-2), a(n-1)) a(1) = a(2) = a(3)
 *            = 1
 * Range:     u8 n <= 18, u16 n <= 37, u32 n <= 77, u64 n <= 156,
 *            u128 n <= 313, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

/* The function implements the recurrence and memoises all
   values that have ever been requested so far.  The cache is
   1‑based so that cache[1] holds a(1).  The array grows
   automatically when a larger index is asked for. */

seq_t a(idx_t n)
{
    /* Static state for the cache ------------------------------------- */
    static seq_t *cache = NULL;   /* array of computed terms, 1‑based */
    static size_t cap   = 0;      /* allocated size (including index 0) */
    static size_t sz    = 0;      /* largest index that has been computed */

    /* Ensure we have a cache big enough for index n. ----------------*/
    if (n + 1 > cap) {
        size_t new_cap = (cap == 0) ? 16 : cap << 1;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        /* realloc returns void*; cast it so the compiler accepts it
           in both C and C++. */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Out of memory – terminate with an indeterminate value. */
            return (seq_t)0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the first three terms once. ------------------------- */
    if (sz < 3) {
        cache[1] = cache[2] = cache[3] = (seq_t)1;
        sz = 3;
    }

    /* Compute any missing terms up to n. ---------------------------- */
    if (n > sz) {
        for (size_t i = sz + 1; i <= (size_t)n; ++i) {
            seq_t a_nm3 = cache[i - 3];
            seq_t a_nm2 = cache[i - 2];
            seq_t a_nm1 = cache[i - 1];
            seq_t g     = gcd(a_nm2, a_nm1);
            cache[i]    = a_nm3 + a_nm2 + g;
        }
        sz = (size_t)n;
    }

    /* Return the requested value ------------------------------------ */
    return cache[(size_t)n];
}
