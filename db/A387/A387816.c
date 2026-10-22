/*
 * A387816  a(n) = Sum_{k=0..n} binomial(7*n+1,7*k).
 * https://oeis.org/A387816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(7*n+1,7*k) Recurrence:
 *            a(n)=71*a(n-1)+7585*a(n-2)-36991*a(n-3)-128*a(n-4) Initial
 *            values: a(0)=1, a(1)=9, a(2)=6451, a(3)=490337
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to memoize computed terms. */
    static seq_t *cache = NULL;        /* pointer to cached values */
    static size_t  cap   = 0;          /* current capacity of cache   */
    static size_t  used  = 0;          /* number of initialized slots */

    /* If the cache is too small, grow it to at least n+1 elements. */
    if ((size_t)n + 1 > cap)
    {
        size_t new_cap = (size_t)n + 1;
        /* Reallocate (or allocate first time). */
        if (cache == NULL)
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        else
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute missing entries up to and including n. */
    for (idx_t i = used; i <= n; ++i)
    {
        if (i == 0)
            cache[0] = 1;
        else if (i == 1)
            cache[1] = 9;
        else if (i == 2)
            cache[2] = 6451;
        else if (i == 3)
            cache[3] = 490337;
        else
        {
            /* Compute a(i) using the recurrence.
               All operations are performed in seq_t, which is
               unsigned or an arbitrary‑precision class.  The
               sum to be subtracted is guaranteed to be smaller
               than the preceding positive sum, so unsigned
               subtraction yields the correct positive result. */
            seq_t term1 = cache[i-1] * (seq_t)71;
            seq_t term3 = cache[i-2] * (seq_t)7585;
            seq_t sum   = term1 + term3;
            seq_t term2 = cache[i-4] * (seq_t)128;
            seq_t term4 = cache[i-3] * (seq_t)36991;
            sum -= term2;
            sum -= term4;
            cache[i] = sum;
        }
    }
    used = (size_t)n + 1;

    return cache[n];
}
