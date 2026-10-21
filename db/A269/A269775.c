/*
 * A269775  Number of length-n 0..7 arrays with every repeated value unequal
 *          to the previous repeated value plus one mod 7+1.
 * https://oeis.org/A269775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 42*a(n-2) - 49*a(n-3), a(1)=8, a(2)=64,
 *            a(3)=512
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_N 10001  /* supports n up to 10000 */

seq_t a(idx_t n)
{
    /* sequence is 1-indexed; n==0 is out of range */
    if (n < 1)
        return (seq_t)0;

    /* static cache of precomputed values */
    static seq_t cache[MAX_N];
    static idx_t cached_to = 0;  /* largest n that has been computed and stored */

    /* initialise first three terms once */
    if (cached_to == 0)
    {
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)64;
        cache[3] = (seq_t)512;
        cached_to = 3;
    }

    /* if requested n is beyond preallocated cache, compute on the fly */
    if (n >= MAX_N)
    {
        seq_t a1 = (seq_t)8;   /* a(1) */
        seq_t a2 = (seq_t)64;  /* a(2) */
        seq_t a3 = (seq_t)512; /* a(3) */
        if (n == 1) return a1;
        if (n == 2) return a2;
        if (n == 3) return a3;
        for (idx_t i = 4; i <= n; ++i)
        {
            seq_t next = (seq_t)14 * a3 - (seq_t)42 * a2 - (seq_t)49 * a1;
            a1 = a2;
            a2 = a3;
            a3 = next;
        }
        return a3;
    }

    /* if not yet computed up to n, finish computing */
    if (n > cached_to)
    {
        for (idx_t i = cached_to + 1; i <= n; ++i)
        {
            cache[i] = (seq_t)14 * cache[i - 1]
                     - (seq_t)42 * cache[i - 2]
                     - (seq_t)49 * cache[i - 3];
        }
        cached_to = n;
    }

    return cache[n];
}
