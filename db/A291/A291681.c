/*
 * A291681  First differences of A067046.
 * https://oeis.org/A291681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n+1)*(n+2)*(9 + (3+2*n)*(-1)^n)/24
 * Range:     i8 n <= 1, i16 n <= 23, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                 /* cast index to seq_t */
    seq_t prod = (nn + 1) * (nn + 2);    /* (n+1)*(n+2) */

    /* (-1)^n  => 1 if n even, -1 if n odd */
    seq_t sign = (n & 1) ? (seq_t)-1 : (seq_t)1;

    seq_t inner = 9 + (3 + 2 * nn) * sign;   /* 9 + (3+2n)*(-1)^n */
    prod = prod * inner;                    /* (n+1)*(n+2)*inner */
    prod = prod / 24;                       /* divide exactly */

    return prod;
}
