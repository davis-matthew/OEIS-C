/*
 * A298030  Partial sums of A298029.
 * https://oeis.org/A298030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n^2 - 18*n + (8 if n even else 11)) / 2 Initial terms:
 *            a(0)=1, a(1)=4, a(2)=10, a(3)=22
 * Range:     u8 n <= 8, u16 n <= 121, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* small n handled explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)10;
    if (n == 3) return (seq_t)22;

    /* use arithmetic that avoids overflow for all values that fit in seq_t */
    seq_t nt = (seq_t)n;
    seq_t t   = nt * (nt - (seq_t)2);          /* t = n*(n-2) */
    seq_t part1 = (t / (seq_t)2) * (seq_t)9;   /* floor((9*t)/2) for even t part */
    seq_t part2 = ((t & (seq_t)1) * (seq_t)9) / (seq_t)2; /* remainder part */
    seq_t add   = (t & (seq_t)1) ? (seq_t)6 : (seq_t)4;   /* 6 if t odd, 4 if t even */
    return part1 + part2 + add;
}
