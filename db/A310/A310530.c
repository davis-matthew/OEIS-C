/*
 * A310530  Coordination sequence Gal.5.134.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            2*a(n-6) + 2*a(n-7) - a(n-8) for n > 8 Initial values: a(0)=1,
 *            a(1)=4, a(2)=10, a(3)=16, a(4)=22, a(5)=28, a(6)=34, a(7)=40,
 *            a(8)=44
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[9] = {1,4,10,16,22,28,34,40,44};

    if (n <= 8)
        return init[n];

    seq_t buf[9];
    for (int i = 0; i < 9; ++i)
        buf[i] = init[i];

    for (idx_t k = 9; k <= n; ++k) {
        seq_t val =
            2*buf[(k-1)%9] - 2*buf[(k-2)%9] + 2*buf[(k-3)%9] -
            2*buf[(k-4)%9] + 2*buf[(k-5)%9] - 2*buf[(k-6)%9] +
            2*buf[(k-7)%9] - buf[(k-8)%9];
        buf[k%9] = val;
    }

    return buf[n%9];
}
