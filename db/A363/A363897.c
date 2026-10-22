/*
 * A363897  Expansion of Sum_{k>0} k * x^k / (1 - x^(5*k)).
 * https://oeis.org/A363897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, n/d==1 mod 5} d
 * Range:     u8 n <= 215, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* the sequence is defined for n ≥ 1 */

    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0)
            continue;

        idx_t d1 = d;
        idx_t d2 = n / d;

        if ((n / d1) % 5 == 1)  /* condition for divisor d1 */
            sum += (seq_t)d1;

        if (d1 != d2 && (n / d2) % 5 == 1)  /* condition for divisor d2 */
            sum += (seq_t)d2;
    }
    return sum;
}
