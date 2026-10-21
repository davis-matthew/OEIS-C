/*
 * A291035  p-INVERT of (1,0,0,1,0,0,1,0,0,...), where p(S) = 1 - S - 2 S^2.
 * https://oeis.org/A291035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 2*a(n-3) - a(n-4) - a(n-6) for n >= 7
 *            Initial values: a(0)=1, a(1)=3, a(2)=5, a(3)=12, a(4)=27,
 *            a(5)=58, a(6)=130
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 112,
 *            big any n
 * Helpers:   alloc_seq_array, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_computed = (idx_t)-1;

    /* initial setup */
    if (cache == NULL)
    {
        /* need space for indices 0..6 */
        cap = 7;
        cache = alloc_seq_array(cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)27;
        cache[5] = (seq_t)58;
        cache[6] = (seq_t)130;
        max_computed = 6;
    }

    if (n <= max_computed)
        return cache[n];

    /* ensure enough capacity */
    idx_t needed = n + 1;
    if ((size_t)needed > cap)
        grow_seq_array(&cache, &cap, needed);

    /* compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        if (i <= 6) /* already initialized */
            continue;

        cache[i] = cache[i-1]
                 + 2*cache[i-2]
                 + 2*cache[i-3]
                 - cache[i-4]
                 - cache[i-6];
    }
    max_computed = n;

    return cache[n];
}
