/*
 * A336243  a(0) = 1; a(n) = Sum_{k=0..n-1} binomial(n,k)^2 * (n-k-1)! * a(k).
 * https://oeis.org/A336243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = Σ_{k=0}^{n-1} binomial(n,k)^2 * (n-k-1)! * a(k)
 *            (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static caches for previously computed values and factorials          */
    static seq_t *a_cache      = NULL; /* a(0..size-1)                        */
    static idx_t  a_cache_size = 0;    /* current cache length (entries)      */
    static seq_t *fact_cache   = NULL; /* factorials 0! … (size-1)!           */
    static idx_t  fact_cache_size = 0;/* current factorial cache length       */

    /* --- Ensure the factorial cache reaches index n-1 ------------------- */
    if (n > 0) {
        idx_t needed_fact = n;       /* need factorials up to (n-1)! */
        if (needed_fact > fact_cache_size) {
            /* grow factorial cache */
            idx_t new_size = needed_fact;
            seq_t *tmp = (seq_t *)realloc(fact_cache, new_size * sizeof(seq_t));
            if (!tmp) abort();            /* allocation failure             */
            fact_cache = tmp;

            /* initialise new entries */
            if (fact_cache_size == 0) {
                fact_cache[0] = (seq_t)1; /* 0! = 1                         */
                fact_cache_size = 1;
            }
            for (idx_t i = fact_cache_size; i < new_size; ++i) {
                fact_cache[i] = fact_cache[i-1] * (seq_t)i; /* i!  */
            }
            fact_cache_size = new_size;
        }
    }

    /* --- Ensure the a(n) cache reaches index n -------------------------- */
    if (n >= a_cache_size) {
        idx_t new_size = n + 1;          /* include a(n) itself             */
        seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure              */
        a_cache = tmp;

        /* initialise new entries (if any) */
        if (a_cache_size == 0) {
            a_cache[0] = (seq_t)1;       /* a(0) = 1                        */
            a_cache_size = 1;
        }

        /* compute missing terms from a_cache_size up to n                */
        for (idx_t m = a_cache_size; m <= n; ++m) {
            if (m == 0) {
                a_cache[0] = (seq_t)1; /* already set above                */
                continue;
            }

            seq_t sum   = (seq_t)0;
            seq_t binom = (seq_t)1; /* C(m,0) */

            for (idx_t k = 0; k < m; ++k) {
                /* term = binom^2 * (m-k-1)! * a(k) */
                seq_t term = binom * binom;
                term *= fact_cache[m - k - 1];
                term *= a_cache[k];
                sum += term;

                /* update binom for next k (skip after last iteration) */
                if (k + 1 < m) {
                    seq_t num = (seq_t)(m - k);
                    seq_t den = (seq_t)(k + 1);
                    binom = (binom * num) / den;
                }
            }
            a_cache[m] = sum;
        }
        a_cache_size = new_size;
    }

    return a_cache[n];
}
