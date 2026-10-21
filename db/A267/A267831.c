/*
 * A267831  Expansion of (1 + 5*x - 7*x^2 - 3*x^3)/((1 - x)*(1 + x^2)^2).
 * https://oeis.org/A267831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(4*k) = 1 a(4*k+1) = 16*k + 6 a(4*k+2) = -3 a(4*k+3) =
 *            -16*(k+1)
 * Range:     i8 n <= 32, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = n / 4;          /* floor division by 4 */
    seq_t k = (seq_t)q;       /* cast for seq_t operations */
    switch (n % 4) {
    case 0:
        return (seq_t)1;
    case 1:
        return 16 * k + 6;
    case 2:
        return (seq_t)-3;
    case 3:
        return -16 * (k + 1);
    }
    /* unreachable, but required to satisfy the compiler */
    return (seq_t)0;
}
