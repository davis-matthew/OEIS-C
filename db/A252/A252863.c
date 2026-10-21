/*
 * A252863  Number of Eulerian paths in a lattice graph bounded by the four
 *          equations x+y=1, x+y=2n, x-y=2, and x-y=-2.
 * https://oeis.org/A252863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20*a(n-1) - 16*a(n-2) with offset 1: a(1)=1, a(2)=16
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;
static idx_t computed = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */
    if (n == 1) return 1;
    if (n == 2) return 16;

    /* Ensure storage for all needed terms */
    if (!cache || computed < n)
    {
        seq_t *tmp = (seq_t*)realloc(cache, n * sizeof(seq_t));
        if (!tmp) return 0;        /* allocation failure */
        cache = tmp;
    }

    /* Initialize base cases if not yet done */
    if (computed == 0)
    {
        cache[0] = 1;
        cache[1] = 16;
        computed = 2;
    }

    /* Compute missing terms */
    for (idx_t i = computed; i < n; ++i)
    {
        cache[i] = 20 * cache[i - 1] - 16 * cache[i - 2];
    }
    computed = n;

    return cache[n - 1];
}
