/*
 * A391094  a(n) is the number of closed binary operations on a set of n
 *          labeled elements with empty center.
 * https://oeis.org/A391094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} (-1)^j * binomial(n,j) * * n^(j*(j+1)/2 +
 *            n*(n-j))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return (seq_t)1;

    seq_t nseq = (seq_t)n;      /* base for all powers */
    seq_t pos = (seq_t)0;       /* sum of positive terms   */
    seq_t neg = (seq_t)0;       /* sum of negative terms   */

    for (idx_t j = 0; j <= n; ++j)
    {
        seq_t bin = binomial(n, j);

        /* exponent = j*(j+1)/2 + n*(n-j) */
        seq_t exp = ((seq_t)j * (j + 1) / 2)
                  + (seq_t)n * ((seq_t)n - (seq_t)j);

        seq_t term = bin * big_pow(nseq, exp);

        if ((j & 1ULL) == 0ULL)   /* (-1)^j positive for even j */
            pos += term;
        else
            neg += term;
    }

    /* The result is guaranteed to be non‑negative */
    return pos - neg;
}
