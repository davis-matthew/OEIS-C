/*
 * A302650  Number of minimal total dominating sets in the n-barbell graph.
 * https://oeis.org/A302650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,2)^2 - (n-1)^2 + 1
 * Range:     u8 n <= 6, u16 n <= 23, u32 n <= 362, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined only for n ≥ 1, but guard against overflow */
    seq_t N = (seq_t)n;
    seq_t bin = N * (N - 1) / 2;    /* binomial(n,2) */
    seq_t val = bin * bin;          /* binomial(n,2)^2 */
    seq_t vm1 = (N - 1) * (N - 1);  /* (n-1)^2 */
    val = val - vm1 + 1;            /* combine to get a(n) */
    return val;
}
