/*
 * A241125  Number of n X 2 0..2 arrays with no element equal to exactly two
 *          horizontal and vertical neighbors, with new values 0..2 introduced
 *          in row major order.
 * https://oeis.org/A241125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 2*a(n-2) – 9*a(n-3) – 10*a(n-4) + 8*a(n-5) for
 *            n > 6, with initial values a(1)=2, a(2)=9, a(3)=54, a(4)=325,
 *            a(5)=1965, a(6)=11876
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial segment directly. */
    if (n == 0) {
        return (seq_t)0;   /* not defined for the sequence, but safe */
    }
    if (n <= 6) {
        /* The first six terms are constant. */
        static const seq_t base[6] = {2, 9, 54, 325, 1965, 11876};
        return base[n - 1];
    }

    /* Keep the last six values needed by the recurrence. */
    seq_t v[6];
    v[0] = 2;
    v[1] = 9;
    v[2] = 54;
    v[3] = 325;
    v[4] = 1965;
    v[5] = 11876;

    for (idx_t i = 7; i <= n; ++i) {
        /* Compute a(i) from the previous five terms. */
        seq_t next = 6 * v[5] + 2 * v[4] - 9 * v[3] - 10 * v[2] + 8 * v[1];

        /* Shift the window of values. */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = next;
    }

    return v[5];
}
