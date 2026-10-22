/*
 * A327882  a(n) = n*(2*(n-1))! for n > 0, a(0) = 1.
 * https://oeis.org/A327882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (2*(n-1))!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(n) = 2*n*(2*n-3)*a(n-1) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* a(1) = 1 */
    seq_t val = (seq_t)1;

    /* Compute a(n) iteratively using the recurrence */
    for (idx_t i = 2; i <= n; ++i) {
        /* factor = 2*i*(2*i-3) */
        seq_t ii = (seq_t)i;
        seq_t factor = (seq_t)2 * ii * ((seq_t)2 * ii - (seq_t)3);
        val = factor * val;
    }

    return val;
}
