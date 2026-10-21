/*
 * A263824  Permutation of the nonnegative integers: [6k+3, 6k+4, 6k+5, 6k,
 *          6k+1, 6k+2, ...].
 * https://oeis.org/A263824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 3*(-1)^floor(n/3)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0) = 3, a(1) = 4, a(2) = 5, a(3) = 0, a(4) = 1, a(5) = 2 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)n + (seq_t)3;          /* start with n + 3 */
    if (((n / 3) & 1) != 0)                  /* if floor(n/3) is odd */
        res -= (seq_t)6;                     /* subtract 6 to get n - 3 */
    return res;
}
