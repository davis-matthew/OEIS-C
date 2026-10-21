/*
 * A277668  Number of n-length words over a 5-ary alphabet {a_1,a_2,...,a_5}
 *          avoiding consecutive letters a_i, a_{i+1}.
 * https://oeis.org/A277668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 4*a(n-2) + 3*a(n-3) - 2*a(n-4) + a(n-5) for n
 *            >= 5 Initial values: a(0)=1, a(1)=5, a(2)=21, a(3)=88, a(4)=369
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External linkage function required by the build system */
seq_t a(idx_t n)
{
    /* Small table of the first five terms */
    static const seq_t base[5] = { (seq_t)1, (seq_t)5, (seq_t)21,
                                   (seq_t)88, (seq_t)369 };

    if (n < 5)
        return base[n];

    /* Memoisation buffer for previously computed terms */
    static seq_t *cache = NULL;  /* dynamically allocated array */
    static idx_t last = 0;       /* highest index currently stored */
    static size_t cap  = 0;      /* allocated capacity */

    /* Initialise the cache on first use */
    if (cache == NULL)
    {
        /* start with capacity 8 or N+1 if a larger n is requested */
        size_t init_cap = 8;
        if ((size_t)n + 1 > init_cap)
            init_cap = (size_t)n + 1;
        cap = init_cap;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)
            abort();              /* allocation failure */
        /* fill the first five values */
        for (idx_t i = 0; i < 5; ++i)
            cache[i] = base[i];
        last = 4;
    }

    /* If the requested term is already cached, return it */
    if (n <= last)
        return cache[n];

    /* Expand the cache if necessary */
    if ((size_t)n >= cap)
    {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;   /* double until it fits */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            abort();         /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute terms from last+1 up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        cache[i] = (seq_t)5 * a1
                 - (seq_t)4 * a2
                 + (seq_t)3 * a3
                 - (seq_t)2 * a4
                 + (seq_t)1 * a5;
    }

    last = n;
    return cache[n];
}
