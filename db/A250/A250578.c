/*
 * A250578  Number of (n+1) X (3+1) 0..1 arrays with nondecreasing
 *          max(x(i,j),x(i,j-1)) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 22, u64 n <= 48, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A250578
 *  Number of (n+1)×(3+1) 0..1 arrays with nondecreasing max(x(i,j),x(i,j-1))
 *  in the i direction and nondecreasing min(x(i,j),x(i-1,j)) in the j direction.
 *
 *  Recurrence (valid for n ≥ 9):
 *      a(n) = 5*a(n-1) - 6*a(n-2) - 7*a(n-3) + 21*a(n-4)
 *             - 14*a(n-5) - a(n-6) + 4*a(n-7) - a(n-8)
 *  Initial values (n = 1 … 8):
 *      a(1)=55, a(2)=142, a(3)=336, a(4)=778, a(5)=1779,
 *      a(6)=4077, a(7)=9346, a(8)=21510
 *
 *  This implementation uses the recurrence with a simple cache to allow
 *  arbitrary order of calls.  All arithmetic is performed using only
 *  the +, -, * operators on seq_t, so it works for both fixed‑width
 *  unsigned types and arbitrary‑precision classes.
 *
 *  No external utilities are used.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Sanity: the sequence is undefined for n < 1; return 0 in that case. */
    if (n == 0) return 0;

    /* Initial terms for n = 1 … 8 */
    static const seq_t init[8] = {
        55, 142, 336, 778, 1779, 4077, 9346, 21510
    };

    /* Cache of previously computed values.
     * cache[0] holds a(1), cache[1] holds a(2), etc.            */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of elements in cache */

    /* Ensure the cache is allocated and contains the first 8 terms. */
    if (cache == NULL)
    {
        cache_size = 8;
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        if (cache == NULL) abort();     /* memory exhaustion */
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init[i];
    }

    /* If n is already in the cache, just return it. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Grow the cache to size n (allocate one more slot for each new term). */
    seq_t *new_cache = (seq_t *)realloc(cache,
                      (size_t)n * sizeof(seq_t));
    if (new_cache == NULL) abort();       /* memory exhaustion */
    cache = new_cache;

    /* Compute terms from cache_size+1 up to n inclusive.  The
     * recurrence needs the previous eight values.                 */
    for (idx_t i = cache_size + 1; i <= n; ++i)
    {
        /* Compute the positive and negative parts separately to avoid
         * underflow in unsigned arithmetic.                         */
        seq_t pos = cache[i - 2];       /* 5 * a(i-1) */
        pos *= 5;
        seq_t t = cache[i - 5];         /* 21 * a(i-4) */
        t *= 21;
        pos += t;
        t = cache[i - 8];               /* 4  * a(i-7) */
        t *= 4;
        pos += t;

        seq_t neg = cache[i - 3];       /* 6 * a(i-2) */
        neg *= 6;
        t = cache[i - 4];               /* 7 * a(i-3) */
        t *= 7;
        neg += t;
        t = cache[i - 6];               /* 14* a(i-5) */
        t *= 14;
        neg += t;
        neg += cache[i - 7];            /*  a(i-6)  */
        neg += cache[i - 9];            /*  a(i-8)  */

        cache[i - 1] = pos - neg;       /* a(i) = pos - neg   */
    }

    cache_size = n;
    return cache[n - 1];
}
