/*
 * A269774  Number of length-n 0..6 arrays with every repeated value unequal
 *          to the previous repeated value plus one mod 6+1.
 * https://oeis.org/A269774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=7, a(2)=49, a(3)=343, * a(n)=12*a(n-1)-30*a(n-2)-36*a(n-3)
 *            (n>=4)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* unused offset; safe guard */

    static seq_t *cache   = NULL;    /* array of computed values   */
    static size_t cache_sz = 0;      /* allocated size              */
    static size_t last    = 0;       /* largest index computed so far */

    /* ensure space for indices 1..3 at least */
    size_t need = (size_t)n + 1;
    if (need < 4) need = 4;

    if (cache == NULL || need > cache_sz) {
        cache   = (seq_t*)realloc(cache, need * sizeof(seq_t));
        cache_sz = need;
    }

    /* value already known? */
    if (last >= n) return cache[n];

    /* initialise the first three terms once */
    if (last < 3) {
        cache[1] = 7;
        cache[2] = 49;
        cache[3] = 343;
        last = 3;                     /* now the first three are set */
        if (n <= 3) return cache[n];  /* early return for small n   */
    }

    /* compute remaining terms up to n */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        cache[i] =
            cache[i-1] * (seq_t)12
          - cache[i-2] * (seq_t)30
          - cache[i-3] * (seq_t)36;
    }

    last = n;                          /* update highest computed index */
    return cache[n];
}
