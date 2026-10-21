/*
 * A254228  Indices of heptagonal numbers (A000566) which are also centered
 *          square numbers (A001844).
 * https://oeis.org/A254228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+322*a(n-2)-322*a(n-3)-a(n-4)+a(n-5);
   Initial terms: a(1)=1, a(2)=41, a(3)=185, a(4)=13105, a(5)=59473. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not used, defensive */

    /* Return the initial terms directly. */
    if (n <= 5) {
        static const seq_t init[5] = {
            (seq_t)1,
            (seq_t)41,
            (seq_t)185,
            (seq_t)13105,
            (seq_t)59473
        };
        return init[n - 1];
    }

    /* Keep the last five values needed for the recurrence. */
    seq_t a1 = (seq_t)1;           /* a(n-5) */
    seq_t a2 = (seq_t)41;          /* a(n-4) */
    seq_t a3 = (seq_t)185;         /* a(n-3) */
    seq_t a4 = (seq_t)13105;       /* a(n-2) */
    seq_t a5 = (seq_t)59473;       /* a(n-1) */

    for (idx_t k = 6; k <= n; ++k) {
        /* Compute a(k) = a5 + 322*a4 - 322*a3 - a2 + a1 */
        seq_t term = a5
                   + (seq_t)322 * a4
                   - (seq_t)322 * a3
                   - a2
                   + a1;

        /* Shift the window for the next iteration. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = term;
    }

    return a5;
}
