/*
 * A360644  Number of 3-dimensional tilings of a 2 X 2 X n box using 1 X 1 X 1
 *          cubes, 2 X 1 X 1 dominos, 2 X 2 X 1 plates and trominos (L-shaped
 *          connection of 3 cubes).
 * https://oeis.org/A360644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by a recurrence with auxiliary sequences
 *            a,b,c,d,e. The recurrence is a(n) = 12*a(n-1) + 4*b(n-1) +
 *            2*c(n-1) + d(n-1) + e(n-1) + 43*a(n-2) + 8*b(n-2) + c(n-2) +
 *            2*d(n-2) b(n) = 32*a(n-1) + 9*b(n-1) + 4*c(n-1) + 2*d(n-1) +
 *            e(n-1) c(n) = 60*a(n-1) + 16*b(n-1) + 6*c(n-1) + 4*d(n-1) + 2...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* previous two values for a and previous single values for b,c,d,e */
    seq_t a1 = 1, a2 = 0;
    seq_t b1 = 0, b2 = 0;
    seq_t c1 = 0, c2 = 0;
    seq_t d1 = 0, d2 = 0;
    seq_t e1 = 0, e2 = 0;

    /* iterate up to n */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t a_new = 12*a1 + 4*b1 + 2*c1 + d1 + e1
                    + 43*a2 + 8*b2 + c2 + 2*d2;
        seq_t b_new = 32*a1 + 9*b1 + 4*c1 + 2*d1 + e1;
        seq_t c_new = 60*a1 + 16*b1 + 6*c1 + 4*d1 + 2*e1;
        seq_t d_new = 14*a1 + 3*b1 + d1;
        seq_t e_new = 64*a1 + 13*b1 + 2*c1 + 2*d1;

        /* shift for next iteration */
        a2 = a1; a1 = a_new;
        b2 = b1; b1 = b_new;
        c2 = c1; c1 = c_new;
        d2 = d1; d1 = d_new;
        e2 = e1; e1 = e_new;
    }
    return a1; /* a(n) */
}
