/*
 * A315617  Coordination sequence Gal.3.37.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-9) - a(n-10), a(0)=1, a(1)=6, a(2)=12,
 *            a(3)=15, a(4)=18, a(5)=30, a(6)=33, a(7)=36, a(8)=42, a(9)=48,
 *            a(10)=54
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Known first 11 terms        */
    static const seq_t init[11] = {
        (seq_t)1,  (seq_t)6,  (seq_t)12, (seq_t)15,
        (seq_t)18, (seq_t)30, (seq_t)33, (seq_t)36,
        (seq_t)42, (seq_t)48, (seq_t)54
    };

    if (n < 11u)          /* return the precalculated term */
        return init[n];

    /* buffer that always contains the last 11 values:
       buf[0] -> a(i-10), …, buf[10] -> a(i-1) for the current i      */
    seq_t buf[11];
    for (idx_t i = 0; i < 11; ++i)
        buf[i] = init[i];

    for (idx_t i = 11; i <= n; ++i) {
        /* a(i) = a(i-1) + a(i-9) - a(i-10) */
        seq_t val = buf[10] + buf[2] - buf[1];

        /* shift left to drop a(i-10) and make room for a(i)   */
        for (idx_t j = 0; j < 10; ++j)
            buf[j] = buf[j + 1];
        buf[10] = val;                     /* newest value   */
    }

    return buf[10];                       /* a(n) is now stored here */
}
