/*
 * A301289  Coordination sequence for a tetravalent node of type 3.4.3.12 in
 *          "cph" 2-D tiling (or net).
 * https://oeis.org/A301289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - 3*a(n-2) + 4*a(n-3) - 4*a(n-4) +
                     4*a(n-5) - 3*a(n-6) + 2*a(n-7) - a(n-8);
   initial values: a(0)=1, a(1)=4, a(2)=5, a(3)=6, a(4)=12, a(5)=14,
                    a(6)=15, a(7)=18, a(8)=21, a(9)=26. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t capacity = 0;      /* allocated size of cache          */
    static size_t last    = 0;       /* largest index computed so far    */
    static const seq_t base[10] =
        {1,4,5,6,12,14,15,18,21,26};

    size_t need = (size_t)n + 1;      /* number of slots required       */

    /* Allocate or grow the cache if necessary */
    if (capacity < need) {
        size_t newcap = capacity ? capacity * 2 : 10;
        while (newcap < need) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failure (unlikely)   */
        cache = tmp;
        capacity = newcap;
        /* initialise the first 10 terms on first allocation */
        if (capacity >= 10 && last < 9) {
            for (size_t i = 0; i < 10; ++i) cache[i] = base[i];
            last = 9;
        }
    }

    if (n <= last)
        return cache[n];

    /* Compute any missing terms up to n */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        seq_t val = 0;
        val += 2 * cache[i-1];
        val -= 3 * cache[i-2];
        val += 4 * cache[i-3];
        val -= 4 * cache[i-4];
        val += 4 * cache[i-5];
        val -= 3 * cache[i-6];
        val += 2 * cache[i-7];
        val -= cache[i-8];
        cache[i] = val;
    }
    last = (size_t)n;
    return cache[n];
}
