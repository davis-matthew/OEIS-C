/*
 * A332778  a(1) = 1; a(n) = Sum_{d|n, d < n} phi(n/d) * a(d)^2.
 * https://oeis.org/A332778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d < n} phi(n/d) * a(d)^2. Recurrence: a(1)=1;
 *            for n>1 a(n)=Sum_{k=1..n-1} a(gcd(n,k))^2. Implementation uses
 *            memoised dynamic arrays
 * Range:     u8 n <= 17, u16 n <= 35, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

/*--------------------------------------------------------------------
  Memoised computation of the sequence.
  Dynamically grow two tables:
    * cache[ ] – stores a(d) for all d up to current n
    * phi[ ]   – Euler's φ for all numbers up to current n
  The sieve recompute phi each time a larger n is requested; for the
  expected limits (n ≤ 10000) this is fast enough.
--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* static (per-process) memoisation state */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;   /* current allocated size of cache */
    static idx_t  last    = 0;   /* highest index already computed */

    static seq_t *phi     = NULL;
    static size_t phi_cap = 0;   /* length of phi array */

    /*--- grow cache if necessary --------------------------------*/
    if (cap < n + 1) {
        size_t new_cap = (cap == 0) ? 1 : cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /*--- grow phi array and recompute it --------------------------------*/
    if (phi_cap < n + 1) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(phi, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        phi = tmp;
        phi_cap = new_cap;

        /* initialise phi[i] = i */
        for (idx_t i = 0; i < phi_cap; ++i) {
            phi[i] = (seq_t)i;
        }

        /* Euler sieve for φ */
        for (idx_t p = 2; p < phi_cap; ++p) {
            if (phi[p] == (seq_t)p) {            /* p is prime   */
                for (idx_t j = p; j < phi_cap; j += p) {
                    phi[j] -= phi[j] / (seq_t)p;
                }
            }
        }
    }

    /* base case */
    if (last < 1) {
        cache[1] = (seq_t)1;
        last = 1;
    }

    /* compute new values up to n */
    for (idx_t m = last + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d) continue;

            idx_t e = m / d;                /* complementary divisor */

            /* divisor d */
            if (d < m) {
                seq_t val   = cache[d];
                seq_t term  = phi[e] * val * val;
                sum += term;
            }

            /* complementary divisor e, if distinct */
            if (e != d && e < m) {
                seq_t val   = cache[e];
                seq_t term  = phi[d] * val * val;
                sum += term;
            }
        }
        cache[m] = sum;
    }

    last = n;
    return cache[n];
}
