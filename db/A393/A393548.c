/*
 * A393548  Number of total dominating sets in K_{2n,2n,2n} minus a perfect
 *          matching.
 * https://oeis.org/A393548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^n-1)^3 + 3*(4^n-1-n)^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow4n = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow4n *= (seq_t)4;          /* compute 4^n */

    seq_t term1 = pow4n - (seq_t)1;  /* 4^n - 1 */

    seq_t part1 = term1 * term1 * term1;                /* (4^n-1)^3 */

    seq_t term2 = term1 - (seq_t)n;                     /* 4^n-1-n */
    seq_t part2 = term2 * term2;                       /* (4^n-1-n)^2 */

    return part1 + (seq_t)3 * part2;                   /* sum of terms */
}
