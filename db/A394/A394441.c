/*
 * A394441  a(n) = Sum_{k=0..n} Sum_{j=0..k} (-1)^(k - j)*binomial(k, j)*j^(n
 *          - k)*2^j. Row sums of A394444.
 * https://oeis.org/A394441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Sum_{j=0..k} (-1)^(k - j)*binomial(k,
 *            j)*j^(n - k)*2^j
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 26, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* helper: binary exponentiation for non‑negative exponent */
static seq_t seq_pow_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* allocate arrays; cast void* from calloc for C++ */
    seq_t *binom = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!binom) return 0;
    seq_t *pow2 = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!pow2) { free(binom); return 0; }

    binom[0] = 1;
    pow2[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        pow2[i] = pow2[i - 1] * 2;

    seq_t result = 0;

    for (idx_t k = 0; k <= n; ++k) {
        idx_t exp = n - k;              /* exponent for j^(n-k) */

        for (idx_t j = 0; j <= k; ++j) {
            if (j == 0 && exp != 0) continue;   /* 0^(positive) = 0 */

            seq_t term = binom[j];

            if (j > 0)
                term = term * seq_pow_uint((seq_t)j, exp); /* j^(n-k) */

            term = term * pow2[j];                    /* * 2^j   */

            if ((k - j) & 1)
                result -= term;                       /* sign = -1 */
            else
                result += term;                       /* sign = +1 */
        }

        /* compute next row of Pascal's triangle */
        if (k < n) {
            for (idx_t j = k + 1; j > 0; --j)
                binom[j] = binom[j] + binom[j - 1];
        }
    }

    free(binom);
    free(pow2);
    return result;
}
