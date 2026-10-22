/*
 * A344719  Minimum number of cells in an n X n grid that need to be painted
 *          so that each unpainted cell is orthogonally adjacent to exactly
 *          two painted cells.
 * https://oeis.org/A344719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more, changed
 * Formula:   a(n) = ceil((n+2)^2 / 3) – 6 for n ≥ 7; a(1)=1, a(2)=2, a(3)=5,
 *            a(4)=8, a(5)=11, a(6)=17
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small n by explicit values */
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 5;
    if (n == 4) return 8;
    if (n == 5) return 11;
    if (n == 6) return 17;

    /* For n ≥ 7 use the conjectured formula.
       ceil(x/3) = (x + 2) / 3  where x is integer. */
    seq_t nn = (seq_t)n;            /* cast to seq_t before arithmetic */
    seq_t tmp = (nn + 2) * (nn + 2); /* (n+2)^2 */
    seq_t ceil_div = (tmp + 2) / 3;   /* ceil division by 3 */
    return ceil_div - 6;
}
