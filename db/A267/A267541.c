/*
 * A267541  Expansion of (2 + 4*x + x^2 + x^3 + 2*x^4 + x^5)/(1 - x - x^5 +
 *          x^6).
 * https://oeis.org/A267541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-5) - a(n-6) (a(0)=2, a(1)=6, a(2)=7, a(3)=8,
 *            a(4)=10, a(5)=13)
 * Range:     u8 n <= 115, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values a(0)..a(5) */
    static const seq_t init[6] = { 2, 6, 7, 8, 10, 13 };

    if (n < 6)               /* return the known terms directly */
        return init[n];

    /* circular buffer holding the last six sequence values */
    seq_t last[6];
    for (int i = 0; i < 6; ++i)
        last[i] = init[i];

    seq_t next;
    for (idx_t i = 6; i <= n; ++i) {
        /* recurrence: a_i = a_{i-1} + a_{i-5} - a_{i-6} */
        next = last[5] + last[1] - last[0];

        /* shift buffer to make room for the new value */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = last[5];
        last[5] = next;
    }

    return last[5];
}
