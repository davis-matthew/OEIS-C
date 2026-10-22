/*
 * A322753  Expansion of x*(1 + 2*x - 3*x^2 + 4*x^3) / (1 - x - x^2 + x^3 -
 *          x^4).
 * https://oeis.org/A322753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-4) for n>4 initial values:
 *            a(0)=0, a(1)=1, a(2)=3, a(3)=1, a(4)=7
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 53, u64 n <= 107,
 *            u128 n <= 214, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function relies solely on the user-provided types seq_t
   (the value type) and idx_t (the index type) and the standard
   library <stdlib.h>. No other headers or external symbols are needed. */

#include <stdlib.h>

/* external types supplied by the build; do NOT redefine them */
/* typedef seq_t;   --- already defined elsewhere */
/* typedef idx_t;   --- already defined elsewhere */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* dynamic array holding computed terms   */
    static size_t cap    = 0;         /* allocated size of cache (in elements) */
    static idx_t  max_cached = (idx_t)-1;  /* highest n for which cache is filled */

    /* --- initialise cache with the first five terms --- */
    if (!cache)
    {
        cap = 5;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;         /* out of memory → return 0 as safe fallback */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)7;
        max_cached = 4;
    }

    /* --- grow cache if necessary --- */
    if (n > max_cached)
    {
        /* determine new capacity: double until it exceeds n */
        size_t new_cap = cap;
        if (new_cap == 0)
            new_cap = 1;
        while (new_cap <= (size_t)n)
            new_cap = new_cap * 2 + 1;  /* +1 avoids infinite loop for cap==0 */

        /* reallocate memory */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;          /* realloc failed → return 0 */

        cache = tmp;
        cap   = new_cap;

        /* compute missing terms */
        for (idx_t i = max_cached + 1; i <= n; ++i)
        {
            /* cast indices to size_t for array access */
            size_t ui = (size_t)i;
            cache[ui] = cache[ui - 1] + cache[ui - 2]
                                - cache[ui - 3] + cache[ui - 4];
        }
        max_cached = n;
    }

    return cache[(size_t)n];
}
