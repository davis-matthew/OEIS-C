/*
 * A375116  Maximum number of squares covered (i.e., attacked) by 3
 *          independent (i.e., nonattacking) queens on an n X n chessboard.
 * https://oeis.org/A375116
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n - 43 - (n mod 2) for n >= 10; base cases: a(4)=16,
 *            a(5)=25, a(6)=35, a(7)=45, a(8)=55, a(9)=66
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle defined offset and small values explicitly */
    if (n == 4) return 16;
    if (n == 5) return 25;
    if (n == 6) return 35;
    if (n == 7) return 45;
    if (n == 8) return 55;
    if (n == 9) return 66;
    if (n < 4) return 0;           /* undefined, but harmless */

    /* n >= 10: use the closed‑form formula */
    seq_t nn = (seq_t)n;
    seq_t res = (seq_t)12 * nn - (seq_t)43 - (seq_t)(n % 2);
    return res;
}
