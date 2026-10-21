/*
 * A291031  p-INVERT of the positive integers, where p(S) = 1 - 3*S + 2*S^3.
 * https://oeis.org/A291031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 27*a(n-2) + 36*a(n-3) - 27*a(n-4) + 9*a(n-5) -
 *            a(n-6) with initial terms: a(0)=3, a(1)=15, a(2)=70, a(3)=321,
 *            a(4)=1461, a(5)=6624
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first 6 terms directly. */
    if (n <= 5)
    {
        static const seq_t init[6] =
        {
            (seq_t)3,
            (seq_t)15,
            (seq_t)70,
            (seq_t)321,
            (seq_t)1461,
            (seq_t)6624
        };
        return init[n];
    }

    /* Buffers holding the previous 6 terms. */
    seq_t buf[6] =
    {
        (seq_t)3,
        (seq_t)15,
        (seq_t)70,
        (seq_t)321,
        (seq_t)1461,
        (seq_t)6624
    };

    for (idx_t i = 6; i <= n; ++i)
    {
        seq_t next =
            buf[5] * (seq_t)9
          - buf[4] * (seq_t)27
          + buf[3] * (seq_t)36
          - buf[2] * (seq_t)27
          + buf[1] * (seq_t)9
          - buf[0];

        /* Shift the buffer to make room for the new term. */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = next;
    }

    return buf[5];
}
