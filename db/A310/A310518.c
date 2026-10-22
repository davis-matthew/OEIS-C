/*
 * A310518  Coordination sequence Gal.3.21.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-5) - a(n-6) for n > 6 with initial values
 *            a(0)=1, a(1)=4, a(2)=10, a(3)=16, a(4)=22, a(5)=26, a(6)=30
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* constant base values */
    const seq_t base[7] = {
        (seq_t)1,
        (seq_t)4,
        (seq_t)10,
        (seq_t)16,
        (seq_t)22,
        (seq_t)26,
        (seq_t)30
    };

    if (n < 7UL) {
        return base[(int)n];
    }

    /* circular buffer holding the last 7 computed values */
    seq_t buf[7];
    for (int i = 0; i < 7; ++i) {
        buf[i] = base[i];
    }

    for (idx_t i = 7; i <= n; ++i) {
        int i1 = (int)((i - 1) % 7);
        int i5 = (int)((i - 5) % 7);
        int i6 = (int)((i - 6) % 7);
        seq_t new_val = buf[i1] + buf[i5] - buf[i6];
        buf[(int)(i % 7)] = new_val;
    }

    return buf[(int)(n % 7)];
}
