/*
 * A351049  G.f. A(x) satisfies: A(x) = 1 + x + x^2 * A(x/(1 - 3*x)) / (1 -
 *          3*x).
 * https://oeis.org/A351049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A351049
   a(0) = a(1) = 1;
   a(n) = Σ_{k=0}^{n-2} binomial(n-2,k) * 3^k * a(n-k-2)   for n ≥ 2   */
#include <stdlib.h>
#include <stddef.h>

/* Computes the n-th term of A351049.  Values are stored in a
   static cache, so the function may be called in any order.      */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;   /* array holding computed terms   */
    static size_t cache_cap  = 0;       /* allocated array capacity in terms */
    static idx_t  max_computed = 0;     /* largest index for which a() is known */

    /* Ensure cache is large enough for term n.  Allocate one more element
       to allow writing up to a[n] (n is inclusive).                     */
    if (cache_cap < (size_t)(n + 1)) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp)               /* allocation failure – return 0 (undefined) */
            return (seq_t)0;
        cache     = tmp;
        cache_cap = n + 1;
    }

    /* Initialise the first two terms if they are not already present.
       For the special case n == 0 we must not touch cache[1] because
       it does not exist yet.                                               */
    if (max_computed == 0) {
        cache[0] = (seq_t)1;
        if (n >= 1) {
            cache[1] = (seq_t)1;
            max_computed = 1;
        }
    }

    /* For every m that is still unknown, compute a(m) using the
       recurrence.  The loop runs from max_computed+1 up to n.           */
    for (idx_t m = max_computed + 1; m <= n; ++m) {
        /* m ≥ 2 here – otherwise the loop would have stopped.       */
        seq_t sum  = (seq_t)0;   /* running total for a(m)                 */
        seq_t C    = (seq_t)1;   /* binomial(m-2,0)                        */
        seq_t pow3 = (seq_t)1;   /* 3^0                                    */

        /* Sum over k = 0 .. m-2                                    */
        for (idx_t k = 0; k <= m - 2; ++k) {
            /* term = C * pow3 * a(m-k-2)                            */
            seq_t term = C;
            term = term * pow3;
            term = term * cache[(m - k - 2)];

            sum += term;

            /* Prepare C and pow3 for the next k.                     */
            if (k < m - 2) {
                seq_t num = (seq_t)(m - 2 - k);
                seq_t den = (seq_t)(k + 1);
                C = (C * num) / den;   /* exact division            */
            }
            pow3 *= (seq_t)3;
        }

        cache[m] = sum;
        max_computed = m;
    }

    return cache[n];
}
