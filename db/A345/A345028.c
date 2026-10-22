/*
 * A345028  a(n) = Sum_{k=1..n} 2^(floor(n/k) - 1).
 * https://oeis.org/A345028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} 2^(floor(n/k)-1) (OEIS A345028)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for n < 1 – return 0 for safety */

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t f = n / k;          /* floor(n/k) */
        sum += pow2(f - 1);
    }
    return sum;
}
