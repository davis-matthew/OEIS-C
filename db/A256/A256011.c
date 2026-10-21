/*
 * A256011  Integers k with the property that the largest prime factor of k^2
 *          + 1 is less than k.
 * https://oeis.org/A256011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A256011: k  s.t. the largest prime factor of k^2+1 is < k.
    The function returns the nth such integer (a(1)=7).  It uses the
    utility function gpf() (greatest prime factor).  A simple cache is
    kept so calls may be made in any order.  Only <stdlib.h> and the
    gpf utility are required.  All arithmetic is done in 128‑bit to
    avoid overflow for realistic n. */

#include <stdlib.h>
#include "../utils/gpf.h"                 /* uint64_t gpf(uint64_t) */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;

    /* last k that was examined;
       k=7 is the first term, so initialise at 6 */
    static uint64_t cur_k = 6;

    while (len < (size_t)n)
    {
        ++cur_k;

        /* compute m = k^2 + 1 safely */
        unsigned __int128 tmp = (unsigned __int128)cur_k * cur_k + 1;
        uint64_t m = (uint64_t)tmp;   /* safe for k up to 2^32-1 */

        uint64_t lpf = gpf(m);        /* largest prime factor of m */

        if (lpf < cur_k)              /* satisfies the condition */
        {
            /* grow cache if necessary */
            if (len + 1 > cap)
            {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp_cache) abort();   /* allocation failure */
                cache = tmp_cache;
                cap = new_cap;
            }
            cache[len++] = (seq_t)cur_k;
        }
    }
    return cache[n - 1];
}
