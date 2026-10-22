/*
 * A395801  a(n) = Sum_{k=0..n} binomial(n+2*k,3*k) * (binomial(n+3*k,k) -
 *          binomial(n+3*k,k-1)).
 * https://oeis.org/A395801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+2*k, 3*k) * (binomial(n+3*k, k) -
 *            binomial(n+3*k, k-1))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A395801. */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t c1 = binomial(n + 2 * k, 3 * k);
        seq_t c2 = binomial(n + 3 * k, k);
        seq_t c3 = 0;
        if (k > 0)
            c3 = binomial(n + 3 * k, k - 1);
        seq_t term = c1 * (c2 - c3);
        sum += term;
    }
    return sum;
}
