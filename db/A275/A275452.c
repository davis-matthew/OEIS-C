/*
 * A275452  G.f.: 3F2([1/9, 4/9, 7/9], [1/3, 1], 729 x).
 * https://oeis.org/A275452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A275452 – a(n) = (3*(9n-8)*(9n-5)*(9n-2) / (n^2*(3n-2))) * a(n-1)
 * with a(0) = 1.  The implementation normalises the recurrence by canceling
 * common factors between numerator, denominator and the running product to
 * avoid overflow on fixed‑size integer types.  It works for any integer
 * type that seq_t can represent. */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return 1;

    seq_t a_prev = 1;          /* a(i-1) */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* Convert index to the integer type used for calculations. */
        seq_t i_seq = (seq_t)i;

        /* Numerator factors: 3, 9i-8, 9i-5, 9i-2 */
        seq_t num_factors[4];
        num_factors[0] = 3;
        num_factors[1] = 9 * i_seq - 8;
        num_factors[2] = 9 * i_seq - 5;
        num_factors[3] = 9 * i_seq - 2;

        /* Denominator factors: i, i, 3i-2 */
        seq_t den_factors[3];
        den_factors[0] = i_seq;
        den_factors[1] = i_seq;
        den_factors[2] = 3 * i_seq - 2;

        /* Cancel common factors between numerator and denominator. */
        for (int nf = 0; nf < 4; ++nf)
        {
            for (int df = 0; df < 3; ++df)
            {
                seq_t g = gcd(num_factors[nf], den_factors[df]);
                if (g > 1)
                {
                    num_factors[nf] /= g;
                    den_factors[df] /= g;
                }
            }
        }

        /* Multiply and cancel with remaining denominator factors to keep
         * intermediate values within the bounds of seq_t. */
        seq_t a = a_prev;
        for (int nf = 0; nf < 4; ++nf)
        {
            /* Cancel denominator factors that divide 'a' before multiplying. */
            for (int df = 0; df < 3; ++df)
            {
                seq_t g = gcd(a, den_factors[df]);
                if (g > 1)
                {
                    a /= g;
                    den_factors[df] /= g;
                }
            }
            /* Multiply by the current numerator factor. */
            a *= num_factors[nf];
        }

        /* Any remaining denominator factors (should be 1) are divided at the end. */
        for (int df = 0; df < 3; ++df)
        {
            if (den_factors[df] > 1)
                a /= den_factors[df];
        }

        /* Store computed value for next iteration. */
        a_prev = a;
    }
    return a_prev;
}
