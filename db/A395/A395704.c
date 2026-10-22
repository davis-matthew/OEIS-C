/*
 * A395704  Expansion of g^4*(2*g-1)/(3-2*g)^4, where g = 1+x*g^3 is the g.f.
 *          of A001764.
 * https://oeis.org/A395704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) * Sum_{k=0..n} 3^(n-k) * binomial(2*n + k + 1, k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t pow = pow3(n);          /* 3^n */
    seq_t bin = 1;                /* binomial(2*n + 0 + 1, 0) = 1 */

    for (idx_t k = 0; k <= n; ++k) {
        sum += bin * pow;

        if (k < n) {
            /* C_{k+1} = C_k * (2*n + k + 2) / (k + 1) */
            bin = bin * (seq_t)(2*n + k + 2) / (k + 1);
        }

        pow /= 3;                 /* exact integer division for powers of 3 */
    }

    return sum * ((seq_t)n + 1);
}
