/*
 * A255661  Number of length n+1 0..3 arrays with at most two downsteps in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A255661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in * x * (16 - 128 x + 543 x^2 - 1388
 *            x^3 + 2394 x^4 - 2964 x^5 * + 2683 x^6 - 1760 x^7 + 814 x^8 -
 *            252 x^9 + 47 x^10 - 4 x^11) * / (1 - x)^12 * * The sequence is
 *            given for offset 1 (a(1) = 16). For n < 1 the * result is
 *            undefined; the implementation returns 0 for n == 0...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 30, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   binom_11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_11.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; for n==0 just return 0. */
    if (n == 0U)
        return (seq_t)0;

    /* Coefficients of the numerator polynomial (degree 11). */
    static const int coeff[12] = {
        16,   -128,   543,   -1388,  2394,
        -2964, 2683,  -1760,  814,   -252,
        47,   -4
    };

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t j = 0; j < 12; ++j)
    {
        /* For n <= j the corresponding binomial term vanishes. */
        if (n <= j)
            continue;

        idx_t k = n - j + 10U;   /* top of the binomial C(k, 11) */

        if (k < 11U)
            continue;

        seq_t bin = binom_11(k);

        if (coeff[j] >= 0)
            sum_pos += bin * (seq_t)coeff[j];
        else
            sum_neg += bin * (seq_t)(-coeff[j]);
    }

    /* The sequence values are positive, so sum_pos >= sum_neg. */
    return sum_pos - sum_neg;
}
