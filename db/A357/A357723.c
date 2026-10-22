/*
 * A357723  Number of ways to place a non-attacking black king and white king
 *          on an n X n board, up to rotation and reflection.
 * https://oeis.org/A357723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*(n-1)*(n^2 + 3*n + 2*(n%2))/8
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 429, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0, 1, or 2 the value is 0. */
    if (n < 3)
        return (seq_t)0;

    seq_t nm2 = (seq_t)(n - 2);
    seq_t nm1 = (seq_t)(n - 1);

    /* t1 = (n-2)*(n-1) */
    seq_t t1 = nm2 * nm1;

    /* t2 = n^2 + 3*n + 2*(n%2) */
    seq_t t2 = (seq_t)n * (seq_t)n + 3 * (seq_t)n + 2 * ((n) & 1);

    /* product = t1 * t2 */
    seq_t prod = t1 * t2;

    /* divide by 8 (right shift 3 bits) */
    return prod >> 3;
}
