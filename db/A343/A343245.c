/*
 * A343245  Hyperplane-counting upper bound on the number of sorted orders of
 *          X+Y for two lists X and Y of length n.
 * https://oeis.org/A343245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..2*n} binomial(2*binomial(n,2)^2 +
 *            2*binomial(n,2), i)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* binomial(n,2) = n*(n-1)/2 computed in seq_t */
    seq_t bin_n_2 = ((seq_t)n) * (n - 1) / 2;

    /* m = 2*bin_n_2^2 + 2*bin_n_2 */
    seq_t m = 2 * bin_n_2 * bin_n_2 + 2 * bin_n_2;

    seq_t sum = 0;          /* accumulator for the partial sum */
    seq_t cur = 1;          /* C(m,0) */

    idx_t limit = 2 * n;    /* upper bound for i in the sum */

    for (idx_t i = 0; i <= limit; ++i)
    {
        sum += cur;        /* add C(m,i) to the sum */

        if (i == limit)   /* no next binomial needed after the last term */
            break;

        /* compute C(m,i+1) from C(m,i) */
        seq_t num   = cur * (m - (seq_t)i);    /* numerator: cur * (m - i) */
        seq_t denom = (seq_t)(i + 1);          /* denominator: i + 1 */
        cur = num / denom;                    /* exact division */
    }

    return sum;
}
