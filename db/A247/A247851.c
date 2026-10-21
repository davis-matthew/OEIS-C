/*
 * A247851  The 6th Hermite Polynomial evaluated at n: H_6(n) =
 *          64*n^6-480*n^4+720*n^2-120.
 * https://oeis.org/A247851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 64*n^6 - 480*n^4 + 720*n^2 - 120
 * Range:     i8 n <= 0, i16 n <= 3, i32 n <= 18, i64 n <= 724, i128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the 6th Hermite polynomial evaluated at n */
seq_t a(idx_t n)
{
    /* Promote the unsigned index to a seq_t */
    seq_t x = (seq_t)n;

    /* Compute powers */
    seq_t x2 = x * x;        /* n^2 */
    seq_t x4 = x2 * x2;      /* n^4 */
    seq_t x6 = x4 * x2;      /* n^6 */

    /* Evaluate the polynomial */
    seq_t res = 64 * x6;
    res -= 480 * x4;
    res += 720 * x2;
    res -= (seq_t)120;

    return res;
}
