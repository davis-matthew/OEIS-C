/*
 * A269799  Number of vertices of the fractional perfect matching polytope for
 *          the complete graph on n vertices.
 * https://oeis.org/A269799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (n-1)·a(n-2) + Σ_{odd k≥3} C(n-1,k-1)·(k-1)!/2 · a(n-k)
 *            a(1) = 0, a(0) = 1 (a(0) is only for the recurrence base case)
 *            The summation is taken over odd integers k with 3≤k≤n
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  last  = 0;   /* largest n already computed */

    if (n == (idx_t)0) return (seq_t)1;   /* unused but needed for the recurrence */

    /* ensure the cache is large enough */
    if (n >= (idx_t)cap) {
        size_t new_cap = (size_t)n + 1;       /* store indices up to n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failure, return 0 */
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise base cases if necessary */
    if (last < 1) {
        cache[0] = (seq_t)1;                 /* a(0) = 1 (recurrence base) */
        if (cap > 1) cache[1] = (seq_t)0;    /* a(1) = 0 */
        last = 1;
    }

    /* compute missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val = (seq_t)0;

        /* contribution of a 2‑cycle (pair) containing vertex 1 */
        if (i >= 2)
            val += ((seq_t)(i - 1)) * cache[i - 2];

        /* contributions of odd cycles (length ≥3) */
        if (i >= 3) {
            /* m = i-k for the current odd cycle size k (starting with k=3) */
            idx_t m = i - 3;

            /* initial t = C(i-1,2) * (2)!/2 = C(i-1,2) * 1 = (i-1)(i-2)/2 */
            seq_t t = (((seq_t)(i - 1) * (i - 2)) / (seq_t)2);

            /* add term for k = 3 */
            val += t * cache[i - 3];

            /* iterate over larger odd k (k += 2) */
            while (m > 1) {
                /* t for next odd k is previous t times m*(m-1)   */
                t *= (seq_t)m;
                t *= (seq_t)(m - 1);
                m -= 2;                /* new m = i - (k+2) */
                val += t * cache[m];
            }
        }

        cache[i] = val;
    }

    last = n;
    return cache[n];
}
