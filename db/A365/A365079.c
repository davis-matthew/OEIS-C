/*
 * A365079  G.f. satisfies A(x) = 1 + x*A(x)*(1 + x^4*A(x)^3).
 * https://oeis.org/A365079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/5⌋} binomial(n-4*k,k) *
 *            binomial(n-k+1,n-4*k) / (n-k+1)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 5;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t b1 = binomial(n - 4 * k, k);
        seq_t b2 = binomial(n - k + 1, n - 4 * k);
        idx_t denom = n - k + 1;
        unsigned __int128 prod = (unsigned __int128)b1 * (unsigned __int128)b2;
        seq_t term = (seq_t)(prod / denom);
        sum += term;
    }
    return sum;
}
