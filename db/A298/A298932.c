/*
 * A298932  Expansion of f(-x^3)^3 * phi(-x^12) / (f(-x) * chi(-x^4)) in
 *          powers of x where phi(), chi(), f() are Ramanujan theta functions.
 * https://oeis.org/A298932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2500, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A298932 – Expansion of
 *
 *      G(q) = ∏_{m≥1} (1 – q^m)^{e(m)} ,
 *
 *  where
 *
 *      e(m) = 3*⌊[3|m]⌋ + ⌊[8|m]⌋ + 2*⌊[12|m]⌋ – 1 –
 *             ⌊[4|m]⌋ – ⌊[24|m]⌋ .
 *
 *  The function computes the coefficient of qⁿ in G(q).
 *
 *  The computation is performed with signed 64‑bit intermediates;
 *  the final result is cast to seq_t.
 *
 *  Complexity:  O(n log n) ; fast enough for n ≈ 10000. */

#include <stdlib.h>
#include <stdint.h>

/* binomial coefficients for k ≤ 3 – used only for e > 0 */
static const int C[4][4] = {
    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {1, 2, 1, 0},
    {1, 3, 3, 1}
};

seq_t a(idx_t n)
{
    /* allocate coefficient arrays */
    size_t len = (size_t)n + 1;
    int64_t *coeff    = (int64_t *)calloc(len, sizeof(int64_t));
    int64_t *newcoeff = (int64_t *)calloc(len, sizeof(int64_t));
    if (!coeff || !newcoeff) {
        free(coeff); free(newcoeff);
        return (seq_t)0;            /* fallback on allocation failure */
    }

    coeff[0] = 1;                     /* starting series is 1 */

    for (idx_t m = 1; m <= n; ++m) {
        /* exponent e(m) */
        int e = -1;
        if (m % 3 == 0)  e += 3;
        if (m % 8 == 0)  e += 1;
        if (m % 12 == 0) e += 2;
        if (m % 4 == 0)  e -= 1;
        if (m % 24 == 0) e -= 1;

        if (e == 0) continue;         /* factor 1 – nothing to do */

        /* initialise newcoeff to zero */
        for (size_t i = 0; i < len; ++i) newcoeff[i] = 0;

        if (e > 0) {                  /* finite polynomial (1 – q^m)^e */
            int k = e;                /* 1 ≤ k ≤ 3 */
            for (int j = 0; j <= k; ++j) {
                idx_t shift = (idx_t)j * m;
                if (shift > n) break;

                int64_t coef = C[k][j];
                if (j & 1) coef = -coef;   /* (-1)^j * binomial */

                for (idx_t i = shift; i <= n; ++i) {
                    newcoeff[i] += coeff[i - shift] * coef;
                }
            }
        } else {                       /* infinite series (1 – q^m)^(-k) */
            int kneg = -e;              /* kneg = 1 or 2 */
            if (kneg == 1) {            /* weight 1 for every term */
                for (idx_t t = 0, shift = 0; shift <= n; ++t, shift = t * m) {
                    for (idx_t i = shift; i <= n; ++i) {
                        newcoeff[i] += coeff[i - shift];
                    }
                }
            } else {                   /* kneg == 2 – weight t+1 */
                for (idx_t t = 0, shift = 0; shift <= n; ++t, shift = t * m) {
                    int64_t weight = (int64_t)(t + 1);
                    for (idx_t i = shift; i <= n; ++i) {
                        newcoeff[i] += coeff[i - shift] * weight;
                    }
                }
            }
        }

        /* swap coeff and newcoeff for next iteration */
        int64_t *tmp = coeff;
        coeff = newcoeff;
        newcoeff = tmp;
    }

    seq_t result = (seq_t)coeff[n];

    free(coeff);
    free(newcoeff);
    return result;
}
