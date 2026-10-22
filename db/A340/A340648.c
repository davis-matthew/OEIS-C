/*
 * A340648  a(n) is the maximum number of nonzero entries in an n X n
 *          sign-restricted matrix.
 * https://oeis.org/A340648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^2-n)/4 if n mod 4 == 0 or 3; a(n) = (3*n^2-n+2)/4 if
 *            n mod 4 == 1 or 2
 * Range:     u8 n <= 9, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to seq_t */
    seq_t n2 = nn * nn;          /* n squared (fits for n up to 10000) */
    seq_t num;

    switch (n & 3ULL) {          /* n mod 4 */
    case 0:
    case 3:
        num = 3 * n2 - nn;
        break;
    default:                     /* 1 or 2 */
        num = 3 * n2 - nn + 2;
        break;
    }

    return num / 4;              /* exact integer division */
}
