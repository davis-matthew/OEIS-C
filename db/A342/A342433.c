/*
 * A342433  a(n) = Sum_{k=1..n} gcd(k,n)^(n-1).
 * https://oeis.org/A342433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} gcd(k,n)^(n-1) = Sum_{d|n} phi(n/d) *
 *            d^(n-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 381 (2026-11-01)
 * Helpers:   ipow_idx, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n<1, return 0 */

    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            idx_t j = n / i;      /* complementary divisor */

            /* term for d = i */
            seq_t term1 = ipow_idx((seq_t)i, n - 1) * totient(j);
            sum += term1;

            /* if d = j is distinct, add its term */
            if (j != i)
            {
                seq_t term2 = ipow_idx((seq_t)j, n - 1) * totient(i);
                sum += term2;
            }
        }
    }
    return sum;
}
