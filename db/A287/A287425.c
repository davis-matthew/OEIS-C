/*
 * A287425  Number of maximal matchings in the n-gear graph.
 * https://oeis.org/A287425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 12*a(n-3) - 11*a(n-4) + 7*a(n-5) -
 *            3*a(n-6) + a(n-7) for n ≥ 8 with initial values a(1)=3, a(2)=6,
 *            a(3)=11, a(4)=22, a(5)=47, a(6)=98, a(7)=198
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 34, u64 n <= 72, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

static seq_t *cache = NULL;      /* cache of computed terms */
static idx_t cached = 0;         /* number of terms currently cached (0 if none) */

/* Base values for n = 1 … 7 */
static const seq_t base[7] = {3, 6, 11, 22, 47, 98, 198};

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥ 1 per OEIS offset */
    if (n <= 7)
        return base[n - 1];

    /* Allocate or enlarge cache if necessary */
    if (cache == NULL) {
        /* allocate space for n terms */
        cache = (seq_t *)malloc(n * sizeof(seq_t));
        if (cache == NULL)
            abort();  /* allocation failure – terminate */
        /* store the initial seven values */
        for (idx_t i = 0; i < 7; ++i)
            cache[i] = base[i];
        cached = 7;
    } else if (n > cached) {
        /* resize to hold up to n terms */
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (tmp == NULL)
            abort();      /* allocation failure – terminate */
        cache = tmp;
    }

    /* Compute missing terms up to n (if any) */
    for (idx_t i = cached + 1; i <= n; ++i) {
        seq_t val = 5 * cache[i - 2]
                  - 10 * cache[i - 3]
                  + 12 * cache[i - 4]
                  - 11 * cache[i - 5]
                  + 7  * cache[i - 6]
                  - 3  * cache[i - 7]
                  + cache[i - 8];
        cache[i - 1] = val;
    }
    cached = n;
    return cache[n - 1];
}
