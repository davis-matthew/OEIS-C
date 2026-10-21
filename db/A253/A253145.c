/*
 * A253145  Triangular numbers (A000217) omitting the term 1.
 * https://oeis.org/A253145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = (n+1)*(n+2)/2 for n > 0; a(0) = 0
 * Range:     u8 n <= 21, u16 n <= 360, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;
    seq_t tn = (seq_t)n;
    seq_t a = tn + 1;
    seq_t b = tn + 2;
    /* divide the even factor first to avoid overflow on small seq_t */
    if ((a & 1) == 0)
        a = a / 2;
    else
        b = b / 2;
    return a * b;
}
