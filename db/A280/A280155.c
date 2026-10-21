/*
 * A280155  Number of n X 2 0..1 arrays with no element unequal to a strict
 *          majority of its horizontal, vertical and antidiagonal neighbors,
 *          with the exception of exactly one element, and with new values
 *          introduced in order 0 sequentially upwards.
 * https://oeis.org/A280155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - 2*a(n-3) - 6*a(n-4) - 4*a(n-5) -
 *            a(n-6) for n > 8, with initial values: a(1)=0, a(2)=4, a(3)=8,
 *            a(4)=18, a(5)=40, a(6)=92, a(7)=208, a(8)=470 The function uses
 *            dynamic programming with a sliding window of the last six terms
 *            and does not rely on any global state. Arith...
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 56, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial terms a(1) … a(8) */
    static const seq_t init[8] = {
        (seq_t)0,  /* a(1) */
        (seq_t)4,  /* a(2) */
        (seq_t)8,  /* a(3) */
        (seq_t)18, /* a(4) */
        (seq_t)40, /* a(5) */
        (seq_t)92, /* a(6) */
        (seq_t)208,/* a(7) */
        (seq_t)470 /* a(8) */
    };

    /* Directly return the known initial values. */
    if (n <= 8)
        return init[n - 1];

    /* Window of the last six computed terms:
       prev[0] = a(i-6), prev[1] = a(i-5), … , prev[5] = a(i-1)
       Initially set to a(3) … a(8) when i = 9. */
    seq_t prev[6];
    prev[0] = init[2]; /* a(3) */
    prev[1] = init[3]; /* a(4) */
    prev[2] = init[4]; /* a(5) */
    prev[3] = init[5]; /* a(6) */
    prev[4] = init[6]; /* a(7) */
    prev[5] = init[7]; /* a(8) */

    /* Compute terms for i = 9 … n using the recurrence and slide the window. */
    for (idx_t i = 9; i <= n; ++i) {
        seq_t new_val = 2 * prev[5] + 3 * prev[4];
        new_val -= 2 * prev[3];
        new_val -= 6 * prev[2];
        new_val -= 4 * prev[1];
        new_val -= prev[0];

        /* Shift window forward: drop a(i-6), keep a(i-5)..a(i-1). */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = new_val;
    }

    /* prev[5] now holds a(n). */
    return prev[5];
}
