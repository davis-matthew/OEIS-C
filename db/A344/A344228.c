/*
 * A344228  a(n) =
 *          binomial(2*n,n)*(2*n+1)/2+n*binomial(2*n-2,n)+(n-1)*binomial(2*n-2,n+1).
 * https://oeis.org/A344228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n,n)*(2*n+1)/2 + n*binomial(2*n-2,n) +
 *            (n-1)*binomial(2*n-2,n+1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n<1 */

    idx_t twon = 2 * n;
    seq_t term1 = binomial(twon, n);
    term1 = term1 * ((seq_t)(twon + 1));
    term1 = term1 / 2;

    idx_t twon_minus2 = twon - 2;
    seq_t term2 = binomial(twon_minus2, n);
    term2 = term2 * ((seq_t)n);

    seq_t term3 = binomial(twon_minus2, n + 1);
    term3 = term3 * ((seq_t)(n - 1));

    return term1 + term2 + term3;
}
