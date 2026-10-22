/*
 * A367249  a(n) is the number of n-digit numbers whose difference between the
 *          largest and smallest digits is equal to 8.
 * https://oeis.org/A367249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 17*9^(n-1) - 31*8^(n-1) + 2*7^n; Recurrence: a(1)=0,
 *            a(2)=3, a(3)=79, a(n) = 24*a(n-1)-191*a(n-2)+504*a(n-3) for n>3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;  /* allocated capacity */
    static idx_t size  = 0;  /* largest n computed (>=0) */

    /* enlarge cache if needed */
    if (n > cap)
    {
        idx_t new_cap = (cap > 0) ? cap * 2 : 8;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* if already computed, return cached value */
    if (n <= size) return cache[n-1];

    /* compute terms from size+1 up to n */
    for (idx_t i = size + 1; i <= n; ++i)
    {
        seq_t val;
        if (i == 1)            val = (seq_t)0;
        else if (i == 2)       val = (seq_t)3;
        else if (i == 3)       val = (seq_t)79;
        else
        {
            /* recurrence: a(n) = 24*a(n-1)-191*a(n-2)+504*a(n-3) */
            val = (seq_t)24 * cache[i-2] -
                  (seq_t)191 * cache[i-3] +
                  (seq_t)504 * cache[i-4];
        }
        cache[i-1] = val;
    }

    size = n;
    return cache[n-1];
}
