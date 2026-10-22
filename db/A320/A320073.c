/*
 * A320073  Number of length n primitive (=aperiodic or period n) 8-ary words
 *          which are earlier in lexicographic order than any other word
 *          derived by cyclic shifts of the alphabet.
 * https://oeis.org/A320073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 8^(d-1) * mu(n/d)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   mu_of, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"
#include "../utils/mu_of.h"

seq_t a(idx_t n)
{
    /* Variables for summation by sign to avoid underflow. */
    seq_t pos = 0, neg = 0;

    /* Enumerate all divisors of n. */
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            /* First divisor d = i. */
            int mu_val = mu_of(n / i);   /* mu(n/d) */
            if (mu_val != 0)
            {
                seq_t val = pow_seq((seq_t)8, (int)(i - 1));
                if (mu_val > 0)
                    pos += val;
                else
                    neg += val;
            }

            /* Second divisor d = n / i, if distinct. */
            idx_t d2 = n / i;
            if (d2 != i)
            {
                int mu_val2 = mu_of(i);  /* mu(n/d2) = mu(i) */
                if (mu_val2 != 0)
                {
                    seq_t val = pow_seq((seq_t)8, (int)(d2 - 1));
                    if (mu_val2 > 0)
                        pos += val;
                    else
                        neg += val;
                }
            }
        }
    }

    /* pos >= neg for every n; the subtraction is safe in unsigned arithmetic. */
    return pos - neg;
}
