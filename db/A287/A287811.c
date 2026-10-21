/*
 * A287811  Number of septenary sequences of length n such that no two
 *          consecutive terms have distance 5.
 * https://oeis.org/A287811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 7, a(n) = 6*a(n-1) + 3*a(n-2) (OEIS A287811)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* return the two initial terms directly */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }
    if (n == (idx_t)1) {
        return (seq_t)7;
    }

    /* iterative computation using two previous values */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)7;   /* a(1) */
    seq_t an;

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        an = (seq_t)6 * a1 + (seq_t)3 * a0;  /* a(i) = 6*a(i-1) + 3*a(i-2) */
        a0 = a1;
        a1 = an;
    }
    return a1;   /* a(n) */
}
