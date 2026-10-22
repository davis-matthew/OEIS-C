/*
 * A377117  a(n) = coefficient of the term that is independent of 2^(1/3) and
 *          2^(2/3) in the expansion of (2^(1/3) + 2^(2/3))^n.
 * https://oeis.org/A377117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2) + 6*a(n-3) for n>=5 Initial values: a(0)=1,
 *            a(1)=1, a(2)=4, a(3)=6, a(4)=24
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for n = 0..4 */
    static const seq_t base[5] = { (seq_t)1, (seq_t)1, (seq_t)4,
                                  (seq_t)6, (seq_t)24 };

    if (n < 5)
        return base[n];

    /* Buffer holds a(i-3), a(i-2), a(i-1), a(i) for current i.
       Initial state corresponds to i = 4: a1, a2, a3, a4. */
    seq_t buf[4];
    buf[0] = base[1];  /* a1 */
    buf[1] = base[2];  /* a2 */
    buf[2] = base[3];  /* a3 */
    buf[3] = base[4];  /* a4 */

    seq_t val;
    for (idx_t i = 5; i <= n; ++i)
    {
        /* a(i) = 6*a(i-1) + 6*a(i-2) with a(i-1)=buf[2], a(i-2)=buf[1] */
        val = (seq_t)6 * buf[2] + (seq_t)6 * buf[1];

        /* Shift the queue to incorporate the new value */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = val;
    }

    return buf[3];
}
