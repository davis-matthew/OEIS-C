/*
 * A342618  a(n) = Sum_{d|n} phi(d)^(n+d+1).
 * https://oeis.org/A342618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d)^(n+d+1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is not defined; return 0 for safety. */
    if (n == 0)
        return 0;

    seq_t sum = 0;

    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            /* divisor i */
            idx_t d1 = i;
            seq_t phi_d1 = phi(d1);
            seq_t exp1   = (seq_t)n + (seq_t)d1 + (seq_t)1;
            sum += big_pow(phi_d1, exp1);

            /* complementary divisor n/i, if distinct */
            idx_t d2 = n / i;
            if (d2 != d1)
            {
                seq_t phi_d2 = phi(d2);
                seq_t exp2   = (seq_t)n + (seq_t)d2 + (seq_t)1;
                sum += big_pow(phi_d2, exp2);
            }
        }
    }
    return sum;
}
