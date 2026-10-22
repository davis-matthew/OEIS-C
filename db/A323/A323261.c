/*
 * A323261  a(n) = A323260(n)/2.
 * https://oeis.org/A323261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2) - 5*a(n-3) + 7*a(n-4) - a(n-5)
 *            a(0)=0, a(1)=1, a(2)=3, a(3)=12, a(4)=48, a(5)=195 (Sequence
 *            A323261)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cached computed values: cache[0..size-1] are defined */
    static seq_t *cache = NULL;
    static size_t size = 0;      /* number of computed terms */
    static size_t cap   = 0;      /* current allocation size */

    /* initialise cache with first 6 terms if not yet done */
    if (size == 0) {
        cap = 6;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;                 /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)48;
        cache[5] = (seq_t)195;
        size   = 6;
    }

    /* extend cache if necessary */
    while ((size_t)size <= (size_t)n) {
        /* ensure enough capacity */
        if (size > cap - 1) {                     /* need more space */
            cap = cap * 2;
            if (cap < (size_t)n + 1)
                cap = (size_t)n + 1;
            seq_t *tmp = (seq_t *)realloc(cache,
                                         cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;           /* allocation failure */
            cache = tmp;
        }

        /* compute next term using the recurrence */
        seq_t val = 5 * cache[size - 1];
        val -= 3 * cache[size - 2];
        val -= 5 * cache[size - 3];
        val += 7 * cache[size - 4];
        val -= cache[size - 5];
        cache[size] = val;
        ++size;
    }

    return cache[n];
}
