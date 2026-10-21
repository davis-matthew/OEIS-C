/*
 * A260090  Maximum number of kings on an n X n chessboard such that no king
 *          attacks more than one other king.
 * https://oeis.org/A260090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n*(n+2) + [n mod 3 = 2]) / 3) - [n mod 6 = 2], for n !=
 *            3 Special case: a(3) = 4 where [P] is 1 if P is true, otherwise
 *            0
 * Range:     u8 n <= 15, u16 n <= 441, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3)            /* special case */
        return 4;

    seq_t num = (seq_t)n * ((seq_t)n + 2) + ((n % 3) == 2 ? 1 : 0);
    seq_t res = num / 3;          /* exact division, result is integer */
    if ((n % 6) == 2)
        res--;                    /* subtract 1 if n mod 6 == 2 */
    return res;
}
