/*
 * A257602  Expansion of (1 + x + 21*x^2 + x^3 + x^4)/(1 - x)^5.
 * https://oeis.org/A257602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5),
 *            with initial values a(0) = 1, a(1) = 6, a(2) = 41, a(3) = 156,
 *            a(4) = 426
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 252, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values */
    static const seq_t init[5] = {1, 6, 41, 156, 426};

    /* Return directly for the first five terms */
    if (n < 5)
        return init[n];

    /* Use a sliding window of five previous values */
    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];

    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t next = 5 * a4 - 10 * a3 + 10 * a2 - 5 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;
}
