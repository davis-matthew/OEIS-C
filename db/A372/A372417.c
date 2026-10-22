/*
 * A372417  Coefficient of x^n in the expansion of ( (1-x+x^3)^2 / (1-x)^3
 *          )^n.
 * https://oeis.org/A372417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(2n,k) * binomial(2n-2k-1,n-3k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle the offset case a(0) explicitly */
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t result = 0;
    idx_t max_k = n / 3;

    for (idx_t k = 0; k <= max_k; ++k) {
        /* C(2n,k)  */
        seq_t c1 = binomial(2 * n, k);

        /* C(2n-2k-1, n-3k)  */
        seq_t c2 = binomial(2 * n - 2 * k - (idx_t)1,
                            n - 3 * k);

        result += c1 * c2;
    }

    return result;
}
