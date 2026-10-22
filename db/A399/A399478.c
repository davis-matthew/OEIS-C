/*
 * A399478  a(n) = n! * (H_{n-1} - 1/n), where H_n is the n-th harmonic
 *          number.
 * https://oeis.org/A399478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, new
 * Formula:   a(n) = n! * (H_{n-1} - 1/n) * Recurrence: a(1) = -1 * a(n) =
 *            n*a(n-1) + (n+1)*(n-2)! for n >= 2
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)-1;
    seq_t fact = 1;      /* (i-2)! for current i */
    seq_t prev = (seq_t)-1;  /* a(1) */
    seq_t cur = 0;
    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)i * prev + ((seq_t)i + 1) * fact;
        fact = fact * ((seq_t)i - 1);
        prev = cur;
    }
    return cur;
}
