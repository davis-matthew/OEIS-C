/*
 * A276482  a(n) = 5^n*Gamma(n+1/5)*Gamma(n+1)/Gamma(1/5).
 * https://oeis.org/A276482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..n} k*(5*k - 4), a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t factor = (seq_t)k * (((seq_t)5 * k) - (seq_t)4);
        result *= factor;
    }
    return result;
}
