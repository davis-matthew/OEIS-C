/*
 * A375691  G.f. A(x) satisfies A(x) = 1 + x^4*(1+x)*A(x)^3.
 * https://oeis.org/A375691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} binomial(k, n-4k) * binomial(3k,
 *            k)/(2k+1)
 * Range:     u8 n <= 15, u16 n <= 23, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 4; ++k)
    {
        idx_t rem = n - 4 * k;
        if (rem > k)          /* binomial(k, rem) would be zero otherwise */
            continue;

        seq_t bin1 = binomial(k, rem);
        if (bin1 == 0)
            continue;

        seq_t bin2 = binomial(3 * k, k);
        seq_t div  = 2 * k + 1;           /* division is exact by definition */

        sum += bin1 * (bin2 / div);
    }
    return sum;
}
