/*
 * A331987  a(n) = ((n + 1) - 9*(n + 1)^2 + 8*(n + 1)^3)/6.
 * https://oeis.org/A331987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(8*n+7)/6
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t a1 = nn;          /* n */
    seq_t b1 = nn + 1;      /* n+1 */
    seq_t c1 = 8 * nn + 7;  /* 8*n+7 */

    /* divide by 2 – one of a1, b1, c1 is even */
    if ((a1 & 1u) == 0)
        a1 = a1 >> 1;
    else if ((b1 & 1u) == 0)
        b1 = b1 >> 1;
    else
        c1 = c1 >> 1;

    /* divide by 3 – one of a1, b1 is a multiple of 3 */
    if (a1 % 3 == 0)
        a1 = a1 / 3;
    else if (b1 % 3 == 0)
        b1 = b1 / 3;
    else
        c1 = c1 / 3;

    /* compute the product */
    seq_t result = a1 * b1;
    result = result * c1;
    return result;
}
