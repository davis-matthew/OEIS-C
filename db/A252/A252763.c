/*
 * A252763  Numbers n such that the hexagonal number H(n) is equal to the sum
 *          of the pentagonal numbers P(m), P(m+1), P(m+2) and P(m+3) for some
 *          m.
 * https://oeis.org/A252763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Recurrence a(n) = 194*a(n-1) - a(n-2) - 48 with initial values
 *            a(1) = 8, a(2) = 1480
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;  /* not used, but safe */

    if (n == 1)
        return (seq_t)8;
    if (n == 2)
        return (seq_t)1480;

    seq_t prev2 = (seq_t)8;   /* a(1) */
    seq_t prev1 = (seq_t)1480;/* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)194 * prev1 - prev2 - (seq_t)48;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
