/*
 * A375540  a(n) = 2^n * n! * [x^n] (1/2 - exp(-x))^n.
 * https://oeis.org/A375540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=0..n} (-1)^{n-j} · binomial(n,j) · j^n · 2^j
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"      /* provides binomial() but we compute it locally */
#include <stdbool.h>             /* for bool if needed */

/* Utility: binary exponentiation for seq_t */
static seq_t pow_ui(seq_t b, idx_t e)
{
    seq_t r = (seq_t)1;
    while (e != 0)
    {
        if (e & 1ULL)
            r = r * b;
        e >>= 1;
        if (e)
            b = b * b;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum_pos = (seq_t)0, sum_neg = (seq_t)0;

    seq_t binom = (seq_t)1;   /* binomial(n,0) */
    seq_t pow2 = (seq_t)1;    /* 2^0 */

    for (idx_t j = 0; j <= n; ++j)
    {
        seq_t powj = pow_ui((seq_t)j, n);   /* j^n */

        /* term = binomial(n,j) * j^n * 2^j */
        seq_t term = binom * powj * pow2;

        /* sign = (-1)^(n-j) */
        if (((n - j) & 1ULL) == 0)
            sum_pos += term;   /* even exponent -> positive sign */
        else
            sum_neg += term;   /* odd exponent  -> negative sign */

        if (j == n)
            break;                 /* avoid extra update after last iteration */

        /* Update binomial coefficient for next j: binom(n,j+1) */
        binom = binom * (seq_t)(n - j) / (seq_t)(j + 1);

        /* Update power of 2: 2^{j+1} */
        pow2 <<= 1;
    }

    return sum_pos - sum_neg;
}
