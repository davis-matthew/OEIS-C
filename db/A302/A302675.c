/*
 * A302675  Number of nX3 0..1 arrays with every element equal to 0, 1, 4 or 6
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) for n > 5 with initial terms: a(1)=3,
 *            a(2)=3, a(3)=8, a(4)=7, a(5)=11
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five known values directly */
    if (n == 1) return 3;
    if (n == 2) return 3;
    if (n == 3) return 8;
    if (n == 4) return 7;
    if (n == 5) return 11;

    /* For n >= 6, use the recurrence a(n)=a(n-1)+a(n-2) */
    seq_t prev2 = 7;   /* a(4) */
    seq_t prev1 = 11;  /* a(5) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t cur = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
