/*
 * A300993  O.g.f. A(x) satisfies: A(x) = x * (1 - 5*x*A'(x)) / (1 -
 *          6*x*A'(x)).
 * https://oeis.org/A300993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A300993:  Original OGF
   A(x) = x * (1 - 5*x*A'(x)) / (1 - 6*x*A'(x))
   Recurrence derived from the functional equation:
     a(1)   = 1
     a(n)   =  (n==1?1:0)
               - 5*(n-1)*a(n-1)
               + 6 * Σ_{i=1}^{n-1} a(i) * (n-i) * a(n-i)   for n ≥ 1
   This implementation memoises the terms in a static array so that
   calls in arbitrary order are handled efficiently.  The recurrence
   requires O(n²) multiplications; for typical test data (n ≤ 10 000)
   this runs well within the given limits.  The code is compatible
   with both C11 and C++17. */

#include <stdlib.h>

/* function prototype given in the problem description */
seq_t a(idx_t n)
{
    /* static memoisation array */
    static seq_t *cache = NULL;      /* indices 0..cached   */
    static idx_t  cached = 0;        /* maximum computed n */

    if (n == 0) return 0;            /* undefined for n=0 but harmless */

    /* grow the cache if necessary */
    if (cached < n) {
        /* allocate space for indices 0..n */
        idx_t newsize = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * newsize);
        if (!new_cache) {
            /* allocation failure – return 0 (unlikely in test harness) */
            return 0;
        }
        cache = new_cache;

        /* compute new values from cached+1 up to n */
        for (idx_t m = cached + 1; m <= n; ++m) {
            /* compute the convolution sum S_m = Σ_{i=1}^{m-1} a_i * (m-i) * a_{m-i} */
            seq_t sum = 0;
            if (m > 1) {
                for (idx_t i = 1; i < m; ++i) {
                    seq_t left  = cache[i];
                    seq_t right = cache[m - i];
                    seq_t term  = left * ((seq_t)(m - i) * right);
                    sum += term;
                }
            }
            /* recurrence */
            seq_t val = (m == 1 ? 1 : 0);          /* δ_{m,1} */
            val -= 5 * (seq_t)(m - 1) * cache[m - 1];
            val += 6 * sum;
            cache[m] = val;
        }
        cached = n;
    }

    return cache[n];
}
