/*
 * A368881  a(n) = binomial(n+3, 4) + binomial(n+1, 3) + 1.
 * https://oeis.org/A368881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+3, 4) + binomial(n+1, 3) + 1
 * Range:     u8 n <= 6, u16 n <= 33, u32 n <= 564, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* binomial(n+3, 4) = (n+3)*(n+2)*(n+1)*n / 24 */
    seq_t binom4 = ((seq_t)(n + 3) * (n + 2) * (n + 1) * n) / 24;

    /* binomial(n+1, 3) = (n+1)*n*(n-1) / 6  for n >= 2, 0 otherwise */
    seq_t binom3 = 0;
    if (n >= 2) {
        binom3 = ((seq_t)(n + 1) * n * (n - 1)) / 6;
    }

    return binom4 + binom3 + 1;
}
