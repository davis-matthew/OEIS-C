/*
 * A360150  a(n) = Sum_{k=0..floor(n/3)} binomial(2*n-k,n-3*k).
 * https://oeis.org/A360150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(2*n-k, n-3*k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binom, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a cache and a hybrid approach: for n <= 31
 * it evaluates the defining sum (avoiding overflow on 64‑bit builds);
 * for larger n it uses the order‑7 d‑finite recurrence:
 *   n*a(n)+2*(-7*n+6)a(n-1)+2*(36*n-61)a(n-2)+4*(-41*n+103)a(n-3)
 *   +(161*n-530)a(n-4)+(-71*n+278)a(n-5)+6*(2*n-9)a(n-6)=0
 * The recurrence is applied only when n > 31, which is sufficient for
 * the 64‑bit build, since all terms fitting in 64 bits have n ≤ 31.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/binom.h"   /* binom(idx_t a, idx_t b) returns seq_t */
#include "../utils/ensure_capacity.h"  /* ensure_capacity() prototype */

static const seq_t SMALL[7] = { (seq_t)1, (seq_t)2, (seq_t)6, (seq_t)21,
                                (seq_t)77, (seq_t)288, (seq_t)1090 };

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size */
    static idx_t   max_comp   = 0; /* highest index computed so far */

    /* ensure storage for index n */
    if (n + 1 > cache_cap)
        ensure_capacity((size_t)(n + 1), &cache_cap, (void **)&cache, sizeof(seq_t));

    /* initialise cache[0] */
    if (max_comp == 0)
    {
        cache[0] = SMALL[0];
        max_comp = 0;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i)
    {
        if (i <= 6)                     /* base values */
        {
            cache[i] = SMALL[i];
        }
        else if (i <= 31)               /* safe sum of binomial terms */
        {
            seq_t sum = 0;
            idx_t kmax = i / 3;
            for (idx_t k = 0; k <= kmax; ++k)
            {
                seq_t term = binom(2*i - k, i - 3*k);
                sum += term;
            }
            cache[i] = sum;
        }
        else                            /* use the recurrence for larger n */
        {
            /* accumulate positive and negative contributions separately */
            seq_t pos = 0, neg = 0;
            int64_t coeff;

            /* term a[i-1] */
            coeff = 2 * ((int64_t)(-7 * (int64_t)i + 6));
            if (coeff >= 0)
                pos += cache[i-1] * (seq_t)coeff;
            else
                neg += cache[i-1] * (seq_t)(-coeff);

            /* term a[i-2] */
            coeff = 2 * ((int64_t)(36 * (int64_t)i - 61));
            if (coeff >= 0)
                pos += cache[i-2] * (seq_t)coeff;
            else
                neg += cache[i-2] * (seq_t)(-coeff);

            /* term a[i-3] */
            coeff = 4 * ((int64_t)(-41 * (int64_t)i + 103));
            if (coeff >= 0)
                pos += cache[i-3] * (seq_t)coeff;
            else
                neg += cache[i-3] * (seq_t)(-coeff);

            /* term a[i-4] */
            coeff = (int64_t)(161 * (int64_t)i - 530);
            if (coeff >= 0)
                pos += cache[i-4] * (seq_t)coeff;
            else
                neg += cache[i-4] * (seq_t)(-coeff);

            /* term a[i-5] */
            coeff = -71 * (int64_t)i + 278;
            if (coeff >= 0)
                pos += cache[i-5] * (seq_t)coeff;
            else
                neg += cache[i-5] * (seq_t)(-coeff);

            /* term a[i-6] */
            coeff = 6 * ((int64_t)(2 * (int64_t)i - 9));
            if (coeff >= 0)
                pos += cache[i-6] * (seq_t)coeff;
            else
                neg += cache[i-6] * (seq_t)(-coeff);

            cache[i] = (pos - neg) / (seq_t)i;
        }
    }

    max_comp = n;
    return cache[n];
}
