/*
 * A263073  Expansion of phi(-x^5) / (chi(-x) * chi(-x^15)) in powers of x
 *          where phi(), chi() are Ramanujan theta functions.
 * https://oeis.org/A263073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Euler transform of the period‑30 sequence
 *            [1,0,1,0,-1,0,1,0,1,-1,1,0,1,0,0,0,1,0,1,-1,1,0,1,0,-1,0,1,0,1,-1].
 *            The generating function is A(x) = ∏_{k≥1} (1 - x^k)^{-b_k} where
 *            b_k = period[(k-1) mod 30]
 * Range:     u8 n <= 52, u16 n <= 147, u32 n <= 465, u64 n <= 1598,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the nth coefficient of this product. */
#include <stdlib.h>

static const int period[30] = {
    1, 0, 1, 0, -1, 0, 1, 0, 1, -1,
    1, 0, 1, 0, 0, 0, 1, 0, 1, -1,
    1, 0, 1, 0, -1, 0, 1, 0, 1, -1
};

seq_t a(idx_t n)
{
    size_t len = (size_t)n + 1;
    seq_t *coef = (seq_t *)malloc(len * sizeof(seq_t));
    if (!coef) return (seq_t)0;          /* memory allocation failure */

    /* initialise with the constant term 1 */
    coef[0] = (seq_t)1;
    for (size_t i = 1; i < len; ++i)
        coef[i] = (seq_t)0;

    /* apply the infinite product factors up to x^n */
    for (idx_t k = 1; k <= n; ++k) {
        int b = period[(k - 1) % 30];
        if (b == 1) {
            for (size_t i = k; i < len; ++i)
                coef[i] += coef[i - k];
        } else if (b == -1) {
            /* multiply by (1 - x^k) */
            for (size_t i = len - 1; i + 1 > (size_t)k; --i)
                coef[i] -= coef[i - k];
        }
    }

    seq_t result = coef[(size_t)n];
    free(coef);
    return result;
}
