/*
 * A275873  a(n) = floor(r*a(n-1)) + floor(r*a(n-2)) + floor(r*a(n-3)), where
 *          r = 3/2, a(0) = a(1) = a(2) = 1.
 * https://oeis.org/A275873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(3/2*a(n-1)) + floor(3/2*a(n-2)) +
 *            floor(3/2*a(n-3)), a(0) = a(1) = a(2) = 1 Sequence A275873
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 52, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n <= 2)
        return (seq_t)1;

    /* Working variables: a(i-3), a(i-2), a(i-1) */
    seq_t a0 = (seq_t)1;  /* a(0) */
    seq_t a1 = (seq_t)1;  /* a(1) */
    seq_t a2 = (seq_t)1;  /* a(2) */
    seq_t ai;            /* a(i) */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t f0 = (a2 * (seq_t)3) / (seq_t)2; /* floor(3/2 * a(i-1)) */
        seq_t f1 = (a1 * (seq_t)3) / (seq_t)2; /* floor(3/2 * a(i-2)) */
        seq_t f2 = (a0 * (seq_t)3) / (seq_t)2; /* floor(3/2 * a(i-3)) */

        ai = f0 + f1 + f2;                    /* a(i) */

        /* Shift for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }

    return a2;
}
