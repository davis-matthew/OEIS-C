/*
 * A281239  Solutions x to the negative Pell equation y^2 = 72*x^2 - 83232
 *          with x,y >= 0.
 * https://oeis.org/A281239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) for n > 6 Initial values: a(1)=34,
 *            a(2)=38, a(3)=66, a(4)=102, a(5)=162, a(6)=358
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 33, u64 n <= 71, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[6] = {
        (seq_t)34,
        (seq_t)38,
        (seq_t)66,
        (seq_t)102,
        (seq_t)162,
        (seq_t)358
    };

    if (n >= 1 && n <= 6)
        return base[n - 1];

    /* Buffer holds the last six computed terms:
       buf[0] = a(n-6), buf[1] = a(n-5), ..., buf[5] = a(n-1) */
    seq_t buf[6] = { base[0], base[1], base[2], base[3], base[4], base[5] };
    seq_t next;

    for (idx_t k = 7; k <= n; ++k) {
        next = (seq_t)6 * buf[3] - buf[0];  /* a(n) = 6*a(n-3) - a(n-6) */
        /* shift buffer left: discard oldest, shift others */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = next;
    }

    return buf[5];  /* a(n) is now in the last buffer slot */
}
