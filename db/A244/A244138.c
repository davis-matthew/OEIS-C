/*
 * A244138  Triangle read by rows: coefficients T(n,k) of a binomial
 *          decomposition of n*(n-1) as Sum(k=0..n)T(n,k)*binomial(n,k).
 * https://oeis.org/A244138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(i,j) where (i,j) is the row and column of the row‑major
 *            flattening of the OEIS triangle. For the triangle, T(n,k) = k *
 *            (1-k)^(k-2) * k^(n-k) for k > 1, and T(n,0)=T(n,1)=0
 * Range:     i8 n <= 18, i16 n <= 34, i32 n <= 75, i64 n <= 169,
 *            i128 n <= 432, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* seq_t and idx_t are supplied by the build configuration.  */
seq_t a(idx_t n)
{
    /* Find the row i (0‑based) and column j such that the index n
       lies in row i and the j‑th element of that row.   */
    idx_t i = 0;          /* current row */
    idx_t cum = 0;        /* number of elements before row i */

    /* The row lengths are 1, 2, 3, …  */
    while (n >= cum + i + 1u) {
        cum += i + 1u;
        i++;
    }
    idx_t j = n - cum;    /* column within the row */

    /* If k <= 1, the coefficient is defined to be 0. */
    if (j <= 1u) {
        return 0;
    }

    /* Convert indices to seq_t for arithmetic. */
    seq_t k_ts = (seq_t)j;
    seq_t i_ts = (seq_t)i;

    /* Compute the two power terms. */
    seq_t part1 = big_pow((seq_t)1 - k_ts, (seq_t)(j - 2u));
    seq_t part2 = big_pow(k_ts, (seq_t)(i - j));

    /* Multiply everything together. */
    return k_ts * part1 * part2;
}
