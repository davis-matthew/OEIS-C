/*
 * A261109  Number of (n+2) X (4+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00010001 00010101 or 01010101.
 * https://oeis.org/A261109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +6*a(n-3) +8*a(n-4) +10*a(n-5) +11*a(n-6)
 *            +7*a(n-7) +a(n-8) -7*a(n-9) -6*a(n-10) +a(n-11) +3*a(n-12)
 *            +a(n-13) (n > 14) Initial values: a(1)=97, a(2)=206, a(3)=566,
 *            a(4)=1498, a(5)=3908, a(6)=10268, a(7)=27030, a(8)=70980,
 *            a(9)=186290, a(10)=489516, a(11)=1286458, a(12)=3379154, a(13...
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 19, u64 n <= 42, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed values */
    static seq_t *cache = NULL;      /* index n holds a(n) */
    static size_t cap   = 0;         /* allocated capacity in elements */
    static idx_t last   = 0;         /* largest n for which cache[n] is valid */

    /* Initial values for n = 1 .. 14 */
    static const seq_t init[15] = {
        0,            /* dummy for index 0 */
        97,   206,    566,   1498,   3908,
       10268, 27030,  70980, 186290, 489516,
      1286458, 3379154, 8876106, 23319066  /* a(14) */
    };

    /* Allocate cache and copy initial values */
    if (!cache) {
        cap   = 15;                     /* indices 0 .. 14 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < 15; ++i)
            cache[i] = init[i];
        last = 14;
    }

    if (n <= last)
        return cache[n];

    /* Grow cache so that indices up to n are valid */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap    = new_cap;
    }

    /* Compute new entries from last+1 up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* For i <= 14 we already have initial values.  The recurrence only
           applies for i > 14, but this loop will simply copy the stored
           values for those indices.  */
        if (i <= 14) {
            cache[i] = init[i];
            continue;
        }

        seq_t val = cache[i-1];
        val += cache[i-3] * 6;
        val += cache[i-4] * 8;
        val += cache[i-5] * 10;
        val += cache[i-6] * 11;
        val += cache[i-7] * 7;
        val += cache[i-8];
        val -= cache[i-9] * 7;
        val -= cache[i-10] * 6;
        val += cache[i-11];
        val += cache[i-12] * 3;
        val += cache[i-13];
        cache[i] = val;
    }

    last = n;
    return cache[n];
}
