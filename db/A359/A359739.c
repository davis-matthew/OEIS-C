/*
 * A359739  a(n) = Sum_{j=0..n, j even} binomial(n, j) * oddfactorial(j/2) *
 *          n^j, where oddfactorial(n) = (2*n)! / (2^n*n!).
 * https://oeis.org/A359739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=0..n, j even} binomial(n,j) * oddfactorial(j/2) *
 *            n^j where oddfactorial(m) = Π_{k=1..m} (2k-1) (odd double
 *            factorial)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t binom = 1;            /* binomial(n,0) */
    seq_t oddfac = 1;           /* m = 0 : oddfactorial(0) = 1 */
    seq_t pow_n = 1;            /* n^0 */
    seq_t n_seq = (seq_t)n;
    seq_t n_sq = n_seq * n_seq; /* n^2 */

    for (idx_t j = 0; j <= n; j += 2) {
        /* add current term */
        sum += binom * oddfac * pow_n;

        /* stop if no further term */
        if (j + 2 > n) break;

        /* update oddfactorial for next m = j/2 + 1  (multiply by 2*m+1) */
        seq_t m = (seq_t)(j / 2);
        oddfac *= (seq_t)(2 * m + 1);

        /* update binomial coefficient (C(n, j+2) from C(n, j)) */
        seq_t numerator   = (seq_t)(n - (j + 1)) * (seq_t)(n - j);
        seq_t denominator = (seq_t)(j + 2) * (seq_t)(j + 1);
        binom = binom * numerator / denominator;

        /* update power n^j (multiply by n^2) */
        pow_n *= n_sq;
    }
    return sum;
}
