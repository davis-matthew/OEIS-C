/*
 * A370246  Coefficient of x^n in the expansion of ( 1/(1-x) * (1+x^3) )^n.
 * https://oeis.org/A370246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n,k) * binomial(2*n-3*k-1,
 *            n-3*k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return (seq_t)1;

    seq_t total = (seq_t)0;
    idx_t max_k = n / 3;

    for (idx_t k = 0; k <= max_k; ++k) {
        seq_t c1 = binomial(n, k);

        idx_t a = (idx_t)(2 * n - 3 * k - 1);
        idx_t b = n - 3 * k;

        seq_t c2 = binomial(a, b);
        total += c1 * c2;
    }

    return total;
}
