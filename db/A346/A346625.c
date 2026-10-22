/*
 * A346625  a(0) = 8; for n > 0, a(n) = a(n-1)^2 - 2.
 * https://oeis.org/A346625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 8; for n > 0, a(n) = a(n-1)^2 - 2 */

seq_t a(idx_t n) {
    /* Handle trivial case */
    if (n == 0) {
        return (seq_t)8;
    }

    /* Compute iteratively */
    seq_t val = (seq_t)8;
    /* idx_t is unsigned, so the loop is safe even for large n */
    for (idx_t i = 1; i <= n; ++i) {
        val = val * val - (seq_t)2;
    }
    return val;
}
