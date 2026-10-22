/*
 * A347838  Positive numbers that are congruent to 2, 5, or 11 modulo 12.
 * https://oeis.org/A347838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -1 + 3*n + 3*floor(n/3) (indices start at 1)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* guards against calls with n < 1, which are out of the defined range */
    if (n == 0)
        return 0;

    seq_t nn = (seq_t)n;          /* cast index to the integer type used for values */
    seq_t mul3   = 3 * nn;        /* 3*n  */
    seq_t div3   = nn / 3;        /* floor(n/3)   */
    seq_t res    = mul3 + 3 * div3 - 1;   /* -1 + 3*n + 3*floor(n/3) */
    return res;
}
