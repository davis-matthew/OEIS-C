/*
 * A335338  P_5(2n+1), the Legendre polynomial of order 5 at 2n+1.
 * https://oeis.org/A335338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 252*n^5 + 630*n^4 + 560*n^3 + 210*n^2 + 30*n + 1
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;            /* avoids overflow for very small seq_t types */
    seq_t nn = (seq_t)n;
    seq_t res = 252;                  /* 252*n^5 + 630*n^4 + ...   */
    res = res * nn + 630;
    res = res * nn + 560;
    res = res * nn + 210;
    res = res * nn + 30;
    res = res * nn + 1;
    return res;
}
