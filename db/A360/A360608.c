/*
 * A360608  Number of solutions to a 4 X n Ring-Ring puzzle on an empty grid.
 * https://oeis.org/A360608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the recurrence a(n) = 3*a(n-1) + 3*a(n-2) -
 *            10*a(n-3) - 3*a(n-4) + 5*a(n-5) + a(n-6) with initial terms
 *            a(0)=1, a(1)=0, a(2)=2, a(3)=1, a(4)=8, a(5)=12 The sequence is
 *            generated from the rational G.f.
 *            -(2x^2+x-1)(x^2+2x-1)/(x^6+5x^5-3x^4-10x^3+3x^2+3x-1)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first six terms directly */
    switch (n)
    {
        case 0: return 1;
        case 1: return 0;
        case 2: return 2;
        case 3: return 1;
        case 4: return 8;
        case 5: return 12;
    }

    /* Array v[0..5] holds a(n-6) .. a(n-1) respectively */
    seq_t v[6] = {1, 0, 2, 1, 8, 12};

    for (idx_t i = 6; i <= n; ++i)
    {
        /* Compute a(i) = 3*v[5] + 3*v[4] - 10*v[3] - 3*v[2] + 5*v[1] + v[0]  */
        /* Reorder to avoid unsigned underflow: (3*v5 + 3*v4 + 5*v1 + v0) - (10*v3 + 3*v2) */
        seq_t pos = 3 * v[5] + 3 * v[4] + 5 * v[1] + v[0];
        seq_t neg = 10 * v[3] + 3 * v[2];
        seq_t next = pos - neg;   /* pos >= neg for all n >= 6, so safe */

        /* Shift the window for next iteration */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = next;
    }

    return v[5];
}
