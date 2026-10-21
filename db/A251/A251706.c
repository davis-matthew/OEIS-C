/*
 * A251706  6-step Fibonacci sequence starting with (0,0,0,0,1,0).
 * https://oeis.org/A251706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 22, u32 n <= 38, u64 n <= 70, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+6) = a(n)+a(n+1)+a(n+2)+a(n+3)+a(n+4)+a(n+5)
   Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=0 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of computed terms */
    static size_t cap   = 0;      /* current capacity of the cache   */
    static size_t next  = 0;      /* index of first not-yet-computed */

    /* 1) Allocate or grow the cache if necessary */
    size_t required = (size_t)(n + 1);
    if (required > cap)
    {
        size_t newcap = required;
        if (newcap < 64) newcap = 64;          /* avoid tiny reallocs */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;                   /* unlikely, defensive */
        cache = tmp;
        cap   = newcap;
    }

    /* 2) Initialise the first six terms on first use */
    if (next == 0)
    {
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 1;
        cache[5] = 0;
        next    = 6;
    }

    /* 3) Compute any missing terms up to n */
    if (next <= n)
    {
        for (size_t i = next; i <= n; ++i)
        {
            cache[i] = cache[i-1] + cache[i-2] + cache[i-3] +
                       cache[i-4] + cache[i-5] + cache[i-6];
        }
        next = n + 1;
    }

    return cache[(size_t)n];
}
