/*
 * A289797  p-INVERT of the triangular numbers (A000217), where p(S) = 1 - S -
 *          S^2.
 * https://oeis.org/A289797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 5, a(2) = 21, a(3) = 84, a(4) = 330, a(5) =
 *            1291 a(n) = 7*a(n-1) - 17*a(n-2) + 23*a(n-3) - 16*a(n-4) +
 *            6*a(n-5) - a(n-6) for n ≥ 6 (Computed iteratively; no dynamic
 *            allocation required.)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial terms for the recurrence */
    static const seq_t init[6] = {1, 5, 21, 84, 330, 1291};

    if (n < 6)
        return init[n];

    /* working variables holding the last six values */
    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];
    seq_t a5 = init[5];
    seq_t next;

    for (idx_t i = 6; i <= n; ++i) {
        /* Compute a(i) using only additions and subtractions of
           non‑negative values, so the intermediate result never
           underflows for unsigned types. */
        next = 7 * a5 + 23 * a3 + 6 * a1;
        next -= 17 * a4;
        next -= 16 * a2;
        next -= a0;

        /* Slide the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }

    return a5;
}
