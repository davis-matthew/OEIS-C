/*
 * A300522  a(n) = (5*n + 3)*(5*n + 4)*(5*n + 5)/6.
 * https://oeis.org/A300522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 3)*(5*n + 4)*(5*n + 5)/6
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 589, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t N = (seq_t)n;

    /* Compute the three factors */
    seq_t f1 = 5 * N + 3;
    seq_t f2 = 5 * N + 4;
    seq_t f3 = 5 * N + 5;

    /* Divide by 2 (choose any even factor) */
    if ((f1 & 1) == 0)
        f1 /= 2;
    else if ((f2 & 1) == 0)
        f2 /= 2;
    else
        f3 /= 2;

    /* Divide by 3 (choose any factor divisible by 3) */
    if ((f1 % 3) == 0)
        f1 /= 3;
    else if ((f2 % 3) == 0)
        f2 /= 3;
    else
        f3 /= 3;

    /* Multiply the remaining parts */
    return f1 * f2 * f3;
}
