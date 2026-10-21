/*
 * A290026  Number of 3-cycles in the n-halved cube graph.
 * https://oeis.org/A290026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1) * binomial(n,3) for n ≥ 1 (a(1)=0, a(2)=0)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    /* 2^(n-1) */
    seq_t pow = 1;
    for (idx_t i = 1; i < n; ++i)        /* multiply n-1 times */
        pow *= 2;

    /* binomial(n,3) = n*(n-1)*(n-2)/6 */
    seq_t nn   = (seq_t)n;
    seq_t bin  = nn * (nn - 1) * (nn - 2);
    bin /= 6;                           /* exact division */

    return pow * bin;
}
