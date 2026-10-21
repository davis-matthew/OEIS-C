/*
 * A245369  Number of compositions of n into parts 3, 5 and 8.
 * https://oeis.org/A245369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + a(n-5) + a(n-8) Initial values: a(0)=1, a(1)=0,
 *            a(2)=0, a(3)=1, a(4)=0, a(5)=1, a(6)=1,
 * Range:     u8 n <= 31, u16 n <= 55, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached sequence values */
    static idx_t size  = 0;       /* number of cached entries (size==0 ⇒ no cache yet) */

    /* grow cache to include value at index n */
    if (n >= size) {
        /* Allocate / reallocate memory for indices 0 … n */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return 0;     /* allocation failed – return 0 as a safe fallback */
        cache = tmp;

        /* compute new terms */
        for (idx_t i = size; i <= n; ++i) {
            seq_t val = 0;
            if (i == 0) {
                val = 1;         /* a(0) = 1 */
            } else {
                if (i >= 3) val += cache[i - 3];
                if (i >= 5) val += cache[i - 5];
                if (i >= 8) val += cache[i - 8];
            }
            cache[i] = val;
        }
        size = n + 1;
    }

    return cache[n];
}
