/*
 * A304161  a(n) = 2*n^3 - 4*n^2 + 10*n - 2 (n>=1).
 * https://oeis.org/A304161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^3 - 4*n^2 + 10*n - 2
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; behaviour below is irrelevant */
    seq_t sn = (seq_t)n;           /* cast index to seq_t before arithmetic */
    seq_t n2 = sn * sn;            /* n^2 */
    seq_t n3 = n2 * sn;            /* n^3 */
    seq_t res = (2 * n3) + (10 * sn) - (4 * n2); /* 2n^3 + 10n - 4n^2 */
    res = res - 2;                 /* subtract the final 2 */
    return res;
}
