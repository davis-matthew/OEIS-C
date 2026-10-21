/*
 * A299645  Numbers of the form m*(8*m + 5), where m is an integer.
 * https://oeis.org/A299645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n*(n-1) - (2*n-1)*(-1)^n - 1)/4 Simplified: if n is
 *            odd: a(n) = (8*n*(n-1) + 2*n - 2)/4 if n is even: a(n) =
 *            (8*n*(n-1) - 2*n) /4
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1, but safe guard */
    idx_t nn = n;                  /* work in idx_t for safety */
    idx_t num;

    if (nn & 1) {                  /* n odd */
        num = 8 * nn * (nn - 1) + 2 * nn - 2;
    } else {                       /* n even */
        num = 8 * nn * (nn - 1) - 2 * nn;
    }

    return (seq_t)(num / 4);
}
