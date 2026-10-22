/*
 * A308026  a(n) = n*(2*n - 3 - (-1)^n)*(11*n + (-1)^n)/24.
 * https://oeis.org/A308026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n - 3 - (-1)^n)*(11*n + (-1)^n)/24
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                     /* offset starts at 1, but guard unused value */
        return (seq_t)0;

    seq_t N = (seq_t)n;             /* n as seq_t for multiplication */
    int  s   = (n & 1) ? -1 : 1;    /* (-1)^n : 1 if even, -1 if odd */

    int  term1 = 2 * (int)n - 3 - s; /* 2*n - 3 - (-1)^n  (always ≥0) */
    int  term2 = 11 * (int)n + s;    /* 11*n + (-1)^n    (always ≥0) */

    seq_t prod = N * (seq_t)term1;   /* first multiplication */
    prod *= (seq_t)term2;           /* second multiplication */
    prod /= (seq_t)24;              /* exact division */

    return prod;
}
