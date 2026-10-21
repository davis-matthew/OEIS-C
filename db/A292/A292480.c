/*
 * A292480  p-INVERT of the odd positive integers, where p(S) = 1 - S^2.
 * https://oeis.org/A292480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 6*a(n-3) for n >= 5 * a(0) = 0,
 *            a(1) = 1, a(2) = 6, a(3) = 20, a(4) = 56
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first five terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)20;
    if (n == 4) return (seq_t)56;

    /* Sliding window for the recurrence */
    seq_t v0 = (seq_t)6;   /* a(n-3) */
    seq_t v1 = (seq_t)20;  /* a(n-2) */
    seq_t v2 = (seq_t)56;  /* a(n-1) */
    seq_t next;

    /* Compute terms up to a(n) */
    for (idx_t i = 5; i <= n; ++i) {
        next = 4 * v2 - 5 * v1 + 6 * v0;
        v0 = v1;
        v1 = v2;
        v2 = next;
    }
    return v2;
}
