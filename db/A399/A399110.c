/*
 * A399110  Number of endofunctions on [2n] with exactly n isolated fixed
 *          points.
 * https://oeis.org/A399110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} (-1)^{n-j} * j^j * binomial(2n, n) *
 *            binomial(n, j)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to a(n) = binomial(2n, n) * Sum_{k=0..n} (-1)^{n-k} binomial(n, k) * k^k */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* binomial(2*n, n) term */
    seq_t bin2n = binomial(2 * n, n);
    seq_t result = 0;

    for (idx_t j = 0; j <= n; ++j)
    {
        seq_t bin_n_j = binomial(n, j);

        /* j^j, treating 0^0 as 1 */
        seq_t pow_j;
        if (j == 0)
            pow_j = (seq_t)1;
        else
            pow_j = big_pow((seq_t)j, (seq_t)j);

        /* term = bin2n * bin_n_j * j^j */
        seq_t term = bin2n;
        term *= bin_n_j;
        term *= pow_j;

        /* sign */
        if (((n - j) & 1) == 0) /* even number of negative multiplications */
            result += term;
        else
            result -= term;
    }

    return result;
}
