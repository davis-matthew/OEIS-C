/*
 * A323664  Expansion of e.g.f. exp(exp(x)*BesselI(0,2*x) - 1).
 * https://oeis.org/A323664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=1..n} A002426(k) * C(n-1, k-1) * a(n-k),
 *            where A002426(k) = Σ_{p=0..⌊k/2⌋} C(k, 2p) * C(2p, p)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static memoisation tables */
    static seq_t *a_cache   = NULL;
    static seq_t *f_cache   = NULL;
    static size_t cache_cap = 0;

    /* Ensure capacity for indices up to n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        /* Reallocate a_cache */
        seq_t *tmp_a = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp_a) abort();
        a_cache = tmp_a;

        /* Reallocate f_cache */
        seq_t *tmp_f = (seq_t *)realloc(f_cache, new_cap * sizeof(seq_t));
        if (!tmp_f) abort();
        f_cache = tmp_f;

        /* Initialise new entries */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            if (i == 0) {          /* base case */
                a_cache[0] = 1;
                f_cache[0] = 1;    /* A002426(0) = 1 */
                continue;
            }

            /* compute A002426(i) and store in f_cache[i] */
            seq_t sum_f = 0;
            for (idx_t p = 0; p <= (idx_t)(i / 2); ++p) {
                seq_t t = binomial((idx_t)i, (idx_t)(2 * p));
                t *= binomial((idx_t)(2 * p), (idx_t)p);
                sum_f += t;
            }
            f_cache[i] = sum_f;

            /* compute a_cache[i] with the recurrence */
            seq_t sum = 0;
            for (idx_t k = 1; k <= (idx_t)i; ++k) {
                seq_t term = f_cache[(size_t)k];
                term *= binomial((idx_t)(i - 1), k - 1);
                term *= a_cache[(size_t)(i - k)];
                sum += term;
            }
            a_cache[i] = sum;
        }
        cache_cap = new_cap;
    }

    return a_cache[(size_t)n];
}
