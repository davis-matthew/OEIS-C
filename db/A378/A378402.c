/*
 * A378402  a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+3*k,n).
 * https://oeis.org/A378402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+3*k,n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t t1 = binomial(n, k);
        seq_t t2 = binomial(n + 3 * k, n);
        total += t1 * t2;
    }
    return total;
}
