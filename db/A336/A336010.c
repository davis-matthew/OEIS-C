/*
 * A336010  a(0) = ... = a(4) = 1; a(n) = Sum_{k=0..n-5} binomial(n-5,k) *
 *          a(k) * a(n-k-5).
 * https://oeis.org/A336010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(0) = a(1) = a(2) = a(3) = a(4) = 1; a(n) =
 *            Σ_{k=0}^{n‑5} C(n‑5,k) * a(k) * a(n‑k‑5) (for n ≥ 5)
 * Range:     u8 n <= 12, u16 n <= 17, u32 n <= 25, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;
    /* remember the old capacity so we can zero new slots */
    idx_t old_cap = cap;

    /* grow the cache if required */
    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        /* realloc must be cast to seq_t* for C++ */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) {            /* allocation failed – spoil gracefully */
            return 0;
        }
        /* initialise the newly allocated part to zero */
        for (idx_t i = old_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cap   = new_cap;
    }

    /* compute terms that have not been cached yet */
    for (idx_t i = old_cap; i <= n; ++i) {
        if (i < 5) {                  /* base values */
            cache[i] = 1;
            continue;
        }

        idx_t m   = i - 5;
        seq_t sum = 0;
        seq_t bin = 1;                 /* C(m,0) */

        for (idx_t k = 0; k <= m; ++k) {
            idx_t j = i - k - 5;        /* j < i -> already computed */
            seq_t term = cache[k];
            term *= cache[j];
            term *= bin;
            sum += term;

            if (k < m) {               /* update bin for next k */
                seq_t mul = (seq_t)(m - k);
                seq_t den = (seq_t)(k + 1);
                bin = bin * mul;
                bin = bin / den;       /* exact division */
            }
        }
        cache[i] = sum;
    }

    return cache[n];
}
