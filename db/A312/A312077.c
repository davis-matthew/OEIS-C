/*
 * A312077  Coordination sequence Gal.5.65.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; For n > 0 let k = n / 10 and r = n % 10. Then a(n) =
 *            44 * k + (r == 0 ? 0 : a_small[r]), where a_small[0..9] =
 *            {0,4,8,13,18,22,26,31,36,40}
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑computed values for 0 < r < 10 */
    static const seq_t a_small[10] = {
        (seq_t)0, /* unused */
        (seq_t)4,
        (seq_t)8,
        (seq_t)13,
        (seq_t)18,
        (seq_t)22,
        (seq_t)26,
        (seq_t)31,
        (seq_t)36,
        (seq_t)40
    };

    if (n == 0)
        return (seq_t)1;

    idx_t k   = n / 10;
    idx_t r   = n % 10;
    seq_t res = (seq_t)k * (seq_t)44;

    if (r != 0)
        res += a_small[r];

    return res;
}
