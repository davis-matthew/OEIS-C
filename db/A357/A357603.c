/*
 * A357603  a(n) is the number of different pairs of shortest paths in an n X
 *          n lattice going between opposite corners in opposite directions
 *          and not meeting at their middle point.
 * https://oeis.org/A357603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2n,n)^2 - sum_{k=0}^{n} binomial(n,k)^4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute binomial(2n,n) */
    seq_t bin = 1;
    for (idx_t i = 1; i <= n; ++i) {
        bin = (bin * (seq_t)(n + i)) / (seq_t)i;
    }

    /* total number of pairs of shortest paths */
    seq_t total = bin * bin;

    /* subtract pairs that meet at the middle point:
       sum_{k=0}^{n} binomial(n,k)^4 */
    seq_t sum = 0;
    seq_t bin_nk = 1;               /* binomial(n,0) */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t sq     = bin_nk * bin_nk;       /* square */
        seq_t fourth = sq * sq;               /* fourth power */
        sum += fourth;
        if (k < n) {
            bin_nk = (bin_nk * (seq_t)(n - k)) / (seq_t)(k + 1);
        }
    }

    return total - sum;
}
