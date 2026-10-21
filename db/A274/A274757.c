/*
 * A274757  Numbers k such that 6*k+1 is a triangular number (A000217).
 * https://oeis.org/A274757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(2*n-1)/2 for n even, * a(n) = 3*(n-1)*(2*n-1)/2 for
 *            n odd (OEIS A274757)
 * Range:     u8 n <= 7, u16 n <= 130, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the (unlikely) offset < 1 case gracefully */
    if (n == 0)
        return (seq_t)0;

    seq_t nn = (seq_t)n;
    seq_t t = ((seq_t)2 * nn) - 1;      /* 2*n - 1 */

    seq_t val;
    if ((n & 1ULL) == 0) {              /* n is even */
        val = (seq_t)3 * nn * t;
    } else {                            /* n is odd */
        val = (seq_t)3 * (nn - 1) * t;
    }
    return val / 2;                     /* division by 2 is exact */
}
