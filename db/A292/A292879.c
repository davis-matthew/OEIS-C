/*
 * A292879  Triangle read by rows: T(n,k) (n>=1, 4<=k<=n+3) is the number of
 *          k-sequences of balls colored with at most n colors such that
 *          exactly three balls are of a color seen previously in the
 *          sequence.
 * https://oeis.org/A292879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the triangle read by rows, where *
 *            T(n,k) = (binom(k,4)+10*binom(k,5)+15*binom(k,6)) * n!/(n+3-k)!.
 *            * Here a(n) = T(m,k) with m = row number and k = column number
 *            such that * n falls into the m‑th row (the row has length m)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 43, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binomial, mul_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_mul.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Find the row m that contains the n-th element. */
    idx_t m = 1;
    idx_t cum = 1;                /* cumulative number of terms up to row m */
    while (cum < n) {
        ++m;
        cum += m;
    }

    /* Compute the start index of this row and the offset within the row. */
    idx_t start  = cum - m + 1;            /* first index of row m */
    idx_t offset = n - start;              /* 0‑based offset inside the row */

    /* Column number k in the triangle (k ranges from 4 to m+3). */
    idx_t k = 4 + offset;

    /* Compute the coefficient  C(k,4)+10*C(k,5)+15*C(k,6). */
    seq_t coeff = binomial(k, 4);
    coeff += mul_small(binomial(k, 5), 10);
    coeff += mul_small(binomial(k, 6), 15);

    /* Compute the factorial ratio n!/(n+3-k)! as a product
     * of the consecutive integers from m-k+4 up to m. */
    seq_t ratio = 1;
    idx_t start_fact = m - k + 4;          /* first factor of the product */
    for (idx_t i = start_fact; i <= m; ++i) {
        ratio = seq_mul(ratio, (seq_t)i);
    }

    /* Result is coeff * ratio. */
    return seq_mul(coeff, ratio);
}
