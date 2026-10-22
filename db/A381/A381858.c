/*
 * A381858  a(n) is the number of permutations of [n] that avoid 312 and 4321
 *          and whose square avoids 231.
 * https://oeis.org/A381858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 56, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+a(n-2)+2*a(n-3)+3*a(n-4)+2*a(n-5)
   with initial values a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=12 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap  = 0;   /* current allocation size       */
    static size_t len  = 0;   /* number of values computed     */

    size_t tn = (size_t)n;   /* local copy of index          */

    /* Initialise cache and first five terms if this is the first call */
    if (cache == NULL)
    {
        /* We need space for at least the first five terms and whatever
           the caller asked for.  Allocate the maximum of 5 or tn+1.     */
        cap = (tn < 5 ? 5 : tn + 1);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)
            exit(1);  /* memory allocation failed */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)5;
        cache[4] = (seq_t)12;
        len = 5;
    }

    /* Resize cache if it is not big enough for request */
    if (tn + 1 > cap)
    {
        cap = tn + 1;
        seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (tmp == NULL)
            exit(1);  /* memory allocation failed */
        cache = tmp;
    }

    /* Return already computed value if possible */
    if (tn < 5)
        return cache[tn];
    if (tn < len)
        return cache[tn];

    /* Compute missing terms up to tn */
    for (size_t i = len; i <= tn; ++i)
    {
        cache[i] = cache[i - 1] + cache[i - 2]
                 + (cache[i - 3] * (seq_t)2)
                 + (cache[i - 4] * (seq_t)3)
                 + (cache[i - 5] * (seq_t)2);
    }
    len = tn + 1;
    return cache[tn];
}
