/*
 * A305581  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2, 7
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 73, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A305581
   a(1)=4, a(2)=3, a(3)=6, a(4)=8, a(5)=19; for n>5:
   a(n)=a(n-1)+a(n-2)+a(n-3) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    static seq_t *cache = NULL;
    static idx_t cap = 0; /* number of terms stored, cache[0]...cache[cap-1] correspond to a(1)...a(cap) */
    if (n > cap)
    {
        idx_t new_cap = n;
        /* realloc with explicit cast for C++ */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        for (idx_t i = cap; i < new_cap; ++i)
        {
            if (i == 0)
                cache[i] = (seq_t)4;
            else if (i == 1)
                cache[i] = (seq_t)3;
            else if (i == 2)
                cache[i] = (seq_t)6;
            else if (i == 3)
                cache[i] = (seq_t)8;
            else if (i == 4)
                cache[i] = (seq_t)19;
            else
                cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 3];
        }
        cap = new_cap;
    }
    return cache[n - 1];
}
