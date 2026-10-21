/*
 * A275051  Expansion of 3F2([1/9, 4/9, 5/9], [1/3,1], 729*x).
 * https://oeis.org/A275051
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

/* Recurrence: a(0)=1;
   for n≥1: a(n)=a(n-1)*3*(9*n-8)*(9*n-5)*(9*n-4)/(n^2*(3*n-2))
   The sequence is the coefficients of 3F2([1/9,4/9,5/9],[1/3,1],729*x). */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* Build numerator and denominator of the factor */
        seq_t num = (seq_t)3 *
                    (((seq_t)9 * i) - (seq_t)8) *
                    (((seq_t)9 * i) - (seq_t)5) *
                    (((seq_t)9 * i) - (seq_t)4);

        seq_t den = ((seq_t)i * i) *
                    (((seq_t)3 * i) - (seq_t)2);

        /* Cancel common factors with the current result */
        seq_t g = gcd(result, den);
        if (g != (seq_t)1)
        {
            result /= g;
            den /= g;
        }

        /* Cancel common factors with the numerator */
        g = gcd(num, den);
        if (g != (seq_t)1)
        {
            num /= g;
            den /= g;
        }

        /* In rare cases more cancellation might be possible */
        if (den != (seq_t)1)
        {
            g = gcd(result, den);
            if (g != (seq_t)1)
            {
                result /= g;
                den /= g;
            }
            g = gcd(num, den);
            if (g != (seq_t)1)
            {
                num /= g;
                den /= g;
            }
        }

        /* Multiply and divide */
        result *= num;
        if (den != (seq_t)1)
            result /= den;
    }

    return result;
}
