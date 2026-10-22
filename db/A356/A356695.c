/*
 * A356695  Expansion of x*(1+x-7*x^3-3*x^4+x^5)/(1-2*x^2-9*x^3+3*x^5).
 * https://oeis.org/A356695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) + 9*a(n-3) - 3*a(n-5)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=1, a(2)=1, a(3)=2, a(4)=4, a(5)=10, a(6)=24 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;  /* cache[0] unused; cache[1] = a(1) … */
static size_t cap   = 0;     /* current array capacity (elements)        */
static idx_t  upto  = 0;     /* highest computed index                    */

/* Ensure the cache has room for indices 0 … n */
static void ensure_cache(idx_t n)
{
    if (n < (idx_t)cap)
        return;                          /* already enough */

    size_t new_cap = cap ? cap * 2 : 8;
    if (new_cap < (size_t)(n + 1))
        new_cap = (size_t)(n + 1);

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                         /* out of memory */

    cache = tmp;
    cap   = new_cap;
}

/* Compute the n-th term of A356695 (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n<1; return 0 for safety */

    /* Base values for n = 1 … 6.  */
    static const seq_t base[7] = {0, 1, 1, 2, 4, 10, 24};
    if (n <= 6)
        return base[(size_t)n];

    /* Initialise cache on the first call. */
    if (cache == NULL) {
        /* Allocate at least space for indices 0 … 6 */
        cache = (seq_t *)malloc(7 * sizeof(seq_t));
        if (!cache)
            abort();
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 2;
        cache[4] = 4;
        cache[5] = 10;
        cache[6] = 24;
        cap   = 7;
        upto  = 6;
    }

    /* Extend cache and compute missing terms up to n. */
    if (n > upto) {
        ensure_cache(n);
        for (idx_t i = upto + 1; i <= n; ++i) {
            /* recurrence: a(i) = 2*a(i-2) + 9*a(i-3) - 3*a(i-5) */
            seq_t val = 2 * cache[(size_t)(i - 2)];
            val      += 9 * cache[(size_t)(i - 3)];
            val      -= 3 * cache[(size_t)(i - 5)];
            cache[(size_t)i] = val;
        }
        upto = n;
    }

    return cache[(size_t)n];
}
