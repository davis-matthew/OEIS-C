/*
 * A292970  Expansion of e.g.f. exp(x^5 * exp(-x)).
 * https://oeis.org/A292970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/5⌋} (-k)^(n-5k)/(k! * (n-5k)!)
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 12, i64 n <= 21, i128 n <= 34,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the offset 0 explicitly. */
    if (n == 0)
        return 1;

    idx_t maxk = n / 5;
    seq_t sum = 0;

    /* C(n, 0) = 1 for the first binomial term. */
    seq_t bin = 1;
    /* product_{i=k+1}^{5k} i for k=0 is 1. */
    seq_t ratio = 1;

    for (idx_t k = 1; k <= maxk; ++k)
    {
        /* Update the binomial coefficient C(n, 5k) from C(n, 5(k-1)). */
        seq_t num = 1;
        for (int i = 0; i < 5; ++i)
            num *= (seq_t)(n - 5 * (k - 1) - i);

        seq_t den = 1;
        for (int i = 1; i <= 5; ++i)
            den *= (seq_t)(5 * (k - 1) + i);

        bin = bin * num / den;

        /* Update ratio = product_{i=k+1}^{5k} i using the recurrence. */
        seq_t num2 = 1;
        for (int i = 0; i < 5; ++i)
            num2 *= (seq_t)(5 * k - 4 + i);  /* 5k-4, 5k-3, ... 5k */
        ratio = ratio * num2 / (seq_t)k;

        /* Compute the power (-k)^(n-5k). */
        idx_t m = n - 5 * k;
        seq_t power = big_pow((seq_t)(- (seq_t)k), (seq_t)m);

        /* Add the current term to the sum. */
        seq_t term = bin * ratio * power;
        sum += term;
    }

    return sum;
}
