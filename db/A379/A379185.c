/*
 * A379185  G.f. A(x) satisfies A(x) = 1/((1 - x*A(x)^2) * (1 - x*A(x))^2).
 * https://oeis.org/A379185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+2*k+1,k) *
 *            binomial(3*n+k+1,n-k)/(n+2*k+1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        /* compute the two binomial factors */
        seq_t b1 = binomial(n + 2 * k + 1, k);
        seq_t b2 = binomial(3 * n + k + 1, n - k);
        /* denom = n + 2*k + 1 */
        idx_t denom_idx = n + 2 * k + 1;
        /* product of the factors, using 128‑bit intermediate to avoid overflow */
        unsigned __int128 prod = (unsigned __int128)b1 *
                                 (unsigned __int128)b2;
        seq_t term = (seq_t)(prod / (unsigned __int128)denom_idx);
        sum += term;
    }
    return sum;
}
