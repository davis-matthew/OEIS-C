/*
 * A265107  Expansion of (2*x^4+x^3+x)/(-x^2-2*x+1).
 * https://oeis.org/A265107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=2, a(3)=6, a(4)=16; for n>=5:
 *            a(n)=2*a(n-1)+a(n-2)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle base cases */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 6;
    if (n == 4) return 16;

    /* iterate using the linear recurrence */
    seq_t a_minus_two = 6;   /* a(3) */
    seq_t a_minus_one = 16;  /* a(4) */
    seq_t cur = 0;

    for (idx_t i = 5; i <= n; ++i) {
        cur = a_minus_one + a_minus_one + a_minus_two;  /* 2*a(i-1)+a(i-2) */
        a_minus_two = a_minus_one;
        a_minus_one = cur;
    }

    return cur;
}
