/*
 * A295867  Numbers of the form A000217(n)*A007494(n) that are divisible by 3.
 * https://oeis.org/A295867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((m%2==0) ? (3*m/2) : (3*m+1)/2) * (m+1)*m/2, where m =
 *            floor((6*n+4)/5)
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS entry A295867: Numbers of the form A000217(n)*A007494(n) that are divisible by 3 */

seq_t a(idx_t n)
{
    seq_t m = ((seq_t)6 * n + 4) / 5;          /* m = floor((6*n+4)/5) */

    seq_t binom = (m + 1) * m / 2;            /* binomial(m+1,2) */

    seq_t factor;
    if ((m & 1) == 0)                         /* m is even */
        factor = (3 * m) / 2;
    else
        factor = (3 * m + 1) / 2;             /* m is odd */

    return factor * binom;
}
