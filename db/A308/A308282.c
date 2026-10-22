/*
 * A308282  The fifth power of the unsigned Lah triangular matrix A105278.
 * https://oeis.org/A308282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 5^(m-k) * binomial(m-1, k-1) * (m! / k!) * where m =
 *            smallest integer such that m*(m+1)/2 ≥ n, * k = n - m*(m-1)/2 (k
 *            is the 1‑based column in row m)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 36, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of range – undefined in OEIS definition */

    /* Find row m */
    idx_t m = 0;
    while ((m * (m + 1)) / 2 < n)
        ++m;

    /* Column k in row m (1‑based) */
    idx_t prev_tri = m * (m - 1) / 2;
    idx_t k = n - prev_tri;

    /* Compute 5^(m-k) */
    seq_t pow5 = big_pow((seq_t)5, (seq_t)(m - k));

    /* Binomial coefficient */
    seq_t bin = binomial(m - 1, k - 1);

    /* Factorial ratio m! / k! = product_{i=k+1}^m i */
    seq_t fac = 1;
    for (idx_t i = k + 1; i <= m; ++i)
        fac *= (seq_t)i;

    return pow5 * bin * fac;
}
