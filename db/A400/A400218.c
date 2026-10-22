/*
 * A400218  Scaled z-coordinates of the vertices of a Boerdijk-Coxeter helix
 *          starting at a tetrahedron with vertices (-1,-1,-1), (1, -1, 1),
 *          (-1, 1, 1), and (1, 1, -1).
 * https://oeis.org/A400218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 27*a(n-3) + 3*a(n-2) - a(n-1) for n>=6 with initial
 *            values: a(0) = -1, a(1) = 1, a(2) = 1, a(3) = -1, a(4) = 5, a(5)
 *            = 1 (the recurrence follows from the given generating function)
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 22, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial terms for n < 6 */
    static const seq_t init[6] = {
        (seq_t)-1, /* a(0) */
        (seq_t) 1, /* a(1) */
        (seq_t) 1, /* a(2) */
        (seq_t)-1, /* a(3) */
        (seq_t) 5, /* a(4) */
        (seq_t) 1  /* a(5) */
    };

    if (n < 6) {
        return init[n];
    }

    /* we only need the last three values to compute the next one */
    seq_t prev3 = init[3]; /* a(n-3) */
    seq_t prev2 = init[4]; /* a(n-2) */
    seq_t prev1 = init[5]; /* a(n-1) */
    seq_t val;

    for (idx_t i = 6; i <= n; ++i) {
        val = (seq_t)27 * prev3 + (seq_t)3 * prev2 - prev1;
        /* shift the window for the next iteration */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = val;
    }

    return prev1;
}
