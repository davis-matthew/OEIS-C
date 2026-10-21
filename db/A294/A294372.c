/*
 * A294372  Expansion of (1-x)^4/(x^2 + 4*x + 1).
 * https://oeis.org/A294372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -4*a(n-1) - a(n-2) for n > 4, with initial values a(0)=1,
 *            a(1)=-8, a(2)=37, a(3)=-144, a(4)=540
 * Range:     i8 n <= 2, i16 n <= 7, i32 n <= 15, i64 n <= 32, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return 1;
    if (n == 1) return -8;
    if (n == 2) return 37;
    if (n == 3) return -144;
    if (n == 4) return 540;

    /* Use an iterative recurrence with O(1) memory. */
    seq_t prev2 = -144; /* a(3) */
    seq_t prev1 =  540; /* a(4) */
    seq_t cur;

    for (idx_t i = 5; i <= n; ++i) {
        cur   = -4 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
