/*
 * A379726  Minimum number of kings that must be placed on an n X n chessboard
 *          such that each square is attacked or occupied by at least two
 *          kings.
 * https://oeis.org/A379726
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * floor((n+2)/3)^2 if n mod 3 != 0 a(n) = 2 * (n/3)^2 +
 *            (n/3) if n mod 3 == 0 Offset: 2
 * Range:     u8 n <= 33, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;              /* below offset, value does not matter */

    if (n % 3) {               /* n not a multiple of 3 */
        idx_t k = (n + 2) / 3;
        seq_t kk = (seq_t)k;
        return (seq_t)2 * kk * kk;
    } else {                   /* n is a multiple of 3 */
        idx_t t = n / 3;
        seq_t tt = (seq_t)t;
        return (seq_t)2 * tt * tt + tt;
    }
}
