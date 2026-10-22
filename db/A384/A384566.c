/*
 * A384566  Area of the unique primitive Pythagorean triple whose inradius is
 *          A002378(n) and such that its long leg and its hypotenuse are
 *          consecutive natural numbers.
 * https://oeis.org/A384566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^2 + n + 1)*(2*n^2 + 2*n + 1)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 35, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn  = (seq_t)n;
    seq_t sn1 = sn + 1;
    seq_t sn2 = sn * sn;                        /* n^2          */
    seq_t t1   = sn * sn1;                      /* n*(n+1)      */
    seq_t t2   = sn2 + sn + 1;                  /* n^2+n+1      */
    seq_t t3   = (sn2 << 1) + (sn << 1) + 1;    /* 2*n^2+2*n+1  */
    return t1 * t2 * t3;
}
