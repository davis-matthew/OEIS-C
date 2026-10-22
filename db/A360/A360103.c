/*
 * A360103  a(n) = Sum_{k=0..n} binomial(n+4*k,n-k) * Catalan(k).
 * https://oeis.org/A360103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..n} binomial(n+4k, n-k) * Catalan(k) Recurrence:
 *            for n ≥ 6 (n+1)*a(n) + 2*(-5n+3)*a(n-1) +(19n-47)*a(n-2)
 *            +20*(-n+4)*a(n-3) +5*(3n-17)*a(n-4) +2*(-3n+22)*a(n-5)
 *            +(n-9)*a(n-6) = 0 Initial terms: a(0)=1, a(1)=2, a(2)=9,
 *            a(3)=49, a(4)=283, a(5)=1715
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[6] = {
        (seq_t)1,
        (seq_t)2,
        (seq_t)9,
        (seq_t)49,
        (seq_t)283,
        (seq_t)1715
    };

    if (n < 6)
        return init[n];

    /* cache previously computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* initialise the cache on first call */
    if (cache == NULL) {
        cache = (seq_t*)malloc(6 * sizeof(seq_t));
        for (size_t i = 0; i < 6; ++i)
            cache[i] = init[i];
        cap = 6;
    }

    /* extend the cache if necessary */
    if ((idx_t)cap <= n) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failed – recreate all entries */
            tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;   /* oops – give up */
            for (idx_t i = 0; i <= (idx_t)new_cap - 1; ++i) {
                if (i < 6) {
                    tmp[i] = init[i];
                } else {
                    seq_t pos = 0, neg = 0;
                    int coeff;

                    coeff = 10 * (int)i - 6;          /* 10n‑6  */
                    if (coeff >= 0) pos += tmp[i-1] * (seq_t)coeff;
                    else             neg += tmp[i-1] * (seq_t)(-coeff);

                    coeff = 47 - 19 * (int)i;          /* 47‑19n  */
                    if (coeff >= 0) pos += tmp[i-2] * (seq_t)coeff;
                    else             neg += tmp[i-2] * (seq_t)(-coeff);

                    coeff = 20 * (int)i - 80;          /* 20n‑80  */
                    if (coeff >= 0) pos += tmp[i-3] * (seq_t)coeff;
                    else             neg += tmp[i-3] * (seq_t)(-coeff);

                    coeff = 85 - 15 * (int)i;          /* 85‑15n  */
                    if (coeff >= 0) pos += tmp[i-4] * (seq_t)coeff;
                    else             neg += tmp[i-4] * (seq_t)(-coeff);

                    coeff = 6 * (int)i - 44;           /* 6n‑44   */
                    if (coeff >= 0) pos += tmp[i-5] * (seq_t)coeff;
                    else             neg += tmp[i-5] * (seq_t)(-coeff);

                    coeff = 9 - (int)i;                /* 9‑n     */
                    if (coeff >= 0) pos += tmp[i-6] * (seq_t)coeff;
                    else             neg += tmp[i-6] * (seq_t)(-coeff);

                    tmp[i] = (pos - neg) / ( (seq_t)(i + 1) );
                }
            }
            free(cache);
            cache = tmp;
            cap   = new_cap;
            return cache[n];
        }
        cache = tmp;

        /* compute newcomers */
        for (idx_t i = (idx_t)cap; i <= n; ++i) {
            seq_t pos = 0, neg = 0;
            int coeff;

            coeff = 10 * (int)i - 6;
            if (coeff >= 0) pos += cache[i-1] * (seq_t)coeff;
            else             neg += cache[i-1] * (seq_t)(-coeff);

            coeff = 47 - 19 * (int)i;
            if (coeff >= 0) pos += cache[i-2] * (seq_t)coeff;
            else             neg += cache[i-2] * (seq_t)(-coeff);

            coeff = 20 * (int)i - 80;
            if (coeff >= 0) pos += cache[i-3] * (seq_t)coeff;
            else             neg += cache[i-3] * (seq_t)(-coeff);

            coeff = 85 - 15 * (int)i;
            if (coeff >= 0) pos += cache[i-4] * (seq_t)coeff;
            else             neg += cache[i-4] * (seq_t)(-coeff);

            coeff = 6 * (int)i - 44;
            if (coeff >= 0) pos += cache[i-5] * (seq_t)coeff;
            else             neg += cache[i-5] * (seq_t)(-coeff);

            coeff = 9 - (int)i;
            if (coeff >= 0) pos += cache[i-6] * (seq_t)coeff;
            else             neg += cache[i-6] * (seq_t)(-coeff);

            cache[i] = (pos - neg) / ( (seq_t)(i + 1) );
        }
        cap = new_cap;
    }

    return cache[n];
}
