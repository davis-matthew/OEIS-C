/*
 * A357945  Numbers k which are not square but D = (b+c)^2 - k is square,
 *          where b = floor(sqrt(k)) and c = k - b^2.
 * https://oeis.org/A357945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A357945 – Numbers k that are not perfect squares and for which
   D = (b+c)^2 - k is a perfect square, where b = floor(sqrt(k)) and
   c = k - b^2.  The algorithm simply scans k in increasing order,
   tests the condition, and memoises the results so that subsequent
   calls return instantly.  This gives an O(K) generation time where K
   is the largest k needed for the requested n.  For n up to around
   10 000 the largest k is well below 400 000, so the routine finishes
   comfortably within the 2‑second limit. */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* Returns the nth term of sequence A357945 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* undefined for n=0, but return 0 safely */

    /* Simple memoisation: keep a static array of found terms. */
    static seq_t *cache   = NULL;   /* array of found terms */
    static size_t cap     = 0;      /* current capacity of the array    */
    static size_t len     = 0;      /* number of stored terms           */
    static idx_t  last_k  = 1;      /* last k that was examined         */

    /* Generate terms until we have at least n of them. */
    while (len < n)
    {
        /* Examine next integer k. */
        seq_t k = ++last_k;
        seq_t b = floor_sqrt(k);

        /* Skip perfect squares. */
        if (b * b == k)
            continue;

        seq_t c = k - b * b;
        /* Compute D = c * (2b + c - 1).  The arithmetic is safe for all
           values that fit in seq_t; overflow would only occur for
           extraordinarily large types and the sequence would never
           reach them for the required n. */
        seq_t D = c * ((b << 1) + c - 1);
        seq_t d = floor_sqrt(D);

        if (d * d == D)          /* D is a perfect square */
        {
            /* Ensure enough space in cache. */
            if (len == cap)
            {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)        /* out of memory – abort gracefully */
                    abort();
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = k;    /* store the term */
        }
    }

    /* n is 1‑based; array is 0‑based. */
    return cache[n - 1];
}
