/*
 * A292478  p-INVERT of A010892, where p(S) = 1 - S - S^3.
 * https://oeis.org/A292478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 32, u64 n <= 65, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A292478
   Formula: a(n) = 4*a(n-1) - 8*a(n-2) + 11*a(n-3) - 8*a(n-4) + 4*a(n-5) - a(n-6)
   Initial values:
     a(0) = 1, a(1) = 2, a(2) = 4, a(3) = 9,
     a(4) = 19, a(5) = 36, a(6) = 66
   The implementation uses memoisation to handle arbitrary call order.
   It computes missing terms using the recurrence while storing all
   computed values in a dynamic array. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cached = 0;   /* number of terms stored */
    static size_t capacity = 0; /* current allocated size */

    /* initialisation on first call */
    if (cached == 0)
    {
        /* reserve capacity for initial 7 terms plus a little extra */
        capacity = 16;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* memory allocation failed; undefined behaviour otherwise */

        /* fill initial terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)9;
        cache[4] = (seq_t)19;
        cache[5] = (seq_t)36;
        cache[6] = (seq_t)66;
        cached = 7;
    }

    /* extend cache if necessary */
    if ((size_t)n + 1 > capacity)
    {
        size_t new_cap = capacity * 2;
        if (new_cap <= (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        cache = tmp;
        capacity = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cached; i <= n; ++i)
    {
        if (i < 7)
        {
            /* initial terms already set, nothing to do */
            continue;
        }
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        seq_t t4 = cache[i - 4];
        seq_t t5 = cache[i - 5];
        seq_t t6 = cache[i - 6];

        /* compute sum1 = 4*t1 + 11*t3 + 4*t5
           compute sum2 = 8*t2 + 8*t4 + t6
           then a(i) = sum1 - sum2  (sum1 >= sum2 for this sequence) */
        seq_t sum1 = 4 * t1 + 11 * t3 + 4 * t5;
        seq_t sum2 = 8 * t2 + 8 * t4 + t6;
        cache[i] = sum1 - sum2;
    }

    cached = (size_t)n + 1; /* updated cached size */
    return cache[n];
}
