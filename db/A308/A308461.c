/*
 * A308461  Expansion of e.g.f. exp(x + 2 * Sum_{k>=2} x^(2^k)/2^k).
 * https://oeis.org/A308461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>=1 a(n) = a(n-1) + 2 * Σ_{t ∈ {4,8,16,32,…}, t ≤
 *            n} a(n-t) * (n-1)!/(n-t)!
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static caches for the sequence and for factorials */
    static seq_t *a_cache = NULL;   /* a_cache[i] = a(i)       */
    static size_t a_cap  = 0;       /* capacity of a_cache     */
    static idx_t  computed = 0;     /* largest n already computed */

    static seq_t *fact_cache = NULL; /* fact_cache[i] = i!     */
    static size_t fact_cap = 0;      /* capacity of fact_cache */

    /* initialise static arrays on first call */
    if (a_cap == 0) {
        a_cache   = (seq_t*)realloc(a_cache, sizeof(seq_t));
        a_cap = 1;
        a_cache[0] = (seq_t)1;          /* a(0) = 1           */
        computed = 0;
    }

    if (fact_cap == 0) {
        fact_cache = (seq_t*)realloc(fact_cache, sizeof(seq_t));
        fact_cap = 1;
        fact_cache[0] = (seq_t)1;       /* 0! = 1 */
    }

    /* enlarge caches if necessary */
    if ((size_t)n + 1 > a_cap) {
        size_t new_cap = (size_t)n + 1;
        a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cap   = new_cap;
    }

    if ((size_t)n + 1 > fact_cap) {
        size_t new_cap = (size_t)n + 1;
        fact_cache = (seq_t*)realloc(fact_cache, new_cap * sizeof(seq_t));
        for (size_t i = fact_cap; i < new_cap; ++i) {
            if (i == 0) fact_cache[0] = (seq_t)1;
            else fact_cache[i] = fact_cache[i-1] * (seq_t)i;
        }
        fact_cap = new_cap;
    }

    /* if we have already computed up to n, just return it */
    if (n <= computed)
        return a_cache[(size_t)n];

    /* compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i)
    {
        /* start with a(i-1) (the k=1 contribution) */
        seq_t sum = a_cache[(size_t)(i - 1)];

        /* add contributions from powers of two ≥4   */
        for (idx_t k = 2; ; ++k) {              /* 2^k, k≥2 gives 4,8,16,… */
            idx_t p = ((idx_t)1) << k;
            if (p > i) break;

            /* prod = (i-1)! / (i-p)!  */
            seq_t prod = fact_cache[i-1] / fact_cache[i-p];

            /* term = 2 * a[i-p] * prod */
            seq_t term = a_cache[(size_t)(i - p)];
            term *= prod;
            term *= (seq_t)2;

            sum += term;
        }

        a_cache[(size_t)i] = sum;
    }

    computed = n;
    return a_cache[(size_t)n];
}
