/*
 * A364094  Sum of divisors of 5*n-3 of form 5*k+1.
 * https://oeis.org/A364094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all divisors d of (5*n-3) such that d mod 5 == 1.
 *            That is: a(n) = Σ_{d | (5*n-3), d≡1 (mod 5)} d
 * Range:     u8 n <= 52, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the target number N = 5*n - 3 */
    seq_t N = (seq_t)5 * n - 3;
    seq_t sum = 0;

    /* Enumerate all divisors up to sqrt(N) */
    for (idx_t d = 1; (seq_t)d * (seq_t)d <= N; ++d)
    {
        if (N % (seq_t)d == 0)
        {
            /* First divisor d */
            if (d % 5 == 1)
                sum += d;

            /* Corresponding divisor N/d, avoid double‑counting the square root */
            idx_t d2 = (idx_t)(N / (seq_t)d);
            if (d2 != d && d2 % 5 == 1)
                sum += d2;
        }
    }

    return sum;
}
