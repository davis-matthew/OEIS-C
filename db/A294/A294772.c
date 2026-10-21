/*
 * A294772  Number of permutations of [n] avoiding {4132, 4123, 1243}.
 * https://oeis.org/A294772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 40*a(n-2) + 83*a(n-3) - 97*a(n-4) * +
 *            64*a(n-5) - 22*a(n-6) + 3*a(n-7) * Initial values: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=76, a(6)=270
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values for n = 0..6 */
    static const seq_t init[7] = { (seq_t)1, (seq_t)1, (seq_t)2,
                                   (seq_t)6, (seq_t)21, (seq_t)76,
                                   (seq_t)270 };
    if (n <= 6)
        return init[n];

    /* Sliding window: a0..a6 represent a(n-7)..a(n-1) respectively */
    seq_t a0 = init[0], a1 = init[1], a2 = init[2], a3 = init[3];
    seq_t a4 = init[4], a5 = init[5], a6 = init[6];
    seq_t next;

    for (idx_t i = 7; i <= n; ++i) {
        /* Compute the recurrence using only addition and subtraction.
           The sequence is strictly increasing, so intermediate subtractions
           will not underflow the unsigned type. */
        next = (seq_t)10 * a6;
        next -= (seq_t)40 * a5;
        next += (seq_t)83 * a4;
        next -= (seq_t)97 * a3;
        next += (seq_t)64 * a2;
        next -= (seq_t)22 * a1;
        next += (seq_t)3 * a0;

        if (i == n)
            return next;

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = next;
    }
    /* Unreachable path: should never get here */
    return init[0];
}
