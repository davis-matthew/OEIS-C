/*
 * A265075  Coordination sequence for (3,4,4) tiling of hyperbolic plane.
 * https://oeis.org/A265075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 2*a(n-3) + a(n-4) - a(n-6) for n >= 7 Initial
 *            values: a(0)=1, a(1)=3, a(2)=6, a(3)=11, a(4)=18, a(5)=29,
 *            a(6)=46
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 45, u64 n <= 94, u128 n <= 190,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Known initial terms */
    static const seq_t init[7] = {
        (seq_t)1,   /* a(0) */
        (seq_t)3,   /* a(1) */
        (seq_t)6,   /* a(2) */
        (seq_t)11,  /* a(3) */
        (seq_t)18,  /* a(4) */
        (seq_t)29,  /* a(5) */
        (seq_t)46   /* a(6) */
    };

    if (n < 7)
        return init[n];

    /* Circular buffer holding the last 7 computed values
       buf[i] corresponds to a(i) for i modulo 7 */
    seq_t buf[7];
    for (idx_t i = 0; i < 7; ++i)
        buf[i] = init[i];

    for (idx_t i = 7; i <= n; ++i) {
        /* indices modulo 7 */
        idx_t i2 = (i + 5) % 7;  /* i-2 */
        idx_t i3 = (i + 4) % 7;  /* i-3 */
        idx_t i4 = (i + 3) % 7;  /* i-4 */
        idx_t i6 = (i + 1) % 7;  /* i-6 */

        seq_t term = buf[i2] + (buf[i3] + buf[i3]) + buf[i4];
        term -= buf[i6];

        buf[i % 7] = term;
    }
    return buf[n % 7];
}
