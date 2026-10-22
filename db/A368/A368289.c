/*
 * A368289  Expansion of e.g.f. exp(-2*x) / (1 - log(1 + x)).
 * https://oeis.org/A368289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-2)^n + Sum_{k=1..n} (-1)^(k-1) * (k-1)! * binomial(n,k)
 *            * a(n-k)
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 22,
 *            big any n
 * Helpers:   grow_seq, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/seq_mul.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* static caches */
    static seq_t *a_cache = NULL;   /* cache for a(n) */
    static size_t a_cap   = 0;       /* allocated capacity of a_cache */
    static size_t a_size  = 0;       /* number of terms already computed */

    static seq_t *fact_cache = NULL; /* cache for factorials */
    static size_t fact_cap   = 0;     /* capacity of fact_cache */
    static size_t fact_size  = 0;     /* highest factorial computed + 1 */

    /* Ensure that the array can hold index n */
    if (n >= a_cap)
        grow_seq(&a_cache, &a_cap, n + 1);

    /* Return already computed value */
    if (n < a_size)
        return a_cache[n];

    /* Compute factorials up to n-1 if needed */
    idx_t need_fact = (n == 0) ? 0 : n - 1;
    if (need_fact + 1 > fact_size) {
        if (fact_cache == NULL) {
            grow_seq(&fact_cache, &fact_cap, need_fact + 1);
            fact_cache[0] = 1;
            fact_size = 1;
        }
        for (idx_t i = fact_size; i <= need_fact; ++i) {
            if (i == 0)
                fact_cache[0] = 1;
            else
                fact_cache[i] = seq_mul(fact_cache[i - 1], (seq_t)i);
        }
        fact_size = need_fact + 1;
    }

    /* Compute all terms up to n */
    for (idx_t k = a_size; k <= n; ++k) {
        if (k == 0) {
            a_cache[0] = 1;
            continue;
        }

        /* (-2)^k */
        seq_t val = pow2(k);
        if (k & 1ULL)
            val = -val;

        /* Sum_{m=0..k-1} ... */
        seq_t sum = 0;
        idx_t nm1 = k - 1;
        seq_t binom = 1;                 /* binomial(k,0) */
        seq_t f = fact_cache[nm1];       /* (k-1)! */

        for (idx_t m = 0; m < k; ++m) {
            /* term = a(m) * f * binom * (-1)^{k-m-1} */
            seq_t term = a_cache[m];
            term = seq_mul(term, f);
            term = seq_mul(term, binom);
            if ((k - m - 1ULL) & 1ULL)
                term = -term;

            sum += term;

            /* prepare for next m */
            if (m + 1 < k) {
                binom = seq_mul(binom, (seq_t)(k - m));
                binom = binom / (m + 1ULL);
                f = f / (k - m - 1ULL);
            }
        }

        val += sum;
        a_cache[k] = val;
    }

    a_size = n + 1;
    return a_cache[n];
}
