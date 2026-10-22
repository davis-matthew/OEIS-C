/*
 * A315515  Coordination sequence Gal.6.200.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = base[n] for n < 12, a(n) = 62 * (n/12) + (n%12 ?
 *            base[n%12] : 0) for n ≥ 12 where base = {1, 6, 11, 17, 22, 26,
 *            31, 36, 40, 45, 51, 56}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[12] = {1,6,11,17,22,26,31,36,40,45,51,56};

    if (n < 12)
        return base[n];

    idx_t q = n / 12;
    idx_t r = n % 12;

    seq_t val = (seq_t)q * 62;
    if (r)
        val += base[r];

    return val;
}
