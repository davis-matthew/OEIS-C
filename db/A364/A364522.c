/*
 * A364522  G.f. satisfies A(x) = 1 + x*A(x) + x^5*A(x)^5.
 * https://oeis.org/A364522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/5)} binomial(n,5*k) * binomial(5*k,k) /
 *            (4*k+1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 28, u64 n <= 52, u128 n <= 98,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    idx_t maxk = n / 5;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t term = binomial(n, 5 * k);
        seq_t t = binomial(5 * k, k);
        t /= (seq_t)(4 * k + 1);
        term *= t;
        sum += term;
    }
    return sum;
}
