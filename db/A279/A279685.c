/*
 * A279685  The maximum number of coins that can be processed in n weighings
 *          of an adaptive strategy that all are real (and identical) except
 *          for one LHR-coin starting in an unknown state.
 * https://oeis.org/A279685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 5*a(n-3) - 4*a(n-4) for n > 9 with
 *            initial values a(0) = 1, a(1) = 1, a(2) = 3, a(3) = 6, a(4) =
 *            16, a(5) = 39, a(6) = 91, a(7) = 216, a(8) = 499, a(9) = 1144
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial terms a(0) to a(9) */
    static const seq_t init[10] = {
        1, 1, 3, 6, 16, 39, 91, 216, 499, 1144
    };

    if (n < 10)
        return init[n];

    /* Prepare the last four values needed for the recurrence:
       a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1). */
    seq_t a0 = init[6];   /* a(6) */
    seq_t a1 = init[7];   /* a(7) */
    seq_t a2 = init[8];   /* a(8) */
    seq_t a3 = init[9];   /* a(9) */

    /* Iterate from 10 up to n, updating the four values each step. */
    for (idx_t i = 10; i <= n; ++i) {
        /* a(i) = 3*a(n-1) - 3*a(n-2) + 5*a(n-3) - 4*a(n-4) */
        seq_t new_val = 3 * a3 - 3 * a2 + 5 * a1 - 4 * a0;

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = new_val;
    }

    return a3;
}
