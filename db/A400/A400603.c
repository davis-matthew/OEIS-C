/*
 * A400603  a(1) = 1; a(n+1) = Sum_{k=1..n} a(n/gcd(n,k)) * a(k/gcd(n,k)).
 * https://oeis.org/A400603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(1)=1; a(n+1)=∑_{k=1..n} a(n/gcd(n,k))*a(k/gcd(n,k))
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 9, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Euclidean GCD for idx_t (uint64_t) */
static inline idx_t _gcd_idx(idx_t a, idx_t b)
{
    while (b != (idx_t)0)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute the nth term of OEIS sequence A400603.
   Handles arbitrary‑precision seq_t as well as fixed‑width types.
   Works for calls in any order; memoises previously computed values.
*/
seq_t a(idx_t n)
{
    if (n < (idx_t)1)  /* safety – not used by the library */
        return (seq_t)0;

    /* Static cache for memoisation */
    static seq_t *cache  = NULL;
    static idx_t  cap    = 0;   /* allocated array length (indices 0..cap-1) */
    static idx_t  last   = 0;   /* highest index already computed */

    /* Initialise cache on first call */
    if (cache == NULL)
    {
        cap   = (idx_t)2;               /* indices 0 and 1 */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failure */
        cache[(idx_t)1] = (seq_t)1;     /* a(1) = 1 */
        last = (idx_t)1;
    }

    /* Grow cache if it cannot hold index n */
    if (n >= cap)
    {
        idx_t newcap = cap;
        while (newcap <= n) newcap <<= 1; /* double until large enough */
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failure */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute terms from last+1 up to n */
    for (idx_t m = last + (idx_t)1; m <= n; ++m)
    {
        idx_t prev = m - (idx_t)1;          /* n in the recurrence */
        seq_t sum  = (seq_t)0;

        for (idx_t k = (idx_t)1; k <= prev; ++k)
        {
            idx_t g  = _gcd_idx(prev, k);
            idx_t i1 = prev / g;
            idx_t i2 = k     / g;
            sum += cache[i1] * cache[i2];
        }

        cache[m] = sum;
    }

    last = n;
    return cache[n];
}
