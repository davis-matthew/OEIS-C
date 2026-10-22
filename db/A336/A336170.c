/*
 * A336170  a(n) = Sum_{k=0..n} (-1)^(n-k) * (n+3*k)!/((n-k)! * k!^4).
 * https://oeis.org/A336170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * (n+3*k)! / ((n-k)! * k!^4)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* pos_sum accumulates terms with (+1) coefficient, neg_sum with (-1) */
    seq_t pos_sum = 0;
    seq_t neg_sum = 0;

    /* first term k = 0 is always 1 */
    seq_t term = 1;

    /* coefficient sign for k = 0 : (-1)^n */
    bool sign_positive = ((n & 1) == 0);   /* true if n is even */

    /* Add term at k = 0 */
    if (sign_positive)
        pos_sum += term;
    else
        neg_sum += term;

    /* Iterate for k = 0 .. n-1 to produce terms k+1 */
    for (idx_t k = 0; k < n; ++k)
    {
        /* Compute the next term using the recurrence
           t_{k+1} = t_k * (n+3k+3)(n+3k+2)(n+3k+1)(n-k) / (k+1)^4
           All arithmetic is performed on seq_t and exact divisions are
           guaranteed.  */
        seq_t next_term = term;

        /* Denominator factors (k+1)^4 */
        seq_t den[4];
        seq_t k1 = (seq_t)(k + 1);
        for (int j = 0; j < 4; ++j)
            den[j] = k1;

        /* Cancel the current term with denominator factors where possible */
        for (int j = 0; j < 4; ++j)
        {
            seq_t g = gcd(next_term, den[j]);
            if (g > (seq_t)1)
            {
                next_term /= g;
                den[j] /= g;
            }
        }

        /* Numerator factors */
        seq_t num[4];
        num[0] = (seq_t)(n + 3 * k + 3);
        num[1] = (seq_t)(n + 3 * k + 2);
        num[2] = (seq_t)(n + 3 * k + 1);
        num[3] = (seq_t)(n - k);

        /* Multiply by each numerator factor, cancelling with denoms as we go */
        for (int m = 0; m < 4; ++m)
        {
            next_term *= num[m];

            for (int j = 0; j < 4; ++j)
            {
                seq_t g = gcd(next_term, den[j]);
                if (g > (seq_t)1)
                {
                    next_term /= g;
                    den[j] /= g;
                }
            }
        }

        /* Any remaining denominator factors must divide the result exactly */
        for (int j = 0; j < 4; ++j)
        {
            if (den[j] > (seq_t)1)
                next_term /= den[j];
        }

        term = next_term;

        /* Update sign for next k+1 (flips each step) */
        sign_positive = !sign_positive;

        /* Add the new term to the appropriate sum */
        if (sign_positive)
            pos_sum += term;
        else
            neg_sum += term;
    }

    /* Final result: pos_sum - neg_sum  (pos_sum >= neg_sum by construction) */
    return pos_sum - neg_sum;
}
