/*
 * A359812  a(n) = Sum_{d|n} (-1)^(d-1) * d^(n/d-1).
 * https://oeis.org/A359812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{d|n} (-1)^(d-1) * d^(n/d-1)
 * Range:     i8 n <= 15, i16 n <= 31, i32 n <= 62, i64 n <= 122,
 *            i128 n <= 245, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast exponentiation for seq_t (exact integer type). */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

/* Computes a(n) as defined above. */
seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            /* contribution for divisor d */
            idx_t exp_d = n / d - 1;
            seq_t power_d = pow_seq((seq_t)d, exp_d);
            /* sign = (-1)^(d-1): +1 if d odd, -1 if d even */
            seq_t sign_d = (d & 1) ? 1 : -1;
            total += sign_d * power_d;

            /* counterpart divisor q = n/d, skip if equal to d */
            idx_t q = n / d;
            if (q != d)
            {
                idx_t exp_q = n / q - 1;          /* equals d-1 */
                seq_t power_q = pow_seq((seq_t)q, exp_q);
                seq_t sign_q = (q & 1) ? 1 : -1;
                total += sign_q * power_q;
            }
        }
    }
    return total;
}
