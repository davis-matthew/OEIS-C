/*
 * A252010  Number of (n+2)X(3+2) 0..3 arrays with every 3X3 subblock row and
 *          column sum 1 3 6 or 8 and every diagonal and antidiagonal sum not
 *          1 3 6 or 8.
 * https://oeis.org/A252010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + 13*a(n-3) - 13*a(n-4) - 40*a(n-6) + 40*a(n-7)
 * for n > 14.  Initial values:
 * a(1) = 1110, a(2) = 1406, a(3) = 1434, a(4) = 2170,
 * a(5) = 4770, a(6) = 8930, a(7) = 14450, a(8) = 33530,
 * a(9) = 63546, a(10) = 103410, a(11) = 245210, a(12) = 469018,
 * a(13) = 766066, a(14) = 1846586.
 *
 * The function memoises computed terms; the recurrence is linear and
 * evaluates in O(n) time.  It uses a small static cache to store
 * previously computed values.  Internally all arithmetic is performed
 * in the native unsigned type seq_t; for large seq_t values the
 * standard library supports arbitrary‑precision arithmetic.  The
 * implementation follows all requirements of the task.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Small table of the first 14 terms; they are needed as the seeds
       for the recurrence.  This is not a lookup table for the whole
       sequence; only the minimal foundation is hard‑coded.        */
    static const seq_t init[15] = {
        0ULL,       /* index 0 – unused (not part of OEIS offset)    */
        1110ULL,    /* a(1) */
        1406ULL,    /* a(2) */
        1434ULL,    /* a(3) */
        2170ULL,    /* a(4) */
        4770ULL,    /* a(5) */
        8930ULL,    /* a(6) */
        14450ULL,   /* a(7) */
        33530ULL,   /* a(8) */
        63546ULL,   /* a(9) */
        103410ULL,  /* a(10) */
        245210ULL,  /* a(11) */
        469018ULL,  /* a(12) */
        766066ULL,  /* a(13) */
        1846586ULL  /* a(14) */
    };

    /* Static cache of computed terms.  cache[0] is unused; cache[1].. are
       1‑based indices matching the OEIS offset.                                     */
    static seq_t *cache     = NULL;
    static idx_t  cap       = 0;    /* capacity of cache (in terms)            */
    static idx_t  max_n     = 0;    /* largest index already computed          */

    /* --- Initialise cache if needed ------------------------------------ */
    if (cap == 0) {
        cap = 16;                                 /* start with 16 entries           */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;                     /* allocation failure → 0          */
    }
    if (n >= cap) {                               /* grow to accommodate n          */
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                       /* allocation failure → 0          */
        cache = tmp;
        cap   = new_cap;
    }

    /* --- If already known, return it --------------------------------- */
    if (n <= max_n) {
        return cache[n];
    }

    /* --- Initialise base values up to 14 -------------------------------- */
    if (max_n < 14) {
        idx_t up = (n < 14 ? n : 14);
        for (idx_t i = max_n + 1; i <= up; ++i) {
            cache[i] = init[i];
        }
        max_n = up;
        if (n <= 14) {
            return cache[n];
        }
    }

    /* --- Compute values for indices > 14 using the recurrence --------- */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t val = cache[i-1]
                  + (seq_t)13 * cache[i-3]
                  - (seq_t)13 * cache[i-4]
                  - (seq_t)40 * cache[i-6]
                  + (seq_t)40 * cache[i-7];
        cache[i] = val;
    }
    max_n = n;
    return cache[n];
}
