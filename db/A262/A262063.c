/*
 * A262063  The first of seven consecutive positive integers the sum of the
 *          squares of which is equal to the sum of the squares of six
 *          consecutive positive integers.
 * https://oeis.org/A262063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=78, a(2)=2100, a(3)=54594; a(n)=27*a(n-1)-27*a(n-2)+a(n-3)
 *            for n>3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[0] = a(1), cache[1] = a(2), ... */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static size_t  count  = 0;       /* number of elements filled in cache */

    if (n == 0)
        return (seq_t)0;              /* invalid index, return 0 */

    /* first use: allocate and initialise the first three terms */
    if (cap == 0) {
        cap = 4;                     /* start with a small capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;         /* allocation failure */
        cache[0] = (seq_t)78;
        cache[1] = (seq_t)2100;
        cache[2] = (seq_t)54594;
        count   = 3;                 /* we've stored three terms now */
    }

    /* resize if the requested index is beyond the current capacity */
    if ((size_t)n > cap) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 4;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;         /* reallocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute required terms up to n */
    while (count < (size_t)n) {
        seq_t t1 = cache[count - 1];
        seq_t t2 = cache[count - 2];
        seq_t t3 = cache[count - 3];
        /* recurrence: a(k) = 27*a(k-1) - 27*a(k-2) + a(k-3) */
        cache[count] = (seq_t)((seq_t)27 * t1 - (seq_t)27 * t2 + t3);
        ++count;
    }

    return cache[n - 1];
}
