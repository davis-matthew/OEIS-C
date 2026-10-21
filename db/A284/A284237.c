/*
 * A284237  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 833", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A284237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) for n > 5 Initial values:
 *            a(0)=1, a(1)=0, a(2)=3, a(3)=5, a(4)=10, a(5)=21
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t base[6] = {1, 0, 3, 5, 10, 21};

    if (n < 6) {
        return base[n];
    }

    /* Initialize the last three terms for i = 6: a(3), a(4), a(5) */
    seq_t v0 = base[3]; /* a(i-3) */
    seq_t v1 = base[4]; /* a(i-2) */
    seq_t v2 = base[5]; /* a(i-1) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = 2 * v2 + v1 - 2 * v0; /* recurrence */
        v0 = v1;                          /* shift window */
        v1 = v2;
        v2 = next;
    }
    return v2;
}
