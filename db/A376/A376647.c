/*
 * A376647  a(n) = Sum_{k=0..floor(n/3)} binomial(floor(k/2),n-3*k).
 * https://oeis.org/A376647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-6) + a(n-7) Initial values: a(0) = 1, a(1) = 0, a(2)
 *            = 0, a(3) = 1, a(4) = 0, a(5) = 0, a(6) = 1 Computation uses a
 *            linear recurrence, so a(n) can be obtained in O(n) time with an
 *            O(1) memory ring buffer
 * Range:     u8 n <= 63, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for n = 0..6 */
    static const seq_t base[7] = { (seq_t)1, (seq_t)0, (seq_t)0,
                                   (seq_t)1, (seq_t)0, (seq_t)0,
                                   (seq_t)1 };

    if (n <= 6)
        return base[n];

    /* 7-element ring buffer holding the last computed values */
    seq_t buf[7] = {
        base[0], base[1], base[2], base[3], base[4], base[5], base[6]
    };

    for (idx_t i = 7; i <= n; ++i)
    {
        seq_t val = buf[(i - 6) % 7] + buf[(i - 7) % 7];
        buf[i % 7] = val;
    }

    return buf[n % 7];
}
