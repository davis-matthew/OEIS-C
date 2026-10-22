/*
 * A356837  Coordination sequence of the {3,5,3} hyperbolic honeycomb.
 * https://oeis.org/A356837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) - 11*a(n-2) + 29*a(n-3) - 29*a(n-4) * +
 *            11*a(n-5) - 13*a(n-6) + a(n-7), * with initial values * a(0)=1,
 *            a(1)=20, a(2)=260, a(3)=3212, a(4)=39470, * a(5)=484760,
 *            a(6)=5953532, a(7)=73117640
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial seven terms (a(0) through a(7)). */
    static const seq_t init[8] = {
        (seq_t)1L, (seq_t)20L, (seq_t)260L, (seq_t)3212L,
        (seq_t)39470L, (seq_t)484760L, (seq_t)5953532L,
        (seq_t)73117640L
    };

    if (n < 8)
        return init[n];

    /* Use a ring buffer to hold the previous 7 values.
       The buffer is indexed in the order a(n-7) ... a(n-1). */
    seq_t buf[7];
    for (idx_t i = 0; i < 7; ++i)
        buf[i] = init[i + 1];

    seq_t next = 0;
    for (idx_t i = 8; i <= n; ++i) {
        /* next = 13*buf[6] - 11*buf[5] + 29*buf[4] - 29*buf[3]
               + 11*buf[2] - 13*buf[1] + buf[0]  */
        seq_t pos = 13 * buf[6];
        pos += 29 * buf[4];
        pos += 11 * buf[2];
        pos += buf[0];

        seq_t neg = 11 * buf[5];
        neg += 29 * buf[3];
        neg += 13 * buf[1];

        next = pos - neg;

        /* Shift buffer: drop a(n-7), make room for new value. */
        for (idx_t j = 0; j < 6; ++j)
            buf[j] = buf[j + 1];
        buf[6] = next;
    }

    return next;
}
