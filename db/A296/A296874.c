/*
 * A296874  Numbers whose base-7 digits d(m), d(m-1), ..., d(0) have #(pits) >
 *          #(peaks); see Comments.
 * https://oeis.org/A296874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A296874: numbers whose base-7 digits have more pits than peaks. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for caching results */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t  last_checked = 0;

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        ++last_checked;                       /* next integer to test */
        seq_t v = last_checked;

        /* convert v to base‑7 and count pits/peaks */
        unsigned char digs[64];
        size_t dlen = 0;
        seq_t tmp = v;
        while (tmp > 0)
        {
            digs[dlen++] = (unsigned char)(tmp % 7);
            tmp /= 7;
        }

        if (dlen >= 3)
        {
            int pits  = 0;
            int peaks = 0;
            for (size_t pos = 1; pos + 1 < dlen; ++pos)
            {
                unsigned char left  = digs[pos - 1];
                unsigned char cur   = digs[pos];
                unsigned char right = digs[pos + 1];
                if (left > cur && cur < right) ++pits;
                if (left < cur && cur > right) ++peaks;
            }
            if (pits > peaks)               /* store term */
            {
                if (cache_len >= cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 1024;
                    seq_t *new_mem = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!new_mem) abort();  /* out of memory */
                    cache = new_mem;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = v;
            }
        }
    }

    return cache[(size_t)n - 1];
}
