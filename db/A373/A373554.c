/*
 * A373554  Number of ternary length-2 squarefree words of length n not
 *          containing the subwords 021 or 10.
 * https://oeis.org/A373554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) + a(n-3) - a(n-4) (Recurrence valid for n ≥ 5,
 *            with initial terms a(0)=1, a(1)=3, a(2)=5, a(3)=7, a(4)=11)
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 53, u64 n <= 109,
 *            u128 n <= 220, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed values */
    static seq_t *cache = NULL;     /* allocated array */
    static size_t cap   = 0;        /* allocated capacity   */
    static size_t sz    = 0;        /* number of valid entries */

    /* Base values for indices 0 … 4 */
    static const seq_t base[5] = { (seq_t)1, (seq_t)3,
                                   (seq_t)5, (seq_t)7,
                                   (seq_t)11 };

    /* Return an already computed value */
    if (n < sz)
        return cache[(size_t)n];

    /* Ensure the allocation is large enough */
    if (cap == 0)
        cap = 5;
    while (cap <= (size_t)n)
        cap <<= 1;                     /* double capacity */

    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));

    /* First time: initialise the first five entries */
    if (sz == 0) {
        for (size_t i = 0; i < 5; ++i)
            cache[i] = base[i];
        sz = 5;                        /* we now have entries 0 … 4 */
    }

    /* Compute missing terms using the recurrence */
    if (n >= sz) {
        for (size_t i = sz; i <= (size_t)n; ++i)
            cache[i] = 2 * cache[i - 2] + cache[i - 3] - cache[i - 4];
        sz = (size_t)n + 1;            /* valid indices up to n */
    }

    return cache[(size_t)n];
}
