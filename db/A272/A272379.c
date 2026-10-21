/*
 * A272379  a(n) = n*(24*n^3 - 60*n^2 + 54*n - 17).
 * https://oeis.org/A272379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(24*n^3 - 60*n^2 + 54*n - 17)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 116, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Convert index to seq_t first to avoid mixed type issues */
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;          /* n^2 */
    seq_t N3 = N2 * N;         /* n^3 */

    /* Compute (24*n^3 + 54*n) - (60*n^2 + 17) avoiding underflow */
    seq_t part1 = (seq_t)24 * N3;   /* 24*n^3 */
    part1 += (seq_t)54 * N;         /* + 54*n */
    seq_t part2 = (seq_t)60 * N2;   /* 60*n^2 */
    part2 += (seq_t)17;             /* + 17 */

    seq_t expr = part1 - part2;     /* 24*n^3 - 60*n^2 + 54*n - 17 */

    return N * expr;                /* n * (expr) */
}
