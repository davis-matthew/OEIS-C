/*
 * A375413  Expansion of e.g.f. exp(x^2 * (1 - x)^2) / (1 - x).
 * https://oeis.org/A375413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * n! * Σ_{k=0..⌊n/2⌋} binomial(2k-1, n-2k) / k!
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 22,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* n! needed for the formula */
    seq_t n_fact = factorial(n);

    /* cur will hold n! / k! during the loop */
    seq_t cur = n_fact;

    seq_t sum = 0;
    idx_t maxk = n / 2;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t bin;
        if (k == 0)
        {
            /* binomial(-1, n) = (-1)^n */
            bin = (n & 1) ? (seq_t)-1 : (seq_t)1;
        }
        else
        {
            idx_t top = 2 * k - 1;
            idx_t bottom = n - 2 * k;
            if (bottom > top)
                bin = (seq_t)0;
            else
                bin = binomial(top, bottom);
        }

        if (bin != (seq_t)0)
            sum += cur * bin;

        /* advance cur to n! / (k+1)! for next iteration */
        if (k < maxk)
            cur /= (seq_t)(k + 1);
    }

    /* apply the overall sign (-1)^n */
    if ((n & 1) != 0)
        return -sum;
    else
        return sum;
}
