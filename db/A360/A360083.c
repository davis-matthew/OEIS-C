/*
 * A360083  a(n) = Sum_{k=0..n} binomial(5*k,n-k) * Catalan(k).
 * https://oeis.org/A360083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(5*k,n-k)*Catalan(k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t bin = binomial(5 * k, n - k);
        if (bin == 0) continue;
        sum += bin * catalan(k);
    }
    return sum;
}
